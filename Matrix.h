#pragma once
#include <vector>
#include <iostream>
typedef struct {
	int col = 0;
	float data = 0.0f;
} colVal;

class MatrixCSR {
public:
	MatrixCSR();
	MatrixCSR(const int &row, const int &col);
	MatrixCSR(const MatrixCSR &e);
	~MatrixCSR();
	void setValor(const int &row, const int &col, const float &value);
	bool getValor(const int &row, const int &col, float &value) const;
	void init(const int &row, const int &col);
	friend std::ostream &operator<<(std::ostream &a, const MatrixCSR &e);
	MatrixCSR operator+(const MatrixCSR &e);
	MatrixCSR operator-(const MatrixCSR &e);
	MatrixCSR operator*(const MatrixCSR &e);
	MatrixCSR operator*(const float &e);
	MatrixCSR operator/(const float &e);
	MatrixCSR &operator=(const MatrixCSR &e);

	void setRow(const int &e);
	void setCol(const int &e);
	void setRowCol(const int &e, const int &a);
	inline int getNFiles()const { return nRow_; }
	inline int getNColumnes()const { return nCol_; }
private:
	void copy(const MatrixCSR &e);


private:
	std::vector<int> rowIndex_;
	std::vector<colVal> colVal_;
	int nRow_, nCol_;

};