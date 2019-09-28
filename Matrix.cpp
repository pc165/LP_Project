#include "Matrix.h"

MatrixCSR::MatrixCSR(const int &row, const int &col):col_(nullptr), value_(nullptr), rowIndex_(nullptr), nRow_(row), nCol_(col), sizeIndexRow_(0) {

}

MatrixCSR::MatrixCSR(const MatrixCSR &e) : nRow_(e.nRow_), nCol_(e.nCol_), sizeIndexRow_(e.sizeIndexRow_) {

	int totalValuesCol = 0;

	totalValuesCol = int(e.rowIndex_[sizeIndexRow_ - 1]); // is always accesible?
	value_ = new float[totalValuesCol];
	col_ = new int[totalValuesCol];
	rowIndex_ = new int[sizeIndexRow_];

	for (int i = 0; i < totalValuesCol; i++) {
		col_[i] = e.col_[i];
		value_[i] = e.value_[i];
	}
	for (int i = 0; i < sizeIndexRow_; i++)
		rowIndex_[i] = e.rowIndex_[i];
}

void MatrixCSR::setValor(const int &row, const int &col, const float &value) {

}

void MatrixCSR::resize() {

	int *tempIndex = rowIndex_;
	rowIndex_ = new int[sizeIndexRow_ + 1];

	for (int i = 0; i < sizeIndexRow_; i++)
		rowIndex_[i] = tempIndex[i];

	int newTotalValue = rowIndex_[sizeIndexRow_ - 1] = rowIndex_[sizeIndexRow_ - 2] + 1;
	sizeIndexRow_++;
	delete[]tempIndex;

	int *tempCol = col_;
	float *tempVal = value_;

	col_ = new int[newTotalValue];
	value_ = new float[newTotalValue];

	for (int i = 0; i < newTotalValue; i++) {
		col_[i] = tempCol[i];
		value_[i] = tempVal[i];
	}

	delete[]tempCol;
	delete[]tempVal;
}
