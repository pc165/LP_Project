#pragma once
#include "Comunitat.h"
#include "MatriuSparse.h"
#include <iostream>
#include <list>
#include <string>

class Graph {
  public:
    Graph(){};
    Graph(std::string nomFitxerRels) : mAdjacencia_(nomFitxerRels){};
    ~Graph(){};
    friend std::ostream &operator<<(std::ostream &os, const Graph &g) {
        os << g.mAdjacencia_;
        return os;
    };
    void calculaComunitats(std::list<Tree<double> *> &listDendrogram) {
        Comunitat a(&mAdjacencia_);
        a.calculaComunitats(listDendrogram);
    };
    void clear() { mAdjacencia_.clear(); }

  private:
    MatriuSparse mAdjacencia_;
};