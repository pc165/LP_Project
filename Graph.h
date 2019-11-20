#pragma once
#include <iostream>
#include <list>
#include <string>
#include "MatriuSparse.h"
#include "Comunitat.h"

class Graph {
public:
    Graph() {};
    Graph(std::string nomFitxerRels):mAdjacencia_(nomFitxerRels) {};
    ~Graph() {};
    friend std::ostream &operator<<(std::ostream &os, const Graph &g) {
        os << g.mAdjacencia_; return os;
    };
    void calculaComunitats(list<Tree<double> *> &listDendrogram);
    void clear() { mAdjacencia_.clear(); }
private:
    MatriuSparse mAdjacencia_;
};