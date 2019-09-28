#include <iostream>
#include "Matrix.h"
using std::cout;
int main() {
	MatrixCSR a;
	a.setValor(4, 3, 1);
	cout << "\n\n\nMatriz A\n";
	cout << a;

	MatrixCSR b(a);
	cout << "\n\n\nMatriz B\n";
	cout << b;

	MatrixCSR c, d;
	d = c = b;
	c.setCol(1);
	d.setRow(10);
	cout << "\n\n\nMatriz C\n";
	cout << c;

	cout << "\n\n\nMatriz d\n";
	cout << d;

	/*
	const int ROW = 5, COL = 5;
	float matrix[ROW][COL] = {
			{0,0,0,0,1}, // 0
			{2,0,0,3,4}, // 1
			{2,3,0,0,0}, // 2
			{0,0,0,0,0}, // 3
			{0,0,0,0,1}, // 4
	};	//	 0 1 2 3 4

	int val[7] = { 1, 2, 3, 8, 2, 3, 1 };
	int col[7] = { 4, 0, 3, 4, 0, 1, 4 };
				// 0, 1, 2, 3, 4, 5, 6
	int row[ROW + 1] = { 0, 1, 4, 6, 6, 7 };

	//*/


	return 0;
}