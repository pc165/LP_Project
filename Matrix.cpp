#include "Matrix.h"

MatrixCSR::MatrixCSR(const int &row, const int &col):col_(nullptr), value_(nullptr), rowIndex_(nullptr), nRow_(row), nCol_(col), sizeIndexRow_(0) {

}

MatrixCSR::MatrixCSR(const MatrixCSR &e) : nRow_(e.nRow_), nCol_(e.nCol_), sizeIndexRow_(e.sizeIndexRow_) {

	int totalValuesCol = 0;

	if (e.rowIndex_ != nullptr)
		totalValuesCol = int(e.rowIndex_[sizeIndexRow_ - 1]); // is always accesible?
	else throw "No es pot accedir a: e.rowIndex_[maxIndexRow_ - 1]";

	value_ = new float[totalValuesCol];
	col_ = new float[totalValuesCol];
	rowIndex_ = new float[sizeIndexRow_];

	for (int i = 0; i < totalValuesCol; i++) {
		col_[i] = e.col_[i];
		value_[i] = e.value_[i];
	}
	for (int i = 0; i < sizeIndexRow_; i++)
		rowIndex_[i] = e.rowIndex_[i];
}
