#include <iostream>
#include "Matrix.h"
using std::cout;
///*

inline int randMinMax(const int &min, const int &max) {
	 return (min + rand() % (min - max + 1));
}

void multiplyMatrixMatrix() {
	 cout << "Test MatriuCSR * MatriuCSR\n";
	 try {
		  std::vector<std::vector<float>> aMat = {
				{1,2,3},
				{0,0,0},
				{7,8,9},
		  }, bMat = {
			  {10,11,12},
			  {0,0,0},
			  {16,17,18},
		  };
		  MatrixCSR a(aMat.size(), aMat[0].size()), b(bMat.size(), bMat[0].size());

		  for (int i = 0; i < aMat.size(); i++)
				for (int j = 0; j < aMat[0].size(); j++)
					 a.setValor(i, j, aMat[i][j]);
		  cout << a;

		  for (int i = 0; i < bMat.size(); i++)
				for (int j = 0; j < bMat[0].size(); j++)
					 b.setValor(i, j, bMat[i][j]);
		  cout << b;

		  MatrixCSR res;

		  res = a * b;
	 } catch (const char *mg) {
		  cout << "Exception: " << mg << "\n";
	 }

}
void multiplyMatrixVector() {
	 cout << "Test MatriuCSR * std::vector\n";
	 try {
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
		  //cout << a;
		  std::vector<float> res = a * vector;

		  for (auto &i : res)
				cout << i << " ";
		  cout << "\n\n";
	 } catch (const char *a) {
		  cout << "Exception: " << a << "\n";
	 }
}
void setValor() {
	 cout << "Test MatriuCSR::setValor()\n";
	 try {
		  MatrixCSR a;

		  for (int i = 0; i < 10; i++) {
				for (int z = 0; z < 10; z++) {
					 int j = randMinMax(0, 9);
					 a.setValor(i, j, i + 1);
				}
		  }
		  cout << "\n";
	 } catch (const char *c) {
		  cout << c;
	 }
}
//*/
int main() {
	 //multiplyMatrixVector();
	 //multiplyMatrixMatrix();
	 setValor();
	 return 0;
}