#pragma once
#include <vector>
#include <iostream>
#include <string>

class MatrixCSR {
public:
    MatrixCSR();
    MatrixCSR(const int &row, const int &col);
    MatrixCSR(const MatrixCSR &e);
    MatrixCSR(const std::string & e, const int & row, const int & col);
    ~MatrixCSR();
    void setValor(const int &row, const int &col, const float &value);
    bool getValor(const int &row, const int &col, float &value) const;
    float getValor(const int &row, const int &col) const;
    void init(const int &row, const int &col);
    friend std::ostream &operator<<(std::ostream &a, const MatrixCSR &e);
    MatrixCSR operator+(const MatrixCSR &e);
    MatrixCSR operator-(const MatrixCSR &e);
    MatrixCSR operator*(const MatrixCSR &e);
    std::vector<float> operator*(const std::vector<float> &e);
    MatrixCSR operator/(const float &e);
    MatrixCSR &operator=(const MatrixCSR &e);

    void setRow(const int &e);
    void setCol(const int &e);
    void setRowCol(const int &e, const int &a);
    inline int getNFiles()const { return nRow_; }
    inline int getNColumnes()const { return nCol_; }
private:
    void copy(const MatrixCSR &e);
    int binarySearch(const int &row, const int &col) const;

private:
    std::vector<int> rowIndex_;
    std::vector<std::pair<int, float>> columnValors_;
    int nRow_, nCol_;

};