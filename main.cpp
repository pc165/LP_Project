#include <iostream>
#include "Matrix.h"
using std::cout;
int main() {

	//test
	MatrixCSR a, b, c;

	a.init(10, 10);
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			a.setValor(i, j, i * j);

	for (int i = 0; i < 10; i++)
		a.setValor(i, 0, i);
	cout << a << "\n\n";

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
			c.setValor(i, j, matrix[i][j] / 3.0f);
			matrix[i][j] /= 3.0f;
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

	MatrixCSR d(c);
	float valorD = 0.0f;
	for (int i = 0; i < matrix.size(); i++) {
		for (int j = 0; j < matrix[0].size(); j++) {
			c.getValor(i, j, valor);
			d.getValor(i, j, valorD);
			cout << std::boolalpha << (fabs(valor - valorD) < FLT_EPSILON) << " ";
		}
		cout << "\n";
	}
	cout << "\n";

	a = b = c = d;
	a.setCol(1);
	a.setRow(1);
	b.setRowCol(2, 2);
	c.setRowCol(7, 7);
	cout << a << "\n\n " << b << "\n\n" << c << "\n\n";

	return 0;
}