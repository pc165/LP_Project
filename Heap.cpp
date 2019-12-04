#include "Heap.h"

Heap &Heap::operator=(const Heap &h) {
    *this = h;
    return *this;
}

inline void Heap::insert(const ElemHeap &el) {
    act_++;
    if (act_ < maxEls_)
        data_[act_] = el;
    else {
        data_.push_back(el);
        maxEls_++;
    }
    int posAct = act_;
    while ((posAct != 0) && (data_[posAct] < data_[getPare(posAct)])) {
        std::swap(data_[posAct], data_[getPare(posAct)]);
        posAct = getPare(posAct);
    }
}

inline void Heap::delMax() {
    if (act_ >= 0) {
        std::swap(data_[0], data_[act_]);
        act_--;
        descendeix(0);
    }
}

bool Heap::operator==(const Heap &h) {
    if (index_ == h.index_ && act_ == h.act_ && maxEls_ == h.maxEls_ && data_ == h.data_) return true;
    return false;
}

inline std::ostream &Heap::printRec(std::ostream &out, int pos, int n) const {
    if (pos <= act_) {
        for (int i = 0; i < n; i++) {
            out << "|--";
        }
        out << "|-->" << data_[pos] << '\n';
        if (((2 * pos) + 1) <= maxEls_) {
            printRec(out, getFillEsq(pos), n + 1);
        }
        if ((2 * pos) < maxEls_) {
            printRec(out, getFillDret(pos), n + 1);
        }
    }
    return out;
}

inline void Heap::ascendir(int pos) {
    int fEsq = getFillEsq(pos);
    int fDret = getFillDret(pos);
    int mesGran = pos;
    if ((fEsq >= act_) && (data_[fEsq] > data_[pos]))
        mesGran = fEsq;
    if ((fDret >= act_) && (data_[fDret] > data_[mesGran]))
        mesGran = fDret;
    if (mesGran != pos) {
        std::swap(data_[pos], data_[mesGran]);
        ascendir(mesGran);
    }
}

inline void Heap::descendeix(int pos) {
    int fEsq = getFillEsq(pos);
    int fDret = getFillDret(pos);
    int mesPetit = pos;
    if ((fEsq <= act_) && (data_[fEsq] < data_[pos]))
        mesPetit = fEsq;
    if ((fDret <= act_) && (data_[fDret] < data_[mesPetit]))
        mesPetit = fDret;
    if (mesPetit != pos) {
        std::swap(data_[pos], data_[mesPetit]);
        descendeix(mesPetit);
    }
}

std::ostream &operator<<(std::ostream &out, const Heap &h) {
    return this->printRec(out, h);
}
