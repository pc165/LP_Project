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

void Comunitat::modificaVei(int com1, int com2, int vei, int cas) {
}

void Comunitat::fusiona(int i, int j) {
    std::set<int> veinsComuns, veinsNomesDe_i, veinsNomesDe_j;
    pMAdj_->getVeins(i, j, veinsComuns, veinsNomesDe_i, veinsNomesDe_j);

    //A. 1. Pels x veïns de i i j:
    for (auto &vei : veinsComuns) {
        auto &dQ_Vei = deltaQ_[vei];
        auto &dQ_J = deltaQ_[j];
        auto &dQ_I = deltaQ_[i];
        //Modifiquem deltaQ(x,j). = deltaQ(x,i)+ deltaQ(x,j).
        dQ_Vei[make_pair(vei, j)] += dQ_Vei[make_pair(vei, i)];
        //Esborrem deltaQ[x,i].
        dQ_Vei.erase(make_pair(vei, i));
        //Modifiquem deltaQ(j,x) = deltaQ(i,x)+ deltaQ(j,x).
        dQ_J[make_pair(j, vei)] += dQ_I[make_pair(i, vei)];
    }

    //A.2. Pels x veïns només de i:
    for (auto &vei : veinsNomesDe_i) {
        auto &dQ_Vei = deltaQ_[vei];
        auto &dQ_J = deltaQ_[j];
        auto &dQ_I = deltaQ_[i];
        //Afegim deltaQ(x,j). = deltaQ(x,i) – 2 A[j]* A[x].
        double dQ = dQ_Vei[make_pair(vei, i)] - 2 * a_[j] * a_[vei];
        dQ_Vei.insert(mapIter(make_pair(vei, j), dQ));
        //Esborrem deltaQ(x,i).
        dQ_Vei.erase(make_pair(vei, i));
        //Afegim deltaQ(j,x) = deltaQ(i,x)‐ 2 A[j]* A[x].
        dQ = dQ_I[make_pair(i, vei)] - 2 * a_[j] * a_[vei];
        dQ_J.insert(mapIter(make_pair(j, vei), dQ));
    }

    //A.3. Pels x veïns només de j:
    for (auto &vei : veinsNomesDe_j) {
        auto &dQ_Vei = deltaQ_[vei];
        auto &dQ_J = deltaQ_[j];
        //Modifiquem deltaQ(x,j). = deltaQ(x,j) – 2 A[i]* A[x]
        dQ_Vei[make_pair(vei, j)] -= 2 * a_[i] * a_[vei];
        //Modifiquem deltaq(j,x) = deltaQ(j,x)‐ 2 A[i]* A[x].
        dQ_J[make_pair(j, vei)] -= 2 * a_[i] * a_[vei];
    }

    //A4. Mantenim el màxim de j i de cada veí al vector maxDeltaQ i modifiquem \
    el seu valor de deltaM al heap hTotal si és necessari
    //Mantenim el màxim de dQ de j ?
    auto &max = maxDeltaQ_[j];
    for (auto &j_2 : deltaQ_[j]) {
        const int &I = j_2.first.first;
        const int &J = j_2.first.second;
        const double dQ = j_2.second;
        if (dQ > max.second)
            max = make_pair(J, dQ);
        if (max.second > hTotal_.max().getVal())
            hTotal_.modifElem(ElemHeap(dQ, make_pair(I, J)));
    }

    //Mantenim el màxim de dQ de cada veí (de j?)
    for (auto &vei : veinsNomesDe_j) {
        for (auto &j_2 : deltaQ_[vei]) {
            const int &I = j_2.first.first;
            const int &J = j_2.first.second;
            const double dQ = j_2.second;
            if (dQ > max.second)
                max = make_pair(J, dQ);
            if (max.second > hTotal_.max().getVal())
                hTotal_.modifElem(ElemHeap(dQ, make_pair(I, J)));
        }
    }

    //A5. Eliminem tots els veïns de i, per deixar la posició i de deltaQ buida..
    //for (auto &vei : veinsNomesDe_i) {
    deltaQ_[i] = map<pair<int, int>, double>();
    //}

    //B1. Marquem la comunitat i com a esborrada al vector IndexComs.
    indexComs_[indexComs_[i].second].first = indexComs_[i].first;
    indexComs_[indexComs_[i].first].second = indexComs_[i].second;
    //L’índex primCom s’actualitazarà només si la comunitat eliminada és la primera.
    if (i == primComdeltaQ_)
        primComdeltaQ_ = j;
    // TODO
    //B2. Fusionem els arbres de les posicions i i j del vector vDendrogrames posant-los a la posició j del vector.
    //B3. Recalculem la A[j]=A[j]+A[i]
    a_[j] += a_[i];
}

void Comunitat::generaDendrogram(int i, int j, double deltaQp1p2) {
    Tree<double> *tree = new Tree<double>(deltaQp1p2);
    tree->setLeft(new Tree<double>(i));
    tree->setRight(new Tree<double>(j));
    delete vDendrograms_[i];
    delete vDendrograms_[j];
    vDendrograms_[i] = tree;
}

void Comunitat::calculaComunitats(list<Tree<double> *> &listDendrogram) {
    calculaM2();
    calculaK();
    calculaA();
    creaIndexComs();
    primComdeltaQ_ = 0;
    creaDeltaQHeap();
    //fusiona(3, 0);
    //Veiem que el vector k ja no el necessitem i per això un cop calculada la A es podria fer un clear() de la k
    //per tal de no tenir ocupada més memòria de la necessària.
    k_.clear();
    pMAdj_->calculaDendograms(vDendrograms_);
    //generaDendrogram(1, 2, 3);
    for (auto &i : vDendrograms_) {
        listDendrogram.push_back(i);
    }
}
