#include "CaronteTest1.h"
#include "CaronteTest2.h"

int main() {
    //CaronteTest1 a;
    //CaronteTest2 b;
    //a.test();
    //b.test();
    string nomFitxer = "XarxaCom.txt";
    MatriuSparse m1(nomFitxer);
    Comunitat a(&m1);
    std::list<Tree<double> *> b;
    a.calculaComunitats(b);
}