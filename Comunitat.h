#pragma once
#include "Heap.h"
#include "MatriuSparse.h"
#include <list>
#include <map>
enum class tipusVei {
    comu,
    nomes_i,
    nomes_j
};
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
    void calculaComunitats(list<Tree<double> *> &listDendrogram);
    void fusiona(int com1, int com2);
    void modificaVei(int com1, int com2, int vei, tipusVei cas);
    int getM2() { return m2_; }
    vector<int> getK() { return k_; }
    vector<double> getA() { return a_; }
    vector<map<pair<int, int>, double>> getdeltaQ() { return deltaQ_; }
    Heap gethTotal() { return hTotal_; }
    vector<pair<int, int>> getIndexComs() { return indexComs_; }
    void clear();

  private:
    void print_dQmap();
    bool isDeleted(const int &i);
    void generaDendrogram(int pos1, int pos2, double deltaQp1p2);
    //vector de maps per cada fila de deltaQij
    vector<map<pair<int, int>, double>> deltaQ_;
    //vector que mante llista de comunitats actives amb un parell que indica anterior i seguent activa.
    //la primera te com anterior -1 i la ultima com a seguent la mida del vector
    vector<pair<int, int>> indexComs_;
    //vector que mante el maxim de deltaQij d'una fila, per no recalcular innecesariament
    vector<pair<int, double>> maxDeltaQ_;
    //index que indica quina es la primera comunitat activa
    int primComdeltaQ_;
    //Vector d'arbres que va creant el dendograma. Inicialment es un vector amb un arbre per cada node del graf.
    //Cada node fulla te el index del node, cada node intern i l'arrel te el deltaQij que l'ha fet unir-se. Si es negatiu vol dir que no hauriem de fer aquesta unio
    // |--arrel
    // |---->FillDret1
    // |------>FillDret2
    // |------>FillEsquerre2
    // |---->FillEsquerre1
    vector<Tree<double> *> vDendrograms_;
    //Vector de graus dels nodes
    vector<int> k_;
    //Vector de index aij de cada aresta
    vector<double> a_;
    //Heap dels maxims deltaQ de cada fila, tindra nfiles elements a l'inici i despres tants com comunitats hi hagi actives
    Heap hTotal_; // (_mAdjacencia.getNFiles());
    //Modularitat Q de la particio en comunitats calculada
    double q_;
    int m2_;
    MatriuSparse *pMAdj_;
};