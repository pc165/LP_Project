#pragma once
#include <list>
#include <map>
#include <vector>
#include "MatriuSparse.h"
#include "Tree.hpp"
#include "Heap.h"
class Comunitat {
public:
    Comunitat(MatriuSparse *pMAdj);
    ~Comunitat();
    void calculaM2() { m_M2 = m_pMAdj->getNValues(); }
    void calculaK() { m_pMAdj->calculaGrau(m_k); };
    void calculaA();
    void creaDeltaQHeap();
    void creaIndexComs();
    void InicialitzaDendrograms() { m_pMAdj->calculaDendograms(m_vDendrograms); }
    void calculaComunitats(list<Tree<double> *> &listDendrogram);
    void fusiona(int com1, int com2);
    void modificaVei(int com1, int com2, int vei, int cas);
    int getM2() { return m_M2; }
    std::vector<int> getK() { return m_k; }
    std::vector<double> getA() { return m_A; }
    std::vector<std::map<std::pair<int, int>, double>> getdeltaQ() { return m_deltaQ; }
    Heap gethTotal() { return m_hTotal; }
    std::vector<std::pair<int, int>> getIndexComs() { return m_indexComs; }
    void clear();
private:
//vector de maps per cada fila de deltaQij
    std::vector<std::map<std::pair<int, int>, double>> m_deltaQ;
    //vector mante llista de comunitats actives amb un parell que indica anterior i seguent activa.
    //la primera te com anterior -1 i la ultima com a seguent la mida del vector
    std::vector<std::pair<int, int>> m_indexComs;
    //vector que mante el maxim de deltaQij d'una fila, per no recalcular innecesariament
    std::vector<std::pair<int, double>> m_maxDeltaQFil;
    //index que indica quina es la primera comunitat activa
    int m_primComdeltaQ;
    //Vector d'arbres per dendrograma. Inicialment vector amb un arbre per cada node del graf.
    //Cada node fulla te index del node, cada node intern i l'arrel te el Q en el moment de la unio
    std::vector<Tree<double> *> m_vDendrograms;
    //Vector de graus dels nodes
    std::vector<int> m_k;
    //Vector index a de cada aresta
    std::vector<double> m_A;
    //Heap maxims deltaQ per fila. Num elems = comunitats actives (a inici = num nodes graf)
    Heap m_hTotal;
    //Modularitat Q de la particio en comunitats calculada
    double m_Q;
    //Nombre d’arestes *2 del graf
    int m_M2;
    MatriuSparse *m_pMAdj;
};