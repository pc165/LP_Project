#include "MatriuSparse.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <math.h>
#include <set>
#include <string>
#include <vector>
#define DOUBLE(x) (static_cast<double>(x))

/*
File constructor: Efficiently constructs the object from a file.
Parses a file containing points (row column) ordered by row and column. 
Sets the value of each element to 1.
*/
MatriuSparse::MatriuSparse(const std::string &e) : nRow_(0), nCol_(0) {
    std::fstream f(e);
    if (f.is_open()) {
        //Count how many values there are in the file for preallocating the vectors columnValors_ and rowIndex_
        int count = 0;
        std::string line, lastLine;
        while (std::getline(f, line)) {
            if (line.size() > 1) {
                lastLine = line;
                count++;
            }
        }
        columnValors_.resize(count, std::pair<int, float>(0, 1.0));

        lastLine = lastLine.substr(0, lastLine.find('\t'));
        nRow_ = std::stoi(lastLine) + 1;
        rowIndex_.resize(nRow_ + 1);
        rowIndex_.back() = count;

        f.clear();
        f.seekg(0, f.beg);
        int newX = 0, oldX = 0, y = 0;
        count = 0;
        while (!f.eof() && count < columnValors_.size()) {
            f >> newX >> y;
            columnValors_[count].first = y;
            if (y > nCol_)
                nCol_ = y;
            if (newX != oldX) {
                for (int i = oldX; i < newX; i++)
                    rowIndex_[i + 1] = count;
                oldX = newX;
            }
            count++;
        }
        nCol_++;
        f.close();
    }
}

inline bool cmpFloat(const float &e, const float &a) {
    return (fabs(e - a) < std::numeric_limits<float>::epsilon());
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
    for (size_t i = 0; i < nRow_; i++)
        for (size_t j = rowIndex_[i]; j < rowIndex_[i + 1]; j++)
            if (columnValors_[j].first == col)
                sum++;
    return sum;
}

void MatriuSparse::init(const int &row, const int &col) {
    if (row < 0 || col < 0)
        throw "Error: Les columnes i files han de ser positius\n";
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
    if (this->getNColumnes() != e.size())
        throw "Producte invalid, el numero de files no es igual al de columnes";

    std::vector<float> result(nRow_, 0);

    for (size_t i = 0; i < nRow_; i++)
        if (rowIndex_[i] != rowIndex_[i + 1])
            for (int k = rowIndex_[i]; k < rowIndex_[i + 1]; k++)
                result[i] += columnValors_[k].second * e[columnValors_[k].first];
    return result;
}

MatriuSparse MatriuSparse::operator/(const float &e) {
    if (cmpFloat(e, 0.0f))
        throw "No es pot dividir per zero";

    MatriuSparse res(*this);
    for (auto &i : res.columnValors_)
        i.second /= e;

    return res;
}
/*
Busca els veins dels arguments i els retorna en una parella de sets
first = set de veins del node1
second = set de veins del node2
*/
std::pair<std::set<int>, std::set<int>> MatriuSparse::getVeins(const int &node1, const int &node2) const {
    std::set<int> a, b;
    for (size_t k = rowIndex_[node1]; k < rowIndex_[node1 + 1]; k++) { //buscar veins del node1
        a.insert(columnValors_[k].first);
    }
    for (size_t k = rowIndex_[node2]; k < rowIndex_[node2 + 1]; k++) { //buscar veins del node2
        b.insert(columnValors_[k].first);
    }
    return std::make_pair(a, b);
}

void MatriuSparse::calculaGrau(std::vector<int> &graus) const {
    graus.resize(rowIndex_.size() - 1);
    for (size_t i = 0; i < rowIndex_.size() - 1; i++)
        graus[i] = rowIndex_[i + 1] - rowIndex_[i];
}

void MatriuSparse::calculaDendograms(std::vector<Tree<double> *> &vDendrogrames) const {
    for (int i = 0; i < rowIndex_.size(); i++) {
        Tree<double> *j = new Tree<double>();
        vDendrogrames.push_back(j);
    }
}

typedef std::pair<std::pair<int, int>, double> mapIter;
//typedef std::pair<int, int> make_pair;

void MatriuSparse::creaMaps(std::vector<std::map<pair<int, int>, double>> &vMaps) const {
    vMaps.resize(getNFiles());
    for (size_t i = 0; i < nRow_; i++) {
        for (size_t k = rowIndex_[i]; k < rowIndex_[i + 1]; k++) {
            if (i != columnValors_[k].first)
                vMaps[i].emplace(mapIter(make_pair(i, columnValors_[k].first), 0));
        }
    }
}

/*
Private.
Inserts the value to the given position in the matrix. 
If the row or column is outisde the matrix, it will create a new row or column filled with zeros,
effectively making the matrix bigger.

0 0
0 1

After inserting the value (3,3,1)

0 0 0 0
0 1 0 0
0 0 0 0
0 0 0 1

*/
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
            for (size_t i = oldRow + 1; i < rowIndex_.size(); i++)
                rowIndex_[i] = rowIndex_[i - 1];
            rowIndex_.back()++;
        } else {
            for (size_t i = row + 1; i < rowIndex_.size(); i++)
                rowIndex_[i]++;
        }
    } else {
        int z = binarySearch(row, col);
        if (z >= 0) {
            columnValors_[z].second = value;
            return;
        } else
            for (size_t i = row + 1; i < rowIndex_.size(); i++)
                rowIndex_[i]++;
    }
    int i = searchFirstGreater(rowIndex_[row], rowIndex_[row + 1] - 1, col, columnValors_, comparePair);
    columnValors_.insert(columnValors_.begin() + i, std::make_pair(col, value));
}

/*
Private.
Removes the selected value from the matrix. 
If the removed value was the last, the rows and/or columns are removed from the matrix.

0 0 0 0
0 1 0 0
0 0 0 0
0 0 0 1

After removing the value (3,3)

0 0
0 1

*/
void MatriuSparse::removeValue(const int &row, const int &col) {
    int i = binarySearch(row, col);
    if (i >= 0) {
        columnValors_.erase(columnValors_.begin() + i);
        if (row + 1 != nRow_) {
            for (size_t z = row + 1; z < rowIndex_.size(); z++)
                rowIndex_[z]--;
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
                for (size_t z = 0; z < nRow_; z++) {
                    if (columnValors_[rowIndex_[z + 1] - 1].first > maxCol)
                        maxCol = columnValors_[rowIndex_[z + 1] - 1].first;
                }
                nCol_ = maxCol + 1;
            }
        } else {
            nCol_ = 0;
        }
    }
    return;
}

/*
Public wrapper for inserting or removing values.
*/
void MatriuSparse::setVal(const int &row, const int &col, const float &value) {
    if (row < 0 || col < 0)
        throw "Error: Els indexs son negatius";

    if (cmpFloat(value, 0.0f)) {
        if (!(row < nRow_ && col < nCol_))
            throw "No es pot eliminar un valor fora de la matriu";
        removeValue(row, col);
    } else {
        insertValue(row, col, value);
    }
}

template <typename T>
T &MatriuSparse::format(T &a, const MatriuSparse &e) const {
    a << "MATRIU DE FILES: " << e.getNFiles() << " : COLUMNES: " << e.getNColumnes() << "\n";
    for (size_t i = 0; i < e.nRow_; i++) {
        if (e.rowIndex_[i] != e.rowIndex_[i + 1]) {
            a << "VALORS FILA:" << i << "(COL:VALOR)\n";
            for (size_t j = e.rowIndex_[i]; j < e.rowIndex_[i + 1]; j++)
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
    a << " [Num Elems:" << e.rowIndex_[e.nRow_] << "] )\n"
      << std::flush;
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
    return a;
    //return e.format<std::ostream>(a, e);
}

std::ofstream &operator<<(std::ofstream &a, const MatriuSparse &e) {
    return e.format<std::ofstream>(a, e);
}

/*
Returns 1 if the row and column is inside the matrix and the value of the given row and column.
*/
bool MatriuSparse::getVal(const int &row, const int &col, float &value) const {
    if (row < 0 || col < 0)
        throw "Error: Els indexs son negatius";
    value = 0.0f;
    int i = binarySearch(row, col);
    if (i != -1)
        value = columnValors_[i].second;
    return (row < getNFiles() && col < getNColumnes());
}

/*
Returns the value.
Throws a exception if the arguments are invalid.
*/
float MatriuSparse::getVal(const int &row, const int &col) const {
    if (row < 0 || col < 0 || !((row < nRow_ && col < nCol_)))
        throw "Error: Seleccio invalida";
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
    if (e > nCol_ || e < 0 || a > nRow_ || a < 0)
        throw "Accio no permesa, utilitzi init per agrandar la matriu";
    nRow_ = a;
    nCol_ = e;
}

/*
Private.
Returns the index for the value in columnValors_. 
Returns -1 if the row and column is outside of the matrix or the value is 0.
*/
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

/*
Return the position between fisrt and last of the first element in the vector that is greater than the argument val.
*/
template <typename T, typename CMP>
int MatriuSparse::searchFirstGreater(const int &first, const int &last, const int &val, std::vector<T> vector, CMP cmp) const {
    int low = first, high = last, mid = 0;
    while (low < high) {
        mid = (low + high) / 2;
        if (cmp(val, vector[mid]))
            low = ++mid;
        else
            high = mid;
    }
    return low;
}
