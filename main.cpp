#include <iostream>
#include "Matrix.h"
using std::cout;
void multiplyMatrixVector() {
	 MatrixCSR a;
	 std::vector<std::vector<float>> matrix = {
	 {0,2,3},
	 {0,5,6},
	 {0,8,0},
	 {0,0,12},
	 };
	 std::vector<float> vector = { 1,0,1 };
	 for (int i = 0; i < matrix.size(); i++)
		  for (int j = 0; j < matrix[0].size(); j++)
				a.setValor(i, j, matrix[i][j]);
	 cout << a;
	 std::vector<float> res = a * vector;

	 for (auto &i : res)
		  cout << i << " ";
	 cout << "\n";
}

int main() {
	 multiplyMatrixVector();




	 return 0;
}