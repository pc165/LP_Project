#include "Comunitat.h"

Comunitat::Comunitat(MatriuSparse *pMAdj):pMAdj_(pMAdj) {}

Comunitat::~Comunitat() {
    if (pMAdj_ != nullptr)
        delete pMAdj_;
}

void Comunitat::clear() {
    deltaQ_.clear();
    indexComs_.clear();
    maxDeltaQFil_.clear();
    vDendrograms_.clear();
    k_.clear();
    A_.clear();
    hTotal_.clear();
    delete pMAdj_;
}
