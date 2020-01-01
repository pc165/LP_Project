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
        dQ_K.insert(mapIter(make_pair(k, j), dQ));
        //Esborrem deltaQ(x,i).
        dQ_K.erase(make_pair(k, i));
        //Afegim deltaQ(j,x) = deltaQ(i,x)‐ 2 A[j]* A[x].
        dQ = dQ_I[make_pair(i, k)] - 2 * a_[j] * a_[k];
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
    std::set<int> veins_comuns, veins_de_i, veins_de_j;
    pMAdj_->getVeins(i, j, veins_comuns, veins_de_i, veins_de_j);


    //cout << "Veins Comuns: \n";
    //for (auto &vei : veins_comuns)
    //    cout << vei << ' ';
    //cout << '\n';
    //cout << "Veins de " << i << " : \n";
    //for (auto &vei : veins_de_i)
    //    cout << vei << ' ';
    //cout << '\n';

    for (auto &vei : veins_comuns)
        modificaVei(i, j, vei, tipusVei::comu);

    for (auto &vei : veins_de_i) {
        if (vei == j || binary_search(veins_comuns.cbegin(), veins_comuns.cend(), vei)) {
            cout << vei << " Skipped \n ";
            continue;
        }
        modificaVei(i, j, vei, tipusVei::nomes_i);
    }
    cout << '\n';

    //cout << "Veins de " << j << " : \n";
    //for (auto &vei : veins_de_j)
    //    cout << vei << ' ';

    cout << '\n';
    for (auto &vei : veins_de_j) {
        if (vei == i || binary_search(veins_comuns.cbegin(), veins_comuns.cend(), vei)) {
            cout << vei << " Skipped \n ";
            continue;
        }
        modificaVei(i, j, vei, tipusVei::nomes_j);
    }

    //A5. Eliminem tots els veïns de i, per deixar la posició i de deltaQ buida..
    deltaQ_[i] = map<pair<int, int>, double>();
    deltaQ_[j].erase(make_pair(j, i));

    //A4. Mantenim el màxim de j i de cada veí al vector maxDeltaQ i modifiquem el seu valor de deltaM al heap hTotal si és necessari
    //Mantenim el màxim de dQ de j ?

    for (auto &vei : veins_de_j) {
        for (auto &dQ : deltaQ_[vei]) {
            if (dQ.second > maxDeltaQ_[vei].second) {
                maxDeltaQ_[vei] = make_pair(dQ.first.second, dQ.second);
                hTotal_.modifElem(ElemHeap(dQ.second, make_pair(vei, dQ.first.second)));
            }
        }
    }

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
}

bool Comunitat::isDeleted(const int &i) {
    bool a = false, b = false;
    if (indexComs_[i].second > -1 && indexComs_[i].second < indexComs_.size())
        a = indexComs_[indexComs_[i].second].first == indexComs_[i].first;
    if (indexComs_[i].first > -1 && indexComs_[i].first < indexComs_.size())
        b = indexComs_[indexComs_[i].first].second = indexComs_[i].second;

    return a && b;
}

void Comunitat::generaDendrogram(int i, int j, double deltaQp1p2) {
    Tree<double> *tree = new Tree<double>(deltaQp1p2);
    tree->setLeft(vDendrograms_[j]);
    tree->setRight(vDendrograms_[i]);
    vDendrograms_[j] = tree;
    vDendrograms_[i] = nullptr;
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

    std::map<int, int> nodesFusionats;
    while (hTotal_.size() > 1) {
        auto max = hTotal_.max();
        auto i = max.getPos().first, j = max.getPos().second;

        hTotal_.modifElem(ElemHeap(-1, make_pair(j, i))); // modificar la posicio redundant (j,i)

        cout << "---------------------Fusio (" << i << ',' << j << "): " << max.getVal() << " -------------------" << hTotal_.size() << '\n';
        cout << "---------------------Before fusion\n";
        print_dQmap();
        cout << hTotal_ << "\n\n";

        hTotal_.delMax();
        if (max.getVal() > 0) {
            q_ += max.getVal();

            auto it = nodesFusionats.find(j);
            if (it != nodesFusionats.cend())
                j = it->second;
            nodesFusionats[i] = j;

            fusiona(i, j);
            cout << "---------------------After fusion\n";
            print_dQmap();
            cout << hTotal_ << "\n\n";
        }

        int z = 0;
        cout << "\nvDendrograms_\n";
        for (auto &i : vDendrograms_) {
            cout << z << '\n';
            if (i != nullptr)
                cout << *i;
            else
                cout << '\n';
            ++z;
        }
        cout << '\n';
    }
    int dummy = 0;
}
