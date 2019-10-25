#include "MatriuSparse.h"
#include <iostream>
#include <math.h>
#include <fstream>
#include <tuple>
MatriuSparse::MatriuSparse(const std::string &e) {
	std::fstream f(e);
	if (f.is_open()) {
		//int row, col;
		//f >> row >> col;
		//init(row, col);
		int x = 0, y = 0;
		while (!f.eof()) {
			f >> x >> y;
			setVal(x, y, 1);
		}
		f.close();
	}
}

inline bool cmpFloat(const float &e, const float &a) {
	return(fabs(e - a) < FLT_EPSILON);
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


void MatriuSparse::init(const int &row, const int &col) {
	if (row < 0 || col < 0) throw "Error: Les columnes i files han de ser positius\n";
	nRow_ = row;
	nCol_ = col;
	rowIndex_.resize(nRow_ + 1);
}

MatriuSparse MatriuSparse::operator+(const MatriuSparse &e) {
	return MatriuSparse();
}

MatriuSparse MatriuSparse::operator-(const MatriuSparse &e) {
	return MatriuSparse();
}

MatriuSparse MatriuSparse::operator*(const MatriuSparse &e) {
	if (this->nCol_ != e.nRow_) throw "Producte invalid, el numero de files no es igual al de columnes";
	MatriuSparse res(this->nRow_, e.nCol_);
	float suma = 0;
	int index = -1;
	for (int i = 0; i < this->nRow_; i++) {
		for (int j = 0; j < e.nCol_; j++) {
			for (int z = 0; z < nCol_; z++) {
				suma += getVal(i, z) * e.getVal(z, j);
			}
			res.setVal(i, j, suma);
			suma = 0;
		}
	}
	return res;

}

MatriuSparse MatriuSparse::operator*(const float &e) {
	MatriuSparse res(*this);
	if (cmpFloat(e, 0)) {
		res.columnValors_.resize(0);
		for (auto &i : res.rowIndex_)
			i = 0;
	} else {
		for (auto &i : res.columnValors_)
			i.second *= e;
	}
	return res;
}

std::vector<float> MatriuSparse::operator*(const std::vector<float> &e) {
	if (this->nCol_ != e.size()) throw "Producte invalid, el numero de files no es igual al de columnes";

	std::vector<float> result(this->nRow_, 0);

	for (int i = 0; i < nRow_; i++) {
		for (int k = rowIndex_[i]; k < rowIndex_[i + 1]; k++) {
			//std::cout << columnValors_[k].second << " * " << e[columnValors_[k].first] << "\n";
			result[i] += columnValors_[k].second * e[columnValors_[k].first];
		}
	}
	return result;
}

MatriuSparse MatriuSparse::operator/(const float &e) {
	if (cmpFloat(e, 0.0f)) throw "No es pot dividir per zero";

	MatriuSparse res(*this);
	for (auto &i : res.columnValors_)
		i.second /= e;

	return res;
}


void MatriuSparse::setVal(const int &row, const int &col, const float &value) {
	if (row < 0 || col < 0) throw "Error: Els indexs son negatius";

	if (cmpFloat(value, 0.0f) && row < nRow_ && col < nCol_) { //if value == 0, delete it from matrix
		int i = binarySearch(row, col);
		if (i >= 0)
			columnValors_.erase(columnValors_.begin() + i);
		if (row + 2 != nRow_) {
			for (int i = row + 1; i < rowIndex_.size(); i++)
				rowIndex_[i]--;
		} else {
			rowIndex_[row]--;
			rowIndex_[row + 1]--;
			rowIndex_.pop_back();
		}
		return;
	}

	if (!(row < nRow_ && col < nCol_)) {
		if (col >= nCol_)
			nCol_ = col + 1;
		int oldRow = nRow_;
		if (row >= nRow_) {
			nRow_ = row + 1;
			rowIndex_.resize(row + 2);
		}
		if (row == rowIndex_.size() - 2) {
			for (int i = oldRow + 1; i < row + 2; i++)
				rowIndex_[i] = rowIndex_[i - 1];
			rowIndex_.back()++;
		} else {
			for (int i = row + 1; i < rowIndex_.size(); i++)
				rowIndex_[i]++;
		}
	} else {
		int z = binarySearch(row, col);
		if (z >= 0) {
			columnValors_[z].second = value;
			return;
		}

		for (int i = row + 1; i < rowIndex_.size(); i++)
			rowIndex_[i]++;
	}

	int low = rowIndex_[row], high = rowIndex_[row + 1] - 1, mid = 0;
	while (low < high) {
		mid = (low + high) / 2;
		if (!(col < columnValors_[mid].first))
			low = ++mid;
		else
			high = mid;
	}
	columnValors_.insert(columnValors_.begin() + low, std::make_pair(col, value));
}

template<typename T>
T &format(T &a, const MatriuSparse &e) {
	a << "MATRIU DE FILES: " << e.getNFiles() << " : COLUMNES: " << e.getNColumnes() << "\n";
	for (int i = 0; i < e.nRow_; i++) {
		if (e.rowIndex_[i] == e.rowIndex_[i + 1])
			i = e.searchFirstNotEqual(i, e.nRow_, e.rowIndex_) - 1;

		a << "VALORS FILA:" << i << "(COL:VALOR)\n";
		for (int j = e.rowIndex_[i]; j < e.rowIndex_[i + 1]; j++)
			a << "(" << e.columnValors_[j].first << " : " << e.columnValors_[j].second << ") ";
		a << "\n";

	}
	a << "MATRIUS\nVALORS\n(";
	for (auto &i : e.columnValors_)
		a << i.second << "  ";
	a << ")\nCOLS\n(";
	for (auto &i : e.columnValors_)
		a << i.first << "  ";
	a << ")\nINIFILA\n(";
	for (int i = 0; i < e.nRow_; i++) {
		if (e.rowIndex_[i] == e.rowIndex_[i + 1])
			i = e.searchFirstNotEqual(i, e.nRow_, e.rowIndex_);
		a << "[ " << i << " : " << e.rowIndex_[i] << " ] ";
	}
	a << " [Num Elems:" << e.rowIndex_[e.nRow_] << "] )\n";
	return a;
}

std::ostream &operator<<(std::ostream &a, const MatriuSparse &e) {
	return format<std::ostream>(a, e);
}

std::ofstream &operator<<(std::ofstream &a, const MatriuSparse &e) {
	return format<std::ofstream>(a, e);
}
bool MatriuSparse::getVal(const int &row, const int &col, float &value) const {
	if (row < 0 || col < 0) throw "Error: Els indexs son negatius";
	value = 0.0f;
	int i = binarySearch(row, col);
	if (i != -1)
		value = columnValors_[i].second;
	return  (row < getNFiles() && col < nCol_);
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


void MatriuSparse::setRow(const int &e) {
	if (e > nRow_ || e < 0) throw "Accio no permesa, utilitzi init per agrandar la matriu";
	nRow_ = e;
}

void MatriuSparse::setCol(const int &e) {
	if (e > nCol_ || e < 0) throw "Accio no permesa, utilitzi init per agrandar la matriu";
	nCol_ = e;
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

template<typename T>
int MatriuSparse::searchFirstNotEqual(const int &min, const int &max, T vector) const {
	int low = min, high = max, mid = 0;
	while (low < high) {
		mid = (low + high) / 2;
		if (vector[low] != vector[mid])
			high = --mid;
		else if (vector[low] == vector[mid])
			low = ++mid;
	}
	return low;
}