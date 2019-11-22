#include "MatriuSparse.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#define FLT_EPSILON 1.192092896e-07F // smallest such that 1.0+FLT_EPSILON != 1.0, this is needed to compare floats.

MatriuSparse::MatriuSparse(const std::string &e):nRow_(0), nCol_(0) {
    std::fstream f(e);
    if (f.is_open()) {
        int count = 0;
        std::string line, lastLine;
        while (std::getline(f, line)) // count the number of non-empty (more than one character) lines from the file
            if (line.size() > 1) { // if there are more than one character
                lastLine = line; // save last non-empty line
                count++;
            }
        lastLine = lastLine.substr(0, lastLine.find('\t')); // find X coordinate, the data is sperarated by a tab (\t)
        columnValors_.resize(count, std::pair<int, float>(0, 1)); // resize to the number of 

        nRow_ = std::stoi(lastLine) + 1; // stoi = String TO Integer, add 1 because the file starts to count from 0.
        rowIndex_.resize(nRow_ + 1); // we need an extra space to store the number of values in the matrix
        rowIndex_.back() = count; // rowIndex_.back() == columnValors_.size()

        f.clear(); // clear flags (end flag)
        f.seekg(0, f.beg); // return to the beginning of the file
        int newX = 0, oldX = 0, y = 0, countIndex = 0;
        count = 0;
        while (!f.eof() && count < columnValors_.size()) {
            f >> newX >> y;
            columnValors_[count].first = y;
            if (y > nCol_) // set the number of columns to the biggest column
                nCol_ = y;
            if (newX != oldX) { // if we changed of row then update the values from oldX to the new X
                for (int i = oldX; i < newX; i++)
                    rowIndex_[i + 1] = count; //rowindex_[i] - rowindex[i+1] = number of elements in the i row
                oldX = newX;
            }
            count++;
        }
        nCol_++; // add one because the file started to count the columns from 0
        f.close();
    }
}

inline bool cmpFloat(const float &e, const float &a) { // we need this function to compare floats, fabs = float aboslute value
    return(fabs(e - a) < FLT_EPSILON);
}

bool compareInt(const int &a, const int &b) {
    return !(a < b);
}

bool comparePair(const int &a, std::pair<int, float> b) {
    return !(a < b.first);
}

MatriuSparse::MatriuSparse() {
    this->init(0, 0);
}

MatriuSparse::MatriuSparse(const int &row, const int &col) {
    this->init(row, col);
}

MatriuSparse::MatriuSparse(const MatriuSparse &e) {
    this->copy(e);
}

MatriuSparse::~MatriuSparse() {

}

void MatriuSparse::copy(const MatriuSparse &e) {
    nRow_ = e.nRow_;
    nCol_ = e.nCol_;
    rowIndex_ = e.rowIndex_;
    columnValors_ = e.columnValors_;
}

int MatriuSparse::numberOfValuesInColumn(const int &col) {
    int sum = 0;
    for (int i = 0; i < nRow_; i++)
        for (int j = rowIndex_[i]; j < rowIndex_[i + 1]; j++)
            if (columnValors_[j].first == col)
                sum++;
    return sum;
}


void MatriuSparse::init(const int &row, const int &col) {
    if (row < 0 || col < 0) throw "Error: Les columnes i files han de ser positius\n";
    nRow_ = row;
    nCol_ = col;
    rowIndex_.resize(nRow_ + 1);
}


MatriuSparse MatriuSparse::operator*(const float &e) {
    MatriuSparse res(*this);
    if (cmpFloat(e, 0)) {
        init(0, 0);
    } else {
        for (auto &i : res.columnValors_)
            i.second *= e;
    }
    return res;
}

std::vector<float> MatriuSparse::operator*(const std::vector<float> &e) {
    if (this->getNColumnes() != e.size()) throw "Producte invalid, el numero de files no es igual al de columnes";

    std::vector<float> result(nRow_, 0);

    for (int i = 0; i < nRow_; i++)
        if (rowIndex_[i] != rowIndex_[i + 1])
            for (int k = rowIndex_[i]; k < rowIndex_[i + 1]; k++)
                result[i] += columnValors_[k].second * e[columnValors_[k].first];
    return result;
}

MatriuSparse MatriuSparse::operator/(const float &e) {
    if (cmpFloat(e, 0.0f)) throw "No es pot dividir per zero";

    MatriuSparse res(*this);
    for (auto &i : res.columnValors_)
        i.second /= e;

    return res;
}

void MatriuSparse::calculaGrau(std::vector<int> &graus) const {
    graus.resize(rowIndex_.size());
    for (int i = 0; i < rowIndex_.size(); i++)
        graus[i] = rowIndex_[i + 1] - rowIndex_[i];
}


void MatriuSparse::insertValue(const int &row, const int &col, const int &value) {
    if (!(row < nRow_ && col < nCol_)) {
        if (col >= nCol_)
            nCol_ = col + 1;
        int oldRow = nRow_;
        if (row >= nRow_) {
            nRow_ = row + 1;
            rowIndex_.resize(row + 2);
        }
        if (row + 1 == nRow_) {
            for (int i = oldRow + 1; i < rowIndex_.size(); i++)
                rowIndex_[i] = rowIndex_[i - 1];
            rowIndex_.back()++;
        } else {
            for (int i = row + 1; i < rowIndex_.size(); i++)
                rowIndex_[i]++;
        }
    } else {
        int z = binarySearch(row, col); // if we found the value, we just need to change the value in the array
        if (z >= 0) {
            columnValors_[z].second = value;
            return;
        } else // make space for the new value
            for (int i = row + 1; i < rowIndex_.size(); i++)
                rowIndex_[i]++;
    }
    //insert value
    int low = searchFirstGreater(rowIndex_[row], rowIndex_[row + 1] - 1, col, columnValors_, comparePair);
    columnValors_.insert(columnValors_.begin() + low, std::make_pair(col, value));
}


