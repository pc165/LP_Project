#include "Comunitat.h"
#include <map>
#include <utility>
#include <vector>

#define DOUBLE(x) (static_cast<double>(x))
typedef std::pair<std::pair<int, int>, double> mapIter;
typedef std::pair<int, int> pairInt;

Comunitat::Comunitat(MatriuSparse *pMAdj) {
    primComdeltaQ_ = -1;
    q_ = 0;
    pMAdj_ = pMAdj;
}

Comunitat::~Comunitat() {
    pMAdj_ = nullptr;
}
void Comunitat::clear() {
    pMAdj_ = nullptr;
    deltaQ_.clear();
    indexComs_.clear();
    maxDeltaQFil_.clear();
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
        ElemHeap max(-2, pairInt(i, -1));
        for (auto &j : deltaQ_[i]) {
            j.second = (1 / DOUBLE(m2_)) - (DOUBLE(k_[j.first.first]) * k_[j.first.second]) / (DOUBLE(m2_) * m2_);
            if (j.second > max.getVal())
                max = ElemHeap(j.second, pairInt(j.first.first, j.first.second));
        }
        hTotal_.insert(max);
    }
}

void Comunitat::modificaVei(int com1, int com2, int vei, int cas) {
}

void Comunitat::fusiona(int com1, int com2) {
}

void Comunitat::generaDendrogram(int pos1, int pos2, double deltaQp1p2) {
}

void Comunitat::calculaComunitats(list<Tree<double> *> &listDendrogram) {
    calculaK();
    calculaA();
    creaIndexComs();
    primComdeltaQ_ = 0;
    creaDeltaQHeap();
}
