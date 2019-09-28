#pragma once
#include <vector>
#include <iostream>
class MatrixCSR {
public:
	MatrixCSR();
	MatrixCSR(const int &row, const int &col);
	MatrixCSR(const MatrixCSR &e);
	~MatrixCSR();
	void setValor(const int &row, const int &col, const float &value); // ha de redimensionar la matriu a row y col conservant els valors
	bool getValor(const int &row, const int &col, float &value) const;
	void init(const int &row, const int &col);
	friend std::ostream &operator<<(std::ostream &a, const MatrixCSR &e);
	MatrixCSR operator+(const MatrixCSR &e);
	MatrixCSR operator-(const MatrixCSR &e);
	MatrixCSR operator*(const MatrixCSR &e);
	MatrixCSR operator*(const float &e);
	MatrixCSR operator/(const float &e);
	MatrixCSR &operator=(const MatrixCSR &e);

	void setRow(const int &e) { resizeRow(e); }
	void setCol(const int &e) { resizeCol(e); }
	inline int getNFiles()const { return nRow_; }
	inline int getNColumnes()const { return nCol_; }
private:
	void copy(const MatrixCSR& e);
	void resizeRow(const int &row);
	void resizeCol(const int &row);


private:
	int *col_, *rowIndex_; // L'ultim valor de "rowIndex_" es el nombre total de valors diferents a zero
	float *value_;
	int nRow_, nCol_;
};