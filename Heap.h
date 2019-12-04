#pragma once
#include <iostream>
#include <string>
#include <utility>
#include <vector>

class ElemHeap {
  public:
    ElemHeap() {
        val_ = 0;
        pos = {0, 0};
    }
    ElemHeap(double val, std::pair<int, int> pos) {
        val_ = val;
        pos = pos;
    }
    double getVal() const { return val_; }
    std::pair<int, int> getPos() const { return pos; }
    bool operator<(const ElemHeap &e) { return (val_ < e.val_); }
    bool operator<=(const ElemHeap &e) { return (val_ <= e.val_); }
    bool operator>(const ElemHeap &e) { return (val_ > e.val_); }
    bool operator>=(const ElemHeap &e) { return (val_ >= e.val_); }
    bool operator==(const ElemHeap &e) { return (val_ == e.val_); }
    ~ElemHeap(){};
    ElemHeap &operator=(const ElemHeap &e) {
        val_ = e.val_;
        pos = e.pos;
    };
    friend std::ostream &operator<<(std::ostream &out, const ElemHeap &elHeap) {
        out << "Valor: " << val << " first: " << pos.first << " second: " << pos.second;
        return out;
    };

  private:
    double val_;
    std::pair<int, int> pos;
};

class Heap {
  public:
    Heap() : maxEls_(0), act_(-1){};
    Heap(int maxEls) : maxEls_(maxEls), act_(-1){};
    Heap(const Heap &h) : data_(h.data_), index_(h.index_), maxEls_(h.maxEls_), act_(h.act_){};
    ~Heap(){};
    Heap &operator=(const Heap &h);
    ElemHeap &max() { return data_[0]; }
    int size() { return act_ + 1; }
    friend std::ostream &operator<<(std::ostream &out, const Heap &h);
    void insert(const ElemHeap &el);
    void resize(int mida);
    void delMax();
    void modifElem(const ElemHeap &nouVal);
    void clear() { data_.clear(), index_.clear(), maxEls_ = 0, act_ = -1; };
    bool operator==(const Heap &h);

  private:
    std::ostream &printRec(std::ostream &out, int pos, int n) const;
    int getPare(int pos) const { return ((pos - 1) / 2); }
    int getFillEsq(int pos) const { return ((2 * pos) + 1); }
    int getFillDret(int pos) const { return ((2 * pos) + 2); }
    void ascendir(int pos);
    void descendeix(int pos);

  private:
    std::vector<ElemHeap> data_;
    //Guardem indexs del vei inicial per cada un dels valors que tenim guardats
    std::vector<int> index_;
    int maxEls_; //indica nombre total de nodes: array va de 0 a m_maxEls‐1
    int act_;    //indica posicio ultima ocupada: inicialment ‐1
};