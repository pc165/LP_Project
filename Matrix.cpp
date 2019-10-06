#include "Matrix.h"
#include <iostream>
#include <algorithm>



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
	 //for (int i = 0; i < nRow_ + 1; i++)
	 //	rowIndex_[i] = 0;
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
	 float sum = 0;

	 //for (int z = 0; z < nRow_; z++) {
	 //	for (int k = 0; k < nCol_; k++) {
	 //		for (int i = 0; i < nRow_; i++) {
	 //			for (int j = rowIndex_[i]; j < rowIndex_[i + 1]; j++) {
	 //				sum += columnValors_[j].data * e.columnValors_[columnValors_[j].col].data;
	 //			}
	 //		}
	 //		res.setValor(z, k, sum);
	 //		sum = 0;
	 //	}
	 //}

	 return res;
}

std::vector<float> MatrixCSR::operator*(const std::vector<float> &e) {
	 if (this->nCol_ != e.size()) throw "Producte invalid, el numero de files no es igual al de columnes";

	 std::vector<float> result(this->nRow_, 0);

	 for (int i = 0; i < nRow_; i++) {
		  for (int k = rowIndex_[i]; k < rowIndex_[i + 1]; k++) {
				std::cout << columnValors_[k].data << " * " << e[columnValors_[k].col] << "\n";
				result[i] += columnValors_[k].data * e[columnValors_[k].col];
		  }
		  std::cout << "\n";
	 }
	 return result;
}

MatrixCSR MatrixCSR::operator/(const float &e) {
	 if (fabs(e) < FLT_EPSILON) throw "No es pot dividir per zero";

	 MatrixCSR temp(*this);
	 for (int i = 0; i < this->rowIndex_[this->nRow_]; i++)
		  this->columnValors_[i].data /= e;


	 return temp;
}


void MatrixCSR::setValor(const int &row, const int &col, const float &value) {
	 if (row < 0 || col < 0) throw "Error: Els indexs son negatius";
	 if (fabs(value - 0.0f) < FLT_EPSILON) return;


	 if (!(row < nRow_ && col < nCol_)) {
		  int oldRow = nRow_;
		  nCol_ = col + 1;
		  if (!(row < nRow_)) {
				nRow_ = row + 1;
				rowIndex_.resize(row + 2);
		  }

		  if (row == rowIndex_.size() - 2) { // si es l'ultima fila
				for (int i = oldRow + 1; i < row + 2; i++)
					 rowIndex_[i] = rowIndex_[i - 1];
				rowIndex_[row + 1]++;
		  } else {
				for (int i = row + 1; i < rowIndex_.size(); i++)
					 rowIndex_[i]++;
		  }

		  columnValors_.insert(columnValors_.begin() + rowIndex_[row], { col,value });
		  std::sort(columnValors_.begin() + rowIndex_[row], columnValors_.begin() + rowIndex_[row + 1]);

	 } else {
		  bool found = false;
		  int i = rowIndex_[row];
		  while (i < rowIndex_[row + 1] && !found) {
				if (columnValors_[i].col == col) {
					 columnValors_[i].data = value;
					 found = true;
				}
				i++;
		  }
		  if (!found) {
				columnValors_.insert(columnValors_.begin() + rowIndex_[row], { col,value });
				std::sort(columnValors_.begin() + rowIndex_[row], columnValors_.begin() + rowIndex_[row + 1]);
				for (int i = row + 1; i < nRow_ + 1; i++)
					 rowIndex_[i]++;

		  }
	 }
}


std::ostream &operator<<(std::ostream &a, const MatrixCSR &e) {
	 float colVal = 0;
	 for (int i = 0; i < e.nRow_; i++) {
		  for (int j = 0; j < e.nCol_; j++) {
				e.getValor(i, j, colVal);
				if (colVal != 0)
					 a << colVal;
				else
					 a << " ";
				a << " ";
		  }
		  a << "\n";
	 }

	 a << "Matrix " << e.getNFiles() << "x" << e.getNColumnes() << "\n";
	 for (auto &i : e.columnValors_) {
		  a << "[" << i.col << "," << i.data << "] ";
	 }
	 a << "\n";

	 for (auto &i : e.rowIndex_) {
		  a << i << " ";
	 }
	 a << "\n";

	 return a;
}


bool MatrixCSR::getValor(const int &row, const int &col, float &value) const {
	 if (row < 0 || col < 0) throw "Error: Els indexs son negatius";

	 value = 0.0f;
	 if (row < nRow_ && col < nCol_)
		  for (int i = rowIndex_[row]; i < rowIndex_[row + 1]; i++)
				if (columnValors_[i].col == col) {
					 value = columnValors_[i].data;
					 break;
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
bool operator<(const colVal &a, const colVal &b) {
	 return a.col < b.col;
}
