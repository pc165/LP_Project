#include "Comunitat.h"
#include <algorithm>
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

        //fusio
        //indexComs_[indexComs_[i].second].first = indexComs_[i].first;
        //indexComs_[indexComs_[i].first].second = indexComs_[i].second;
    }
}
void Comunitat::creaDeltaQHeap() {
    creaIndexComs();
    hTotal_.resize(pMAdj_->getNFiles());
    pMAdj_->creaMaps(deltaQ_);
    for (size_t i = 0; i < deltaQ_.size(); i++) {
        ElemHeap max(-2, make_pair(i, -1));
        for (auto &j : deltaQ_[i]) {
            j.second = (1 / DOUBLE(m2_)) - (DOUBLE(k_[j.first.first]) * k_[j.first.second]) / (DOUBLE(m2_) * m2_);
            if (j.second > max.getVal())
                max = ElemHeap(j.second, make_pair(j.first.first, j.first.second));
        }
        hTotal_.insert(max);
    }
}

void Comunitat::modificaVei(int com1, int com2, int vei, int cas) {
}

void Comunitat::fusiona(int i, int j) {
    auto pairdeVeins = pMAdj_->getVeins(i, j);
    auto &veinsDe_i = pairdeVeins.first;
    auto &veinsDe_j = pairdeVeins.second;
    //std::set<int> veins_unio(veinsDe_i, veinsDe_j); // set que conte els veis

    //A. Per cada veí de la i i per cada veí de la j mirem:
    //veins de J
    for (auto &vei : veinsDe_j) {
        auto &dQ_Vei = deltaQ_[vei];
        auto &dQ_J = deltaQ_[j];
        //A.1. Si és un veí en comú amb la j
        if (std::binary_search(veinsDe_j.begin(), veinsDe_j.begin(), vei)) {
            dQ_Vei[make_pair(vei, j)] += dQ_Vei[make_pair(vei, i)];
            dQ_J[make_pair(j, vei)] += dQ_J[make_pair(i, vei)];
        } else { //A.2. Si només és un veí de la i
            dQ_Vei[make_pair(vei, i)];
            double dQ = dQ_Vei[make_pair(vei, i)] - 2 * a_[j] * a_[vei];
            dQ_Vei.insert(mapIter(make_pair(vei, j), dQ));
            dQ_Vei.erase(make_pair(vei, i));
            dQ = dQ_J[make_pair(i, vei)] - 2 * a_[j] * a_[vei];
            dQ_J.insert(mapIter(make_pair(j, vei), dQ));
        }
        //I esborrem la relació del veí amb la i que ja desapareix.
        dQ_Vei.erase(make_pair(vei, i));
    }

    //veins de I
    for (auto &vei : veinsDe_i) {
        auto &dQ_Vei = deltaQ_[vei];
        auto &dQ_J = deltaQ_[j];
        //A.1. Si és un veí en comú amb la j
        if (std::binary_search(veinsDe_j.begin(), veinsDe_j.begin(), vei)) {
            dQ_Vei[make_pair(vei, j)] += dQ_Vei[make_pair(vei, i)];
            dQ_J[make_pair(j, vei)] += dQ_J[make_pair(i, vei)];
        } else { //A.2. Si només és un veí de la i
            dQ_Vei[make_pair(vei, i)];
            double dQ = dQ_Vei[make_pair(vei, i)] - 2 * a_[j] * a_[vei];
            dQ_Vei.insert(mapIter(make_pair(vei, j), dQ));
            dQ_Vei.erase(make_pair(vei, i));
            dQ = dQ_J[make_pair(i, vei)] - 2 * a_[j] * a_[vei];
            dQ_J.insert(mapIter(make_pair(j, vei), dQ));
        }
        //I esborrem la relació del veí amb la i que ja desapareix.
        dQ_Vei.erase(make_pair(vei, i));
    }

    //Un cop fet això mirem els veïns de j
    for (auto &vei : veinsDe_j) {
        //A.3Per cada veí que només està a j --> no esta en veinsDe_i
        auto &dQ_Vei = deltaQ_[vei];
        auto &dQ_J = deltaQ_[j];
        if (!std::binary_search(veinsDe_i.begin(), veinsDe_i.end(), vei)) {
            dQ_Vei[make_pair(vei, j)] -= 2 * a_[i] * a_[vei];
            dQ_J[make_pair(j, vei)] -= 2 * a_[i] * a_[vei];
        }
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
        if (max.second > hTotal_.max)
            hTotal_.modifElem(ElemHeap(dQ, make_pair(I, J)));
    }

    //Mantenim el màxim de dQ de cada veí (de j?)
    for (auto &vei : veinsDe_j) {
        for (auto &j_2 : deltaQ_[vei]) {
            const int &I = j_2.first.first;
            const int &J = j_2.first.second;
            const double dQ = j_2.second;
            if (dQ > max.second)
                max = make_pair(J, dQ);
            if (max.second > hTotal_.max)
                hTotal_.modifElem(ElemHeap(dQ, make_pair(I, J)));
        }
    }
    //A5. Eliminem tots els veïns de i, per deixar la posició i de deltaQ buida..
    for (auto &vei : veinsDe_i) {
        deltaQ_[vei] = map<pair<int, int>, double>();
    }


    // TODO
    //B1. Marquem la comunitat i com a esborrada al vector IndexComs.
    //B2. Fusionem els arbres de les posicions i i j del vector vDendrogrames posant-los a la posició j del vector.
    //B3 Recalculem la A[j]=A[j]+A[i]
}

void Comunitat::generaDendrogram(int pos1, int pos2, double deltaQp1p2) {
}

void Comunitat::calculaComunitats(list<Tree<double> *> &listDendrogram) {
    calculaK();
    calculaA();
    creaIndexComs();
    primComdeltaQ_ = 0;
    creaDeltaQHeap();
    pMAdj_->calculaDendograms(vDendrograms_);
}
