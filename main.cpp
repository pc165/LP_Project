#include <iostream>
#include "Matrix.h"
using std::cout;
int main() {

	//test
	MatrixCSR a, b, c;
	//a.init(10, 2);
	a.setValor(9,0,1);
	cout << a << "\n\n";
	a.setValor(0,2,2);
	cout << a << "\n\n";
	a.setValor(0,8,8);
	cout << a << "\n\n";
	a.setValor(3,5,3);
	cout << a << "\n\n";
	a.setValor(3,0,1);
	cout << a << "\n\n";
	a.setValor(4,3,3);
	cout << a << "\n\n";
	a.setValor(8,5,5);
	cout << a << "\n\n";
	a.setValor(0,4,4);
	cout << a << "\n\n";

	/*
	b = a;
	b.setCol(1);
	cout << b << "\n\n";

	std::vector<std::vector<float>> matrix = {
		{1,2,3,4,5,6,7,8},
		{1,2,3,4,5,6,7,8},
		{1,2,3,4,5,6,7,8},
		{1,2,3,4,5,6,7,8},
		{1,2,3,4,5,6,7,8},
		{1,2,3,4,5,6,7,8},
		{1,2,3,4,5,6,7,8},
	};

	for (int i = 0; i < matrix.size(); i++) {
		for (int j = 0; j < matrix[0].size(); j++) {
			c.setValor(i, j, matrix[i][j]);
			//matrix[i][j] /= 3.0f;
		}
	}
	cout << c << "\n\n";
	float valor = 0.0f;
	for (int i = 0; i < matrix.size(); i++) {
		for (int j = 0; j < matrix[0].size(); j++) {
			c.getValor(i, j, valor);
			cout << std::boolalpha << (fabs(valor - matrix[i][j]) < FLT_EPSILON) << " ";
		}
		cout << "\n";
	}
	cout << "\n";

	a = b = c;
	b.setRowCol(2, 2);
	c.setRowCol(7, 7);
	cout << a << "\n\n " << b << "\n\n" << c << "\n\n";

	c.init(4, 4);

	cout << c;

	std::vector<std::vector<float>> matrix2 = {
		{1,2,3},
		{1,2,3},
		{1,2,3}
	};

	MatrixCSR v(matrix2.size(), matrix2[0].size());

	for (int i = 0; i < matrix2.size(); i++) {
		for (int j = 0; j < matrix2[0].size(); j++) {
			v.setValor(i, j, matrix2[i][j]);
		}
	}
	//v = v * v;
	cout << v;


	std::vector<float> vector = { 1,2,3 };

	//vector = v * vector;

	//for (int i = 0; i < vector.size(); i++) {
	//	cout << vector[i];
	//}

	*/

	return 0;
}