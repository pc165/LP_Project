#pragma once
#include "Heap.h"
#include "MatriuSparse.h"
#include "Tree.hpp"
#include <list>
#include <map>
#include <vector>
class Comunitat {
  public:
    Comunitat(MatriuSparse *pMAdj);
    ~Comunitat();
    void calculaM2() { m2_ = pMAdj_->getNValues(); }
    void calculaK() { pMAdj_->calculaGrau(k_); };
    void calculaA();
    void creaDeltaQHeap();
    void creaIndexComs();
    void InicialitzaDendrograms() { pMAdj_->calculaDendograms(vDendrograms_); }
    void calculaComunitats(std::list<Tree<double> *> &listDendrogram);
    void fusiona(int com1, int com2);
    void modificaVei(int com1, int com2, int vei, int cas);
    int getM2() { return m2_; }
    std::vector<int> getK() { return k_; }
    std::vector<double> getA() { return A_; }
    std::vector<std::map<std::pair<int, int>, double>> getdeltaQ() { return deltaQ_; }
    Heap gethTotal() { return hTotal_; }
    std::vector<std::pair<int, int>> getIndexComs() { return indexComs_; }
    void clear();

  private:
    //vector de maps per cada fila de deltaQij
    std::vector<std::map<std::pair<int, int>, double>> deltaQ_;
    //vector mante llista de comunitats actives amb un parell que indica anterior i seguent activa.
    //la primera te com anterior -1 i la ultima com a seguent la mida del vector
    std::vector<std::pair<int, int>> indexComs_;
    //vector que mante el maxim de deltaQij d'una fila, per no recalcular innecesariament
    std::vector<std::pair<int, double>> maxDeltaQFil_;
    //index que indica quina es la primera comunitat activa
    int primComdeltaQ_;
    //Vector d'arbres per dendrograma. Inicialment vector amb un arbre per cada node del graf.
    //Cada node fulla te index del node, cada node intern i l'arrel te el Q en el moment de la unio
    std::vector<Tree<double> *> vDendrograms_;
    //Vector de graus dels nodes
    std::vector<int> k_;
    //Vector index a de cada aresta
    std::vector<double> A_;
    //Heap maxims deltaQ per fila. Num elems = comunitats actives (a inici = num nodes graf)
    Heap hTotal_;
    //Modularitat Q de la particio en comunitats calculada
    double Q_;
    //Nombre d’arestes *2 del graf
    int m2_;
    MatriuSparse *pMAdj_;
};