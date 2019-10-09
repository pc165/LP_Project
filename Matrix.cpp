#include "Matrix.h"
#include <iostream>
#include <algorithm>
#include <math.h>

inline bool cmpFloat(const float &e, const float &a) {
	 return(fabs(e - a) < FLT_EPSILON);
}

MatrixCSR::MatrixCSR() {
	 this->init(0, 0);
}


MatrixCSR::MatrixCSR(const int &row, const int &col) {
	 this->init(row, col);
}

MatrixCSR::MatrixCSR(const MatrixCSR &e) {
	 this->copy(e);
}

MatrixCSR::~MatrixCSR() {

}

void MatrixCSR::copy(const MatrixCSR &e) {
	 nRow_ = e.nRow_;
	 nCol_ = e.nCol_;
	 rowIndex_ = e.rowIndex_;
	 columnValors_ = e.columnValors_;
}


void MatrixCSR::init(const int &row, const int &col) {
	 if (row < 0 || col < 0) throw "Error: Les columnes i files han de ser positius\n";

	 nRow_ = row;
	 nCol_ = col;
	 rowIndex_.resize(nRow_ + 1);
}

MatrixCSR MatrixCSR::operator+(const MatrixCSR &e) {
	 return MatrixCSR();
}

MatrixCSR MatrixCSR::operator-(const MatrixCSR &e) {
	 return MatrixCSR();
}

MatrixCSR MatrixCSR::operator*(const MatrixCSR &e) {
	 if (this->nCol_ != e.nRow_) throw "Producte invalid, el numero de files no es igual al de columnes";
	 MatrixCSR res(this->nRow_, e.nCol_);


	 return res;

}

std::vector<float> MatrixCSR::operator*(const std::vector<float> &e) {
	 if (this->nCol_ != e.size()) throw "Producte invalid, el numero de files no es igual al de columnes";

	 std::vector<float> result(this->nRow_, 0);

	 for (int i = 0; i < nRow_; i++) {
		  for (int k = rowIndex_[i]; k < rowIndex_[i + 1]; k++) {
				std::cout << columnValors_[k].second << " * " << e[columnValors_[k].first] << "\n";
				result[i] += columnValors_[k].second * e[columnValors_[k].first];
		  }
		  std::cout << "\n";
	 }
	 return result;
}

MatrixCSR MatrixCSR::operator/(const float &e) {
	 if (cmpFloat(e, 0.0f)) throw "No es pot dividir per zero";

	 MatrixCSR temp(*this);
	 for (int i = 0; i < this->rowIndex_[this->nRow_]; i++)
		  this->columnValors_[i].second /= e;

	 return temp;
}


void MatrixCSR::setValor(const int &row, const int &col, const float &value) {
	 if (row < 0 || col < 0) throw "Error: Els indexs son negatius";
	 if (cmpFloat(value, 0.0f)) return;

	 if (row >= nRow_ || col >= nCol_) {
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
				rowIndex_[row + 1]++;
		  } else
				for (int i = row + 1; i < rowIndex_.size(); i++)
					 rowIndex_[i]++;
		  auto it = std::upper_bound(columnValors_.begin() + rowIndex_[row], columnValors_.begin() + (rowIndex_[row + 1] - 1), col,
											  [](const int &a, const std::pair<int, float> &b) { return a < b.first; });
		  columnValors_.insert(it, std::make_pair(col, value));

	 } else {
		  float val = 0.0f;
		  getValor(row, col, val);
		  if (cmpFloat(val, 0.0f)) {
				for (int i = row + 1; i < nRow_ + 1; i++)
					 rowIndex_[i]++;
				auto it = std::upper_bound(columnValors_.begin() + rowIndex_[row], columnValors_.begin() + (rowIndex_[row + 1] - 1), col,
													[](const int &a, const std::pair<int, float> &b) { return a < b.first; });
				columnValors_.insert(it, std::make_pair(col, value));
		  }
	 }
}

std::ostream &operator<<(std::ostream &a, const MatrixCSR &e) {
	 float colVal = 0;
	 for (int i = 0; i < e.nRow_; i++) {
		  for (int j = 0; j < e.nCol_; j++) {
				e.getValor(i, j, colVal);
				a << colVal << " ";
		  }
		  a << "\n";
	 }

	 a << "\n";
	 a << "Matrix " << e.getNFiles() << "x" << e.getNColumnes() << "\n";
	 a << "[Col, Val]: ";
	 for (auto &i : e.columnValors_) {
		  a << "[" << i.first << "," << i.second << "] ";
	 }
	 a << "\n";
	 a << "IndexPtr: ";
	 for (auto &i : e.rowIndex_) {
		  a << i << " ";
	 }
	 a << "\n\n";

	 return a;
}


bool MatrixCSR::getValor(const int &row, const int &col, float &value) const {
	 if (row < 0 || col < 0) throw "Error: Els indexs son negatius";
	 value = 0.0f;
	 if (row < nRow_ && col < nCol_) {
		  int L = rowIndex_[row], H = rowIndex_[row + 1] - 1;
		  int i = 0;
		  while (L <= H) {
				i = ceil((L + H) / 2);
				if (columnValors_[i].first > col)
					 H = i - 1;
				else if (columnValors_[i].first < col)
					 L = i + 1;
				else {
					 value = columnValors_[i].second;
					 break;
				}
		  }
	 }
	 return  (row < nRow_ && col < nCol_);
}


MatrixCSR &MatrixCSR::operator=(const MatrixCSR &e) {
	 this->copy(e);
	 return *this;
}


void MatrixCSR::setRow(const int &e) {
	 if (e > nRow_ || e < 0) throw "Accio no permesa, utilitzi init per agrandar la matriu";
	 nRow_ = e;
}

void MatrixCSR::setCol(const int &e) {
	 if (e > nCol_ || e < 0) throw "Accio no permesa, utilitzi init per agrandar la matriu";
	 nCol_ = e;
}

void MatrixCSR::setRowCol(const int &a, const int &e) {
	 if (e > nCol_ || e < 0 || a>nRow_ || a < 0) throw "Accio no permesa, utilitzi init per agrandar la matriu";
	 nRow_ = a;
	 nCol_ = e;
}
