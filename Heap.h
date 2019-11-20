#pragma once
#include <string>
#include <iostream>
#include <utility>
#include <vector>

class ElemHeap {
public:
    ElemHeap() { m_Val = 0; m_Pos = { 0, 0 }; }
    ElemHeap(double val, std::pair<int, int> pos) { m_Val = val; m_Pos = pos; };
    double getVal() const { return m_Val; }
    std::pair<int, int> getPos() const { return m_Pos; }
    bool operator<(const ElemHeap &e) { return (m_Val < e.m_Val); }
    bool operator<=(const ElemHeap &e) { return (m_Val <= e.m_Val); }
    bool operator>(const ElemHeap &e) { return (m_Val > e.m_Val); }
    bool operator>=(const ElemHeap &e) { return (m_Val >= e.m_Val); }
    bool operator==(const ElemHeap &e) { return (m_Val == e.m_Val); }
    ~ElemHeap() {};
    ElemHeap &operator=(const ElemHeap &e);
    friend std::ostream &operator<<(std::ostream &out, const ElemHeap &elHeap);
private:
    double m_Val;
    std::pair<int, int> m_Pos;
};

class Heap {
public:
    Heap() { m_act = -1; m_maxEls = 0; };
    Heap(int maxEls);
    Heap(const Heap &h);
    ~Heap() {};
    Heap &operator=(const Heap &h);
    ElemHeap &max() { return m_data[0]; }
    int size() { return m_act + 1; }
    friend std::ostream &operator<<(std::ostream &out, const Heap &h);
    void insert(const ElemHeap &el);
    void resize(int mida);
    void delMax();
    void modifElem(const ElemHeap &nouVal);
    void clear();
    bool operator==(const Heap &h);
private:
    std::vector<ElemHeap> m_data;
    //Guardem indexs del vei inicial per cada un dels valors que tenim guardats
    std::vector<int> m_index;
    int m_maxEls; //indica nombre total de nodes: array va de 0 a m_maxEls‐1
    int m_act; //indica posicio ultima ocupada: inicialment ‐1
    int pare(int pos) const { return ((pos - 1) / 2); }
    int fillEsq(int pos) const { return ((2 * pos) + 1); }
    int fillDret(int pos) const { return ((2 * pos) + 2); }
    void intercanvia(int pos1, int pos2);
    std::ostream &printRec(std::ostream &out, int pos, int n) const;
    void ascendir(int pos);
    void descendir(int pos);
};