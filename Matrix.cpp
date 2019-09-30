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

}

void MatrixCSR::copy(const MatrixCSR &e) {
	nRow_ = e.nRow_;
	nCol_ = e.nCol_;
	rowIndex_ = e.rowIndex_;
	colVal_ = e.colVal_;
	rowNZV_ = e.rowNZV_;
}


void MatrixCSR::init(const int &row, const int &col) {
	if (row < 0 || col < 0) throw "Error: Les columnes i files han de ser positius\n";

	nRow_ = row;
	nCol_ = col;
	rowIndex_.resize(nRow_ + 1);
	rowNZV_.resize(nRow_);
	for (int i = 0; i < nRow_ + 1; i++)
		rowIndex_[i] = 0;
}


void MatrixCSR::setValor(const int &row, const int &col, const float &value) {
	if (row < 0 || col < 0) throw "Error: Els indexs son negatius";
	if (fabs(value - 0.0f) < FLT_EPSILON) return;


	if (!(row < nRow_ && col < nCol_)) {
		int tRow = row + 1;
		int tCol = col + 1;

		rowNZV_.resize(tRow);
		rowNZV_[row]++;
		rowIndex_.resize(tRow + 1);

		for (int i = nRow_; i < tRow + 1; i++)
			rowIndex_[i] = rowIndex_[i == 0 ? i : i - 1] + rowNZV_[i == 0 ? i : i - 1];

		colVal_.insert(colVal_.begin() + rowIndex_[row], { col,value });

		nRow_ = tRow;
		nCol_ = tCol;
	} else {
		bool found = false;
		int i = rowIndex_[row];
		while (i < rowIndex_[row + 1] && !found) {
			if (colVal_[i].col == col) {
				colVal_[i].data = value;
				found = true;
			}
			i++;
		}
		if (!found) {
			colVal_.insert(colVal_.begin() + rowIndex_[row], { col,value });
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
			a << colVal << " ";
		}
		a << "\n";
	}
	return a;
}


bool MatrixCSR::getValor(const int &row, const int &col, float &value) const {
	if (row < 0 || col < 0) throw "Error: Els indexs son negatius";

	value = 0.0f;
	if (row < nRow_ && col < nCol_)
		for (int i = rowIndex_[row]; i < rowIndex_[row + 1]; i++)
			if (colVal_[i].col == col) {
				value = colVal_[i].data;
				break;
			}

	return  (row < nRow_ && col < nCol_);
}


MatrixCSR &MatrixCSR::operator=(const MatrixCSR &e) {
	//this->~MatrixCSR();
	this->copy(e);
	return *this;
}


