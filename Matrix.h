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

	inline void setRow(const int &e) { nRow_ = e; }
	inline void setCol(const int &e) { nCol_ = e; }
	inline int getNFiles()const { return nRow_; }
	inline int getNColumnes()const { return nCol_; }
private:
	void copy(const MatrixCSR &e);


private:
	std::vector<int> rowIndex_, rowNZV_; // L'ultim valor de "rowIndex_" es el nombre total de valors diferents a zero. rowNZV == row Non Zero Value
	std::vector<colVal> colVal_;
	int nRow_, nCol_;

};