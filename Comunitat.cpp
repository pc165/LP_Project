#include "Comunitat.h"
#include <algorithm>
#include <iterator>
#include <map>
#include <set>
#include <utility>
#include <vector>

#define DOUBLE(x) (static_cast<double>(x))
//pair of pairs, used to iterate througth a map of deltaQ_
typedef std::pair<std::pair<int, int>, double> mapIter;
//typedef std::pair<int, int> make_pair;

Comunitat::Comunitat(MatriuSparse *pMAdj) {
    primComdeltaQ_ = -1;
    q_ = 0;
    pMAdj_ = pMAdj;
    m2_ = 0;
}

Comunitat::~Comunitat() {
    pMAdj_ = nullptr;
}

void Comunitat::clear() {
    pMAdj_ = nullptr;
    deltaQ_.clear();
    indexComs_.clear();
    maxDeltaQ_.clear();
    primComdeltaQ_ = -1;
    vDendrograms_.clear();
    k_.clear();
    a_.clear();
    hTotal_.clear();
    q_ = 0;
    m2_ = 0;
}

void Comunitat::calculaA() {
    a_.resize(k_.size());
    for (int i = 0; i < k_.size(); i++) {
        a_[i] = k_[i] / DOUBLE(m2_);
    }
}

void Comunitat::creaIndexComs() {
    indexComs_.resize(pMAdj_->getNFiles());
    for (int i = 0; i < indexComs_.size(); i++) {
        indexComs_[i].first = i + 1;
        indexComs_[i].second = i - 1;
    }
}

void Comunitat::creaDeltaQHeap() {
    creaIndexComs();
    hTotal_.resize(pMAdj_->getNFiles());
    pMAdj_->creaMaps(deltaQ_);
    for (size_t i = 0; i < deltaQ_.size(); i++) {
        ElemHeap max(-2, make_pair(i, -1));
        for (auto &j : deltaQ_[i]) {
            auto &I = j.first.first;
            auto &J = j.first.second;
            j.second = (1 / DOUBLE(m2_)) - (DOUBLE(k_[I]) * k_[J]) / (DOUBLE(m2_) * m2_);
            if (j.second > max.getVal())
                max = ElemHeap(j.second, make_pair(j.first.first, j.first.second));
        }
        hTotal_.insert(max);
        maxDeltaQ_.push_back(make_pair(max.getPos().second, max.getVal()));
    }
}

void Comunitat::modificaVei(int i, int j, int k, tipusVei cas) {
    auto &dQ_K = deltaQ_[k];
    auto &dQ_J = deltaQ_[j];
    auto &dQ_I = deltaQ_[i];
    switch (cas) {
    case tipusVei::comu: {
        //A. 1. Pels x veïns de i i j:
        //Modifiquem deltaQ(x,j). = deltaQ(x,i)+ deltaQ(x,j).
        dQ_K[make_pair(k, j)] += dQ_K[make_pair(k, i)];
        //Esborrem deltaQ[x,i].
        dQ_K.erase(make_pair(k, i));
        //Modifiquem deltaQ(j,x) = deltaQ(i,x)+ deltaQ(j,x).
        dQ_J[make_pair(j, k)] += dQ_I[make_pair(i, k)];
        break;
    }
    case tipusVei::nomes_i: {
        //A.2. Pels x veïns només de i:
        //Afegim deltaQ(x,j). = deltaQ(x,i) – 2 A[j]* A[x].
        double dQ = dQ_K[make_pair(k, i)] - 2 * a_[j] * a_[k];
        if (k != j)
            dQ_K.insert(mapIter(make_pair(k, j), dQ));
        //Esborrem deltaQ(x,i).
        dQ_K.erase(make_pair(k, i));
        //Afegim deltaQ(j,x) = deltaQ(i,x)‐ 2 A[j]* A[x].
        dQ = dQ_I[make_pair(i, k)] - 2 * a_[j] * a_[k];
        if (k != j)
            dQ_J.insert(mapIter(make_pair(j, k), dQ));
        break;
    }
    case tipusVei::nomes_j: {
        //A.3. Pels x veïns només de j:
        //Modifiquem deltaQ(x,j). = deltaQ(x,j) – 2 A[i]* A[x]
        dQ_K[make_pair(k, j)] -= 2 * a_[i] * a_[k];
        //Modifiquem deltaq(j,x) = deltaQ(j,x)‐ 2 A[i]* A[x].
        dQ_J[make_pair(j, k)] -= 2 * a_[i] * a_[k];
        break;
    }
    }
}

void Comunitat::fusiona(int i, int j) {
    std::set<int> veins_de_i, veins_de_j;

    for (auto &veins : deltaQ_[i])
        veins_de_i.insert(veins.first.second);

    for (auto &veins : deltaQ_[j])
        veins_de_j.insert(veins.first.second);

    //A. 1. Si és un veí en comú amb la j:
    for (auto &vei : veins_de_i) {
        if (binary_search(veins_de_j.cbegin(), veins_de_j.cend(), vei))
            modificaVei(i, j, vei, tipusVei::comu);
    }

    //A.2. Si només és un veí de la i
    for (auto &vei : veins_de_i) {
        if (!binary_search(veins_de_j.cbegin(), veins_de_j.cend(), vei))
            modificaVei(i, j, vei, tipusVei::nomes_i);
    }

    //A.3. Per cada veí que només està a j
    for (auto &vei : veins_de_j) {
        if (!binary_search(veins_de_i.cbegin(), veins_de_i.cend(), vei))
            modificaVei(i, j, vei, tipusVei::nomes_j);
    }

    //A4. Mantenim el màxim de j i de cada veí al vector maxDeltaQ i modifiquem el seu valor de deltaM al heap hTotal si és necessari
    for (auto &K : deltaQ_[j])
        maxdQFila(K.first.second);
    maxdQFila(j);

    //A5. Eliminem tots els veïns de i, per deixar la posició i de deltaQ buida..
    deltaQ_[i] = map<pair<int, int>, double>();
    deltaQ_[j].erase(make_pair(j, i));

    //B1. Marquem la comunitat i com a esborrada al vector IndexComs.????
    if (indexComs_[i].second > -1 && indexComs_[i].second < indexComs_.size())
        indexComs_[indexComs_[i].second].first = indexComs_[i].first;
    if (indexComs_[i].first > -1 && indexComs_[i].first < indexComs_.size())
        indexComs_[indexComs_[i].first].second = indexComs_[i].second;
    //L’índex primCom s’actualitazarà només si la comunitat eliminada és la primera.
    if (i == primComdeltaQ_)
        primComdeltaQ_ = j;

    generaDendrogram(i, j, q_);
    //B3. Recalculem la A[j]=A[j]+A[i]
    a_[j] += a_[i];
}

void Comunitat::print_dQmap() {
    cout << "Map start\n";
    for (auto &i : deltaQ_) {
        for (auto &j : i) {
            cout << '(' << j.first.first << " , " << j.first.second << "): " << j.second << ' ';
        }
        cout << '\n';
    }
    cout << "Map end\n";

    cout << hTotal_ << "\n\n";
}

void Comunitat::generaDendrogram(int i, int j, double deltaQp1p2) {
    Tree<double> *tree = new Tree<double>(deltaQp1p2);
    tree->setLeft(vDendrograms_[j]);
    tree->setRight(vDendrograms_[i]);
    vDendrograms_[j] = tree;
    vDendrograms_[i] = nullptr;
}

void Comunitat::maxdQFila(const int &i) {
    std::pair<int, double> max(-1, -1);
    for (auto &dQ : deltaQ_[i])
        if (dQ.second > max.second)
            max = make_pair(dQ.first.second, dQ.second);
    maxDeltaQ_[i] = max;
    hTotal_.modifElem(ElemHeap(max.second, make_pair(i, max.first)));
}

void Comunitat::calculaComunitats(list<Tree<double> *> &listDendrogram) {
    calculaM2();
    calculaK();
    calculaA();
    creaIndexComs();
    primComdeltaQ_ = 0;
    creaDeltaQHeap();
    k_.clear();
    pMAdj_->calculaDendograms(vDendrograms_);

    while (hTotal_.size() > 1) {
        auto max = hTotal_.max();
        auto i = max.getPos().first, j = max.getPos().second;
        hTotal_.delMax();
        if (max.getVal() > 0) {
            q_ += max.getVal();
            fusiona(i, j);
        }
    }
    for (auto &i : vDendrograms_) {
        if (i != nullptr) {
            listDendrogram.push_back(i);
        }
    }
}
