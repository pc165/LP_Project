#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "Tree.hpp"
using namespace std;

class MatriuSparse {
public:
    MatriuSparse();
    MatriuSparse(const int &row, const int &col);
    MatriuSparse(const MatriuSparse &e);
    MatriuSparse(const std::string &e);
    ~MatriuSparse();
    void setVal(const int &row, const int &col, const float &value);
    bool getVal(const int &row, const int &col, float &value) const;
    float getVal(const int &row, const int &col) const;
    void init(const int &row, const int &col);
    friend std::ostream &operator<<(std::ostream &a, const MatriuSparse &e);
    friend std::ofstream &operator<<(std::ofstream &a, const MatriuSparse &e);
    //MatriuSparse operator*(const MatriuSparse &e);
    MatriuSparse operator*(const float &e);
    std::vector<float> operator*(const std::vector<float> &e);
    MatriuSparse operator/(const float &e);
    MatriuSparse &operator=(const MatriuSparse &e);

    void setRowCol(const int &e, const int &a);
    inline int getNFiles()const { return nCol_ > nRow_ ? nCol_ : nRow_; } // square matrix, return the biggets number
    inline int getNColumnes()const { return nCol_ > nRow_ ? nCol_ : nRow_; }

    int getNValues()const;
    void calculaGrau(std::vector<int> &)const;
    void calculaDendograms(std::vector<Tree<double> *> &)const;
    void clear();
private:
    void insertValue(const int &row, const int &col, const int &value);
    void removeValue(const int &row, const int &col);
    template<typename T>
    friend T &format(T &a, const MatriuSparse &e);
    void copy(const MatriuSparse &e);
    int numberOfValuesInColumn(const int &col);
    int binarySearch(const int &row, const int &col) const;
    template<typename T, typename CMP>
    int searchFirstGreater(const int &min, const int &max, const int &val, T vector, CMP cmp) const;

private:
    std::vector<int> rowIndex_;
    std::vector<std::pair<int, float>> columnValors_;
    int nRow_, nCol_;

};