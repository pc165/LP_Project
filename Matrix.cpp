#include "Matrix.h"
#include <iostream>

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
	if (col_ != nullptr) delete[]col_;
	if (value_ != nullptr) delete[]value_;
	if (rowIndex_ != nullptr) delete[]rowIndex_;
}

void MatrixCSR::copy(const MatrixCSR &e) {
	nRow_ = e.nRow_;
	nCol_ = e.nCol_;
	rowIndex_ = new int[e.nRow_ + 1];
	value_ = new float[e.rowIndex_[e.nRow_]];
	col_ = new int[e.rowIndex_[e.nRow_]];

	for (int i = 0; i < e.rowIndex_[e.nRow_]; i++) {
		col_[i] = e.col_[i];
		value_[i] = e.value_[i];
	}
	for (int i = 0; i < nRow_ + 1; i++)
		rowIndex_[i] = e.rowIndex_[i];
}


void MatrixCSR::init(const int &row, const int &col) {
	if (row < 0 || col < 0) throw "Error: Les columnes i files han de ser positius\n";

	nRow_ = row; nCol_ = col;
	rowIndex_ = new int[nRow_ + 1];
	value_ = new float[0];
	col_ = new int[0];
	for (int i = 0; i < nRow_ + 1; i++)
		rowIndex_[i] = 0;
}


void MatrixCSR::setValor(const int &row, const int &col, const float &value) {
	if (row < 0 || col < 0) throw "Error: Les columnes i files han de ser positius\n";

	if (row > nRow_) resizeRow(row);
	if (col > nCol_) resizeCol(col);
	//set valor...
}


void MatrixCSR::resizeRow(const int &row) {
	int *tempRowIndex = rowIndex_;
	rowIndex_ = new int[row + 1];
	rowIndex_[row] = tempRowIndex[nRow_];

	for (int i = 0; i < (row > nRow_ ? row : nRow_); i++)
		rowIndex_[i] = tempRowIndex[i > nRow_ ? nRow_ : i];

	nRow_ = row;

	delete[]tempRowIndex;
}

void MatrixCSR::resizeCol(const int &col) {
	int totalValue = rowIndex_[nRow_];

	float *tempVal = value_;
	int *tempCol = col_;

	col_ = new int[totalValue];
	value_ = new float[totalValue];

	for (int i = 0; i < totalValue; i++) {
		col_[i] = tempCol[i];
		value_[i] = tempVal[i];
	}

	nCol_ = col;

	delete[]tempVal;
	delete[]tempCol;

}

std::ostream &operator<<(std::ostream &a, const MatrixCSR &e) {
	float valor = 0;
	for (int i = 0; i < e.nRow_; i++) {
		for (int j = 0; j < e.nCol_; j++)
			a << (e.getValor(i, j, valor) ? valor : 0) << " ";
		a << "\n";
	}
	return a;
}


bool MatrixCSR::getValor(const int &row, const int &col, float &value) const {
	if (row > nRow_ || col > nCol_ || row < 0 || col < 0) throw "La seleccio esta fora de la matriu o son nombre negatius";

	bool t = false;
	int i = rowIndex_[row];

	while (i < rowIndex_[row + 1] && !t) {
		if (col_[i] == col) {
			value = value_[i];
			t = true;
		}
		i++;
	}
	return t;
}


MatrixCSR &MatrixCSR::operator=(const MatrixCSR &e) {
	this->~MatrixCSR();
	this->copy(e);
	return *this;
}