void MatriuSparse::removeValue(const int &row, const int &col) {
    int i = binarySearch(row, col);
    if (i >= 0) {
        columnValors_.erase(columnValors_.begin() + i);
        if (row + 1 != nRow_) {
            for (int i = row + 1; i < rowIndex_.size(); i++)
                rowIndex_[i]--;
        } else {
            rowIndex_[row + 1]--;
            if (rowIndex_[row] - rowIndex_[row + 1] == 0) {
                rowIndex_.pop_back();
                nRow_--;
            }
        }
        if (columnValors_.size() > 0) {
            if (col + 1 == nCol_ && numberOfValuesInColumn(col) == 0) {
                int maxCol = columnValors_[rowIndex_[1] - 1].first;
                for (int i = 0; i < nRow_; i++) {
                    if (columnValors_[rowIndex_[i + 1] - 1].first > maxCol)
                        maxCol = columnValors_[rowIndex_[i + 1] - 1].first;
                }
                nCol_ = maxCol + 1;
            }
        } else {
            nCol_ = 0;
        }
    }
    return;
}


void MatriuSparse::setVal(const int &row, const int &col, const float &value) {
    if (row < 0 || col < 0) throw "Error: Els indexs son negatius";

    if (cmpFloat(value, 0.0f)) {
        if (row < nRow_ && col < nCol_)
            removeValue(row, col);
    } else {
        insertValue(row, col, value);
    }
}

template<typename T>
T &MatriuSparse::format(T &a, const MatriuSparse &e) const {
    a << "MATRIU DE FILES: " << e.getNFiles() << " : COLUMNES: " << e.getNColumnes() << "\n";
    for (int i = 0; i < e.nRow_; i++) {
        if (e.rowIndex_[i] != e.rowIndex_[i + 1]) {
            a << "VALORS FILA:" << i << "(COL:VALOR)\n";
            for (int j = e.rowIndex_[i]; j < e.rowIndex_[i + 1]; j++)
                a << "(" << e.columnValors_[j].first << " : " << e.columnValors_[j].second << ") ";
            a << "\n";
        }
    }
    a << "MATRIUS\nVALORS\n(";
    for (auto &i : e.columnValors_)
        a << i.second << "  ";
    a << ")\nCOLS\n(";
    for (auto &i : e.columnValors_)
        a << i.first << "  ";
    a << ")\nINIFILA\n(" << std::flush;
    for (int i = 0; i < e.nRow_; i++) {
        if (e.rowIndex_[i] != e.rowIndex_[i + 1])
            a << "[ " << i << " : " << e.rowIndex_[i] << " ] ";
    }
    a << " [Num Elems:" << e.rowIndex_[e.nRow_] << "] )\n" << std::flush;
    return a;
}

std::ostream &operator<<(std::ostream &a, const MatriuSparse &e) {
    //float colVal = 0;
    //for (int i = 0; i < e.nRow_; i++) {
    //    for (int j = 0; j < e.nCol_; j++) {
    //        e.getVal(i, j, colVal);
    //        a << colVal << " ";
    //    }
    //    a << "\n";
    //}
    //return a;
    return e.format<std::ostream>(a, e);
}

std::ofstream &operator<<(std::ofstream &a, const MatriuSparse &e) {
    return e.format<std::ofstream>(a, e);
}
bool MatriuSparse::getVal(const int &row, const int &col, float &value) const {
    if (row < 0 || col < 0) throw "Error: Els indexs son negatius";
    value = 0.0f;
    int i = binarySearch(row, col);
    if (i != -1)
        value = columnValors_[i].second;
    return  (row < getNFiles() && col < getNColumnes());
}

float MatriuSparse::getVal(const int &row, const int &col) const {
    if (row < 0 || col < 0 || !((row < nRow_ && col < nCol_))) throw "Error: Seleccio invalida";
    int i = binarySearch(row, col);
    if (i != -1)
        return columnValors_[i].second;
    else
        return 0;
}


MatriuSparse &MatriuSparse::operator=(const MatriuSparse &e) {
    this->copy(e);
    return *this;
}



void MatriuSparse::setRowCol(const int &a, const int &e) {
    if (e > nCol_ || e < 0 || a>nRow_ || a < 0) throw "Accio no permesa, utilitzi init per agrandar la matriu";
    nRow_ = a;
    nCol_ = e;
}

int MatriuSparse::binarySearch(const int &row, const int &col) const {
    if (row < nRow_ && col < nCol_) {
        int L = rowIndex_[row], H = rowIndex_[row + 1] - 1;
        int i = 0;
        while (L <= H) {
            i = ceil((L + H) / 2);
            if (columnValors_[i].first > col)
                H = --i;
            else if (columnValors_[i].first < col)
                L = ++i;
            else
                return i;
        }
    }
    return -1;
}

template<typename T, typename CMP>
int MatriuSparse::searchFirstGreater(const int &min, const int &max, const int &val, T vector, CMP cmp) const {
    int low = min, high = max, mid = 0;
    while (low < high) {
        mid = (low + high) / 2;
        if (cmp(val, vector[mid]))
            low = ++mid;
        else
            high = mid;
    }
    return low;
}

