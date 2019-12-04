#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

template <class T>
class Tree {
  public:
    Tree();
    Tree(const Tree<T> &t);
    Tree(std::string nomFitxer);
    ~Tree();
    bool isLeave() const { return ((left_ == NULL) && (right_ == NULL)); }
    bool isEmpty() const { return (data_ == NULL); }
    Tree<T> *getRight() { return right_; }
    Tree<T> *getLeft() { return left_; }
    T &getData() { return (*data_); }
    float avalua();
    void mostraExpressio();
    bool cerca(const T &val, Tree<T> *valTrobat);

    friend std::ostream &operator<<(std::ostream &out, const Tree<T> &t) {
        t.coutArbreRec(0, out);
        return out;
    }

  private:
    Tree<T> *left_;
    Tree<T> *right_;
    Tree<T> *father_;
    T *data_;

    void TreeRec(std::ifstream &fitxerNodes, int h, Tree<T> *father);
    std::ostream &coutArbreRec(int n, std::ostream &out) const;
    void setLeft(Tree<T> *tL);
    void setRight(Tree<T> *tR);
};

template <class T>
Tree<T>::Tree() { // Inicialitzem tota l'estructura
    left_ = NULL;
    right_ = NULL;
    father_ = NULL;
    data_ = NULL;
}

template <class T>
Tree<T>::Tree(const Tree<T> &t) {
    if (t.left_ != NULL) {
        left_ = new (Tree<T>);
        left_ = t.left_;
    } else {
        left_ = NULL;
    }

    if (t.right_ != NULL) {
        right_ = new (Tree<T>);
        right_ = t.right_;
    } else {
        right_ = NULL;
    }
    // m_father: here m_father must be NULL
    // we are creating a tree, if it has to be connected with another one you
    // will use setRight o r left.
    father_ = NULL;

    if (t.data_ != NULL) {
        data_ = new (T);
        data_ = t.data_;
    } else {
        data_ = NULL;
    }
}

/////////////////////////////////////////////////////////////////////////////
// Tree(nomFitxer):                                                        //
//                 constructor que obre fitxer i crida a metode privat     //
// TreeRec(std::ifstream& fitxerNodes, int h, Tree<T>* father)              //
//            metode privat que llegeix arbre de forma recursiva           //
// Lectura d'un fitxer a on tindrem                                         //
// alcada                                                                   //
// estat: 0 o 1 segons sigui buit o amb informacio al costat dada           //
// Esta en preordre Preordre(FillEsq) Arrel Preordre(FillDret)                //
/////////////////////////////////////////////////////////////////////////////
template <class T>
Tree<T>::Tree(std::string nomFitxer) {
    std::ifstream fitxerNodes;

    fitxerNodes.open(nomFitxer.c_str());
    if (fitxerNodes.is_open()) {
        // Llegim al�ada arbre binari
        int h;
        fitxerNodes >> h;
        if (!fitxerNodes.eof()) {
            int estat;
            fitxerNodes >> estat;
            if (estat == 1) {
                if (fitxerNodes.is_open())
                    TreeRec(fitxerNodes, h, NULL);
            }
        }
        fitxerNodes.close();
    }
}

template <class T>
void Tree<T>::TreeRec(std::ifstream &fitxerNodes, int h, Tree<T> *father) {
    father_ = father;
    data_ = new (T);
    fitxerNodes >> (*data_);
    if (h > 0) {
        int estat;
        if (!fitxerNodes.eof()) {
            fitxerNodes >> estat;
            if (!fitxerNodes.eof()) {
                if (estat == 1) {
                    left_ = new (Tree<T>);
                    left_->TreeRec(fitxerNodes, h - 1, this);
                }
            }
        }
        if (!fitxerNodes.eof()) {
            fitxerNodes >> estat;
            if (!fitxerNodes.eof()) {
                if (estat == 1) {
                    right_ = new (Tree<T>);
                    right_->TreeRec(fitxerNodes, h - 1, this);
                }
            }
        }
    }
}

template <class T>
Tree<T>::~Tree() {
    if (right_ != NULL) {
        delete right_;
    }

    if (left_ != NULL) {
        delete left_;
    }

    if (data_ != NULL) {
        delete data_;
    }

    father_ = NULL;
}

// Suposem l'arbre ordenat amb valors menors a arrel a esquerra i valors majors
// a dreta
template <class T>
bool Tree<T>::cerca(const T &val, Tree<T> *valTrobat) {
    bool trobat = false;
    // Implementa
    return trobat;
}

template <class T>
void Tree<T>::mostraExpressio() {
    // Implementa
    Tree<T> *left = left_;
    while (left != nullptr) {
        left = left->left_;
    }
}

template <class T>
bool TryParse(std::string input, T &var) {
    static const std::string ws(" \t\f\v\n\r");
    size_t pos = input.find_last_not_of(ws);
    if (pos != std::string::npos)
        input.erase(pos + 1);
    else
        input.clear();
    std::stringstream buffer(input);
    return buffer >> var && buffer.eof();
}

template <class T>
float Tree<T>::avalua() {
    float resultat = 0;
    // Implementa
    return resultat;
}

/////////////////////////////////////////////////////////////////////////////
// coutArbreRec():                                                         //
//         Metode privat cridat per operator<<                             //
//                       que escriu arbre per pantalla de forma recursiva  //
// |--arrel                                                                //
// |---->FillDret1                                                         //
// |------>FillDret2                                                       //
// |------>FillEsquerre2                                                   //
// |---->FillEsquerre1                                                     //
// alcada                                                                   //
// estat: 0 o 1 segons sigui buit o amb informacio al costat dada           //
// Esta en inordre Inordre(FillEsq) Arrel Inordre(FillDret)                 //
/////////////////////////////////////////////////////////////////////////////
template <class T>
std::ostream &Tree<T>::coutArbreRec(int n, std::ostream &out) const {
    if (isEmpty()) { // Pintem arbre buit
        for (int i = 0; i < n; i++) {
            out << "|--";
        }
        out << "-->BUIT" << '\n';
    } else {
        for (int i = 0; i < n; i++) {
            out << "|--";
        }
        out << "|-->" << (*data_) << '\n';
        if (!isLeave()) {
            if (left_ != NULL) {
                left_->coutArbreRec(n + 1, out);
            } else {
                for (int i = 0; i < n + 1; i++) {
                    out << "|--";
                }
                out << "|-->BUIT" << '\n';
            }
            if (right_ != NULL) {
                right_->coutArbreRec(n + 1, out);
            } else {
                for (int i = 0; i < n + 1; i++) {
                    out << "|--";
                }
                out << "|-->BUIT" << '\n';
            }
        }
    }
    return out;
}

template <class T>
void Tree<T>::setLeft(Tree<T> *tL) {
    left_ = tL;
    if (left_ != NULL) { //Fem que this sigui el pare de left
        left_->father_ = this;
    }
}
template <class T>
void Tree<T>::setRight(Tree<T> *tR) {
    right_ = tR;
    if (right_ != NULL) { //Fem que this sigui el pare de right
        right_->father_ = this;
    }
}