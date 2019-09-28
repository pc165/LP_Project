#include "Matrix.h"
#include <iostream>

MatrixCSR::MatrixCSR():col_(nullptr), value_(nullptr), rowIndex_(nullptr), nRow_(0), nCol_(0) {
	rowIndex_ = new int[1];
	value_ = new float[0];
	col_ = new int[0];
	rowIndex_[0] = 0;// total de elements no zero
}


MatrixCSR::MatrixCSR(const int &row, const int &col):col_(nullptr), value_(nullptr), rowIndex_(nullptr), nRow_(row), nCol_(col) {
	rowIndex_ = new int[nRow_ + 1];
	// numero total de valors en la matriu
	rowIndex_[nRow_] = 0;
	// arrays de tamany 0
	value_ = new float[0];
	col_ = new int[0];
	for (int i = 0; i < nRow_; i++)
		rowIndex_[i] = 0;

}

MatrixCSR::MatrixCSR(const MatrixCSR &e): nRow_(e.nRow_), nCol_(e.nCol_) {
	rowIndex_ = new int[nRow_ + 1];
	value_ = new float[e.rowIndex_[e.nRow_]];
	col_ = new int[e.rowIndex_[e.nRow_]];

	for (int i = 0; i < e.rowIndex_[e.nRow_]; i++) {
		col_[i] = e.col_[i];
		value_[i] = e.value_[i];
	}
	for (int i = 0; i < nRow_ + 1; i++)
		rowIndex_[i] = e.rowIndex_[i];
}

void MatrixCSR::setValor(const int &row, const int &col, const float &value) {
	if (row != nRow_) resizeRow(row);

}

void MatrixCSR::resizeRow(const int &row) {
	int *tempRowIndex = rowIndex_;
	rowIndex_ = new int[row + 1];
	rowIndex_[row] = tempRowIndex[nRow_];

	for (int i = 0; i < (row > nRow_ ? nRow_ : row); i++)
		rowIndex_[i] = tempRowIndex[i];

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
	for (int i = 0; i < e.rowIndex_[e.nRow_]; i++) {
		for (int j = e.rowIndex_[i]; j < e.rowIndex_[i + 1]; j++) {
			// imprimir zeros????
			a << e.value_[j] << " ";
		}
		a << "\n";
	}
	return a;
}
