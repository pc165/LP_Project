#include <iostream>
#include <string>
#include <fstream>

#include "Matrix.h"
using namespace std;
/*
inline int randMinMax(const int &min, const int &max) {
	return (min + rand() % (min - max + 1));
}

void multiplyMatrixMatrix() {
	cout << "Test MatriuCSR * MatriuCSR------------------------------------------------------------\n";
	try {
		std::vector<std::vector<float>> aMat = {
			{1,2,3},
			{4,5,6},
			{7,8,9},
		}, bMat = {
			{10,11,12},
			{13,14,15},
			{16,17,18},
		};
		MatriuSparse a(aMat.size(), aMat[0].size()), b(bMat.size(), bMat[0].size());

		for (int i = 0; i < aMat.size(); i++)
			for (int j = 0; j < aMat[0].size(); j++)
				a.setValor(i, j, aMat[i][j]);
		cout << a;

		for (int i = 0; i < bMat.size(); i++)
			for (int j = 0; j < bMat[0].size(); j++)
				b.setValor(i, j, bMat[i][j]);
		cout << b;

		MatriuSparse res;

		res = a * b;
		cout << res;
	} catch (const char *mg) {
		cout << "Exception: " << mg << "\n";
	}

}
void multiplyMatrixVector() {
	cout << "Test MatriuCSR * std::vector------------------------------------------------------------\n";
	try {
		MatriuSparse a;
		std::vector<std::vector<float>> matrix = {
		{1,2,3},
		{4,5,6},
		{7,8,9},
		{10,11,12},
		};
		std::vector<float> vector = { 1,2,3 };

		for (int i = 0; i < matrix.size(); i++)
			for (int j = 0; j < matrix[0].size(); j++)
				a.setValor(i, j, matrix[i][j]);
		cout << a;
		std::vector<float> res = a * vector;

		for (auto &i : res)
			cout << i << " ";
		cout << "\n\n";
	} catch (const char *a) {
		cout << "Exception: " << a << "\n";
	}
}
void setValor() {
	cout << "Test MatriuCSR::setValor()------------------------------------------------------------\n";
	try {
		MatriuSparse a;
		for (int i = 0; i < 20; i++) {
			for (int z = 0; z < 20; z++) {
				int j = randMinMax(0, 20);
				a.setValor(i, j, i);
			}
		}
		cout << a;
		cout << "\n";
	} catch (const char *c) {
		cout << c;
	}
}
int main() {
	multiplyMatrixVector();
	multiplyMatrixMatrix();
	setValor();
	return 0;
}
//*/

///*


bool IgualFitxers(string nomF1, string nomF2, string nomFRes)
{
	ifstream f1,f2;
	f1.open(nomF1);
	f2.open(nomF2);
	bool iguals = true;

	if (f1.is_open() && f2.is_open()) 
	{
		ofstream fitxCompara(nomFRes, ios::out);
		int cont = 0, linia = 0;
		while (!f1.eof() && !f2.eof()) 
		{
			++linia;
			string a, b;
			getline(f1, a); 
			int pos;
			if ((pos = a.find_first_of(char(13))) < string::npos)
				a = a.substr(0, pos);
			getline(f2, b); 
			if ((pos = b.find_first_of(char(13))) < string::npos)
				b = b.substr(0, pos);
			if (a != b) 
			{ 
				++cont;
				fitxCompara << flush << "Error num " << cont << " a linia " << linia << endl << flush;
				iguals = false;				
			}
		}
		if (!f1.eof() && f2.eof()) 
		{
			fitxCompara << "Fitxer " << nomF2 << " es mes curt que " << nomF1 << endl;
		}
		if (!f2.eof() && f1.eof()) 
		{
			fitxCompara << "Fitxer " << nomF1 << " es mes curt que " << nomF2 << endl;
		}
		fitxCompara.close();		
	}
	else
	{
		throw "Error al fitxer de sortida al comparar.";
	}
	return iguals;
}

int main()
{

	try
	{
		float grade = 0.0;
		bool valid = true;
			cout << "Comment :=>> ==========================================" << endl;
			cout << "Comment :=>> TEST SOBRE XARXA1 ....................... " << endl;
			cout << "Comment :=>> ==========================================" << endl;

			string nomFitxer = "Xarxa1.txt";
			cout << "Comment :=>> ==========================================" << endl;
			cout << "Comment :=>> LLEGINT I CREANT XARXA1 ................. " << endl;
			MatriuSparse m1(nomFitxer);

			cout << "Comment :=>> ==========================================" << endl;
			cout << "Comment :=>> ESCRIVINT XARXA1 ........................." << endl;
			cout << m1;

			cout << "Comment :=>> ==========================================" << endl;
			cout << "Comment :=>> AFEGINT ELEMENTS          ................" << endl;
			cout << "Comment :=>> AFEGINT 7 a posicio (0,2) ................" << endl;
			m1.setVal(0, 2, 7);
			cout << "Comment :=>> AFEGINT 8 a posicio (8,1) ................" << endl;
			m1.setVal(8, 1, 8);
			cout << "Comment :=>> AFEGINT 9 a posicio (5,9) ............... " << endl;
			m1.setVal(5, 9, 9);

			cout << "Comment :=>> ==========================================" << endl;
			cout << "Comment :=>> ESCRIVINT XARXA1 ........................." << endl;
			cout << m1;

			cout << "Comment :=>> ==========================================" << endl;
			cout << "Comment :=>> ESCRIVINT FITXER XARXA1 .................." << endl;
			ofstream fitxerResultatX1;
			fitxerResultatX1.open("ResX1COORD.txt");
			if (fitxerResultatX1.is_open())
			{
				fitxerResultatX1 << m1;
				fitxerResultatX1.close();
			}

			if (IgualFitxers("ResX1COORD.txt", "ResX1COORDEsperat.txt", "ResComparaX1.txt"))
			{
				cout << "Comment :=>> XARXA1 BEN CONSTRUIT OK ....................... " << endl;
				grade++;
			}
			else
			{
				cout << "Comment :=>> ERROR CONSTRUCCIO XARXA1 ....................... " << endl;
				valid = false;
			}

			cout << "Comment :=>> ==========================================" << endl;
			cout << "Comment :=>> VALIDANT GETVALORS ......................." << endl;

			float valor;
			bool trobat = m1.getVal(0, 0, valor);
			if (trobat)
				if (valor == 0)
				{
					cout << "Comment :=>> OK VAL(0,0): " << valor << endl;
					grade++;
				}
				else
				{
					cout << "Comment :=>> OK EXISTEIX VAL(0,0): Pero ERROR val " << valor << " Valor correcte 0" << endl;
					valid = false;
				}
			else
			{
				cout << "Comment :=>> ERROR VAL(0,0): NO EXISTEIX. HAURIA D'EXISTIR i VALOR=0" << endl;
				valid = false;
			}

			trobat = m1.getVal(3, 2, valor);
			if (trobat)
				if (valor == 1)
				{
					cout << "Comment :=>>OK VAL(3,2): " << valor << endl;
					grade++;
				}
				else
				{
					cout << "Comment :=>>OK EXISTEIX VAL(3,2): Pero ERROR val " << valor << " Valor correcte 1" << endl;
					valid = false;
				}
			else
			{
				cout << "Comment :=>>VAL(3,2): ERROR NO EXISTEIX. HAURIA D'EXISTIR i VALOR=1" << endl;
				valid = false;
			}

			trobat = m1.getVal(0, 2, valor);
			if (trobat)
				if (valor == 7)
				{
					cout << "Comment :=>> OK VAL(0,2): " << valor << endl;
					grade++;
				}
				else
				{
					cout << "Comment :=>>OK EXISTEIX VAL(0,2): Pero ERROR val " << valor << " Valor correcte 7" << endl;
					valid = false;
				}
			else
			{
				cout << "Comment :=>>VAL(0,2): ERROR NO EXISTEIX. HAURIA D'EXISTIR i VALOR=7" << endl;
				valid = false;
			}

			trobat = m1.getVal(8, 1, valor);
			if (trobat)
				if (valor == 8)
				{
					cout << "Comment :=>> OK VAL(8,1): " << valor << endl;
					grade++;
				}
				else
				{
					cout << "Comment :=>>OK EXISTEIX VAL(8,1): Pero ERROR val " << valor << " Valor correcte 8" << endl;
					valid = false;
				}
			else
			{
				cout << "Comment :=>>VAL(8,1): ERROR NO EXISTEIX. HAURIA D'EXISTIR i VALOR=8" << endl;
				valid = false;
			}

			trobat = m1.getVal(9, 2, valor);
			if (trobat)
				if (valor == 0)
				{
					cout << "Comment :=>> OK VAL(9,2): " << valor << endl;
					grade++;
				}
				else
				{
					cout << "Comment :=>>OK EXISTEIX VAL(9,2): Pero ERROR val " << valor << " Valor correcte 0" << endl;
					valid = false;
				}
			else
			{
				cout << "Comment :=>>VAL(9,2): ERROR NO EXISTEIX. HAURIA D'EXISTIR i VALOR=0" << endl;
				valid = false;
			}

			trobat = m1.getVal(4, 10, valor);
			if (trobat)
			{
				cout << "Comment :=>>VAL(4,10): " << valor << " . ERROR NO HAURIA D'EXISTIR" << endl;
				valid = false;
			}
			else
			{
				cout << "Comment :=>>VAL(4,10): OK NO EXISTEIX" << endl;
				grade++;
			}

			cout << "Comment :=>> ==========================================" << endl;
			cout << "Comment :=>> VALIDANT * per float......................" << endl;
			MatriuSparse mProdX1 = m1 * 6;
			ofstream fProdX1("MatProdX1COORD.txt", ios::out);
			fProdX1 << mProdX1;
			fProdX1.close();
			if (IgualFitxers("MatProdX1COORD.txt", "MatProdX1COORDEsperat.txt", "ResCompMatProdX1.txt"))
			{
				cout << "Comment :=>> Prod per 6 XARXA1 OK ....................... " << endl;
				grade++;
			}
			else
			{
				cout << "Comment :=>> ERROR Prod per 6 XARXA1 ....................... " << endl;
				valid = false;
			}

			cout << "Comment :=>> ==========================================" << endl;
			cout << "Comment :=>> VALIDANT / per float......................" << endl;
			MatriuSparse mDivX1Prod = mProdX1 / 3;
			ofstream fDivX1("MatDivX1COORD.txt", ios::out);
			fDivX1 << mDivX1Prod;
			fDivX1.close();
			if (IgualFitxers("MatDivX1COORD.txt", "MatDivX1COORDEsperat.txt", "ResCompMatDivX1.txt"))
			{
				cout << "Comment :=>> (XARXA1*6)/3 OK ....................... " << endl;
				grade++;
			}
			else
			{
				cout << "Comment :=>> ERROR (XARXA1*6)/3 ....................... " << endl;
				valid = false;
			}

			cout << "Comment :=>> ==========================================" << endl;
			cout << "Comment :=>> VALIDANT * per vector....................." << endl;
			vector<float> v;
			v.resize(m1.getNFiles(), 1);

			vector<float> v2 = m1*v;
			ofstream fProdVX1("VectProdX1COORD.txt", ios::out);
			for (int i = 0; i < v2.size(); i++)
			{
				if (v2[i] != 0)
				{
					fProdVX1 << "[POS: " << i << " ; VAL: " << v2[i] << " ] " << endl;
				}
			}
			fProdVX1.close();
			if (IgualFitxers("VectProdX1COORD.txt", "VectProdX1COORDEsperat.txt", "ResCompVectProdX1.txt"))
			{
				cout << "Comment :=>> (XARXA1*vector) OK ....................... " << endl;
				grade++;
			}
			else
			{
				cout << "Comment :=>> ERROR (XARXA1*vector) ....................... " << endl;
				valid = false;
			}
			if (valid)
				cout << "Comment :=>> Final del test XARXA1 sense errors" << endl;
			if (grade < 0)
				grade = 0;
			cout << endl << "Comment :=>> Grade : " << grade << endl;


			cout << "Comment :=>> ==========================================" << endl;
			cout << "Comment :=>> TEST SOBRE SubEpinions2Ordenat........... " << endl;

			string nomFitxerRels2 = "SubEpinions2Ordenat.txt";

			cout << "Comment :=>> LLEGINT I CREANT SubEpinions2Ordenat..... " << endl;
			MatriuSparse m2(nomFitxerRels2);

			cout << "Comment :=>> ESCRIVINT SubEpinions2Ordenat............ " << endl;
			cout << m2;

			cout << "Comment :=>> AFEGINT 7 a posicio (0,2) ............... " << endl;
			m2.setVal(0, 2, 7);
			cout << "Comment :=>> AFEGINT 8 a posicio (8,1) ............... " << endl;
			m2.setVal(8, 1, 8);
			cout << "Comment :=>> AFEGINT 9 a posicio (5,9) ............... " << endl;
			m2.setVal(5, 9, 9);

			cout << "Comment :=>> ESCRIVINT SubEpinions2Ordenat............ " << endl;
			cout << m2;


			trobat = m2.getVal(0, 0, valor);
			if (trobat)
				if (valor == 0)
				{
					cout << "Comment :=>> OK VAL(0,0): " << valor << endl;
					grade++;
				}
				else
				{
					cout << "Comment :=>> OK EXISTEIX VAL(0,0): Pero ERROR val " << valor << " Valor correcte 0" << endl;
					valid = false;
				}
			else
			{
				cout << "Comment :=>> ERROR VAL(0,0): NO EXISTEIX. HAURIA D'EXISTIR i VALOR=0" << endl;
				valid = false;
			}

			trobat = m2.getVal(3, 2, valor);
			if (trobat)
				if (valor == 0)
				{
					cout << "Comment :=>>OK VAL(3,2): " << valor << endl;
					grade++;
				}
				else
				{
					cout << "Comment :=>>OK EXISTEIX VAL(3,2): Pero ERROR val " << valor << " Valor correcte 0" << endl;
					valid = false;
				}
			else
			{
				cout << "Comment :=>>VAL(3,2): ERROR NO EXISTEIX. HAURIA D'EXISTIR i VALOR=1" << endl;
				valid = false;
			}

			trobat = m2.getVal(0, 2, valor);
			if (trobat)
				if (valor == 7)
				{
					cout << "Comment :=>> OK VAL(0,2): " << valor << endl;
					grade++;
				}
				else
				{
					cout << "Comment :=>>OK EXISTEIX VAL(0,2): Pero ERROR val " << valor << " Valor correcte 7" << endl;
					valid = false;
				}
			else
			{
				cout << "Comment :=>>VAL(0,2): ERROR NO EXISTEIX. HAURIA D'EXISTIR i VALOR=7" << endl;
				valid = false;
			}

			trobat = m2.getVal(8, 1, valor);
			if (trobat)
				if (valor == 8)
				{
					cout << "Comment :=>> OK VAL(8,1): " << valor << endl;
					grade++;
				}
				else
				{
					cout << "Comment :=>>OK EXISTEIX VAL(8,1): Pero ERROR val " << valor << " Valor correcte 8" << endl;
					valid = false;
				}
			else
			{
				cout << "Comment :=>>VAL(8,1): ERROR NO EXISTEIX. HAURIA D'EXISTIR i VALOR=8" << endl;
				valid = false;
			}

			trobat = m2.getVal(30, 60000, valor);
			if (trobat)
			{
				cout << "Comment :=>>VAL(30,60000): " << valor << " . ERROR NO HAURIA D'EXISTIR" << endl;
				valid = false;
			}
			else
			{
				cout << "Comment :=>>VAL(30,60000): OK NO EXISTEIX" << endl;
				grade++;
			}

			trobat = m2.getVal(60000, 30, valor);
			if (trobat)
			{
				cout << "Comment :=>>VAL(60000,30): " << valor << " . ERROR NO HAURIA D'EXISTIR" << endl;
				valid = false;
			}
			else
			{
				cout << "Comment :=>>VAL(60000,30): OK NO EXISTEIX" << endl;
				grade++;
			}

			ofstream fResSubEpiOrdenat;
			fResSubEpiOrdenat.open("ResSubEpi2OrdenatCOORD.txt");
			if (fResSubEpiOrdenat.is_open())
			{
				fResSubEpiOrdenat << m2;
				fResSubEpiOrdenat.close();
			}

			if (IgualFitxers("ResSubEpi2OrdenatCOORD.txt", "ResSubEpi2OrdenatCOORDEsperat.txt", "ResComparaSubEpi2Ordenat.txt"))
			{
				cout << "Comment :=>> SubEpinions2Ordenat BEN CONSTRUIT OK ..................... " << endl;
				grade++;
			}
			else
			{
				cout << "Comment :=>> ERROR CONSTRUCCIO SubEpinions2Ordenat..................... " << endl;
				valid = false;
			}

			cout << "Comment :=>> ==========================================" << endl;
			cout << "Comment :=>> VALIDANT * per float......................" << endl;
			MatriuSparse mProdSubEpiOrdenat = m2 * 6;

			ofstream fProdSubEpiOrdenat("MatProdSubEpi2OrdenatCOORD.txt", ios::out);
			fProdSubEpiOrdenat << mProdSubEpiOrdenat;
			fProdSubEpiOrdenat.close();
			if (IgualFitxers("MatProdSubEpi2OrdenatCOORD.txt", "MatProdSubEpi2OrdenatCOORDEsperat.txt", "ResCompMatProdSubEpi2Ordenat.txt"))
			{
				cout << "Comment :=>> Prod per 6 SubEpi2Ordenat OK ....................... " << endl;
				grade++;
			}
			else
			{
				cout << "Comment :=>> ERROR Prod per 6 SubEpi2Ordenat ....................... " << endl;
				valid = false;
			}
			cout << "Comment :=>> ==========================================" << endl;
			cout << "Comment :=>> VALIDANT / per float......................" << endl;
			MatriuSparse mDivSubEpiOrdenatProd = mProdSubEpiOrdenat / 3;
			ofstream fDivSubEpiOrdenat("MatDivSubEpi2OrdenatCOORD.txt", ios::out);
			fDivSubEpiOrdenat << mDivSubEpiOrdenatProd;
			fDivSubEpiOrdenat.close();
			if (IgualFitxers("MatDivSubEpi2OrdenatCOORD.txt", "MatDivSubEpi2OrdenatCOORDEsperat.txt", "ResCompMatDivSubEpi2Ordenat.txt"))
			{
				cout << "Comment :=>> (SubEpi2Ordenat*6)/3 OK ....................... " << endl;
				grade++;
			}
			else
			{
				cout << "Comment :=>> ERROR (SubEpi2Ordenat*6)/3 ....................... " << endl;
				valid = false;
			}
			cout << "Comment :=>> ==========================================" << endl;
			cout << "Comment :=>> VALIDANT * per vector....................." << endl;
			vector<float> vSubEpiOrdenat;
			vSubEpiOrdenat.resize(m2.getNFiles(), 1);

			vector<float> v2SubEpiOrdenat = m2*vSubEpiOrdenat;

			ofstream fProdVSubEpiOrdenat("VectProdSubEpi2OrdenatCOORD.txt", ios::out);
			for (int i = 0; i < v2SubEpiOrdenat.size(); i++)
			{
				if (v2SubEpiOrdenat[i] != 0)
				{
					fProdVSubEpiOrdenat << "[POS: " << i << " ; VAL: " << v2SubEpiOrdenat[i] << " ] " << endl;
				}
			}
			fProdVSubEpiOrdenat.close();
			if (IgualFitxers("VectProdSubEpi2OrdenatCOORD.txt", "VectProdSubEpi2OrdenatCOORDEsperat.txt", "ResCompVectProdSubEpi2Ordenat.txt"))
			{
				cout << "Comment :=>> (SubEpi2Ordenat*vector) OK ....................... " << endl;
				grade++;
			}
			else
			{
				cout << "Comment :=>> ERROR (SubEpi2Ordenat*vector) ....................... " << endl;
				valid = false;
			}

			if (valid)
				cout << "Comment :=>> Final del test SUBEPINIONS2ORDENAT sense errors" << endl;
			if (grade < 0)
				grade = 0;
			cout << endl << "Comment :=>> Grade : " << grade << endl;


			cout << "Comment :=>> ==========================================" << endl;
			cout << "Comment :=>> TEST SOBRE EPINIONS ORDENAT.............. " << endl;

			string nomFitxerRels = "EpinionsOrdenat.txt";

			cout << "Comment :=>> LLEGINT I CREANT Epinions Ordenat........ " << endl;
			MatriuSparse m3(nomFitxerRels);

			cout << "Comment :=>> AFEGINT 7 a posicio (0,2) ............... " << endl;
			m3.setVal(0, 2, 7);
			cout << "Comment :=>> AFEGINT 8 a posicio (8,1) ............... " << endl;
			m3.setVal(8, 1, 8);
			cout << "Comment :=>> AFEGINT 9 a posicio (5,9) ............... " << endl;
			m3.setVal(5, 9, 9);
			cout << "Comment :=>> AFEGINT 11 a posicio (8677, 1768) ............... " << endl;
			m3.setVal(8677, 1768, 11);
			cout << "Comment :=>> ESCRIVINT Epinions Ordenat............... " << endl;

			ofstream fResEpiOrdenat;
			fResEpiOrdenat.open("ResEpiOrdenatCOORD.txt");
			if (fResEpiOrdenat.is_open())
			{
				fResEpiOrdenat << m3;
				fResEpiOrdenat.close();
			}

			if (IgualFitxers("ResEpiOrdenatCOORD.txt", "ResEpiOrdenatCOORDEsperat.txt", "ResCompEpiOrdenat.txt"))
			{
				cout << "Comment :=>> Epinions Ordenat BEN CONSTRUIT OK ....................... " << endl;
				grade++;
			}
			else
			{
				cout << "Comment :=>> ERROR CONSTRUCCIO Epinions Ordenat....................... " << endl;
				valid = false;
			}

			trobat = m3.getVal(75888, 0, valor);
			if (trobat)
			{
				cout << "Comment :=>>VAL(75888,0): " << valor << " . ERROR NO HAURIA D'EXISTIR" << endl;
				valid = false;
			}
			else
			{
				cout << "Comment :=>>VAL(75888,0): OK NO EXISTEIX" << endl;
				grade++;
			}

			trobat = m3.getVal(0, 2, valor);
			if (trobat)
				if (valor == 7)
				{
					cout << "Comment :=>> OK VAL(0,2): " << valor << endl;
					grade++;
				}
				else
				{
					cout << "Comment :=>> OK EXISTEIX VAL(0,2): Pero ERROR val " << valor << " Valor correcte 7" << endl;
					valid = false;
				}
			else
			{
				cout << "Comment :=>> ERROR VAL(0,2): NO EXISTEIX. HAURIA D'EXISTIR i VALOR=7" << endl;
				valid = false;
			}

			trobat = m3.getVal(8, 1, valor);
			if (trobat)
				if (valor == 8)
				{
					cout << "Comment :=>> OK VAL(8,1): " << valor << endl;
					grade++;
				}
				else
				{
					cout << "Comment :=>> OK EXISTEIX VAL(8,1): Pero ERROR val " << valor << " Valor correcte 8" << endl;
					valid = false;
				}
			else
			{
				cout << "Comment :=>> ERROR VAL(8,1): NO EXISTEIX. HAURIA D'EXISTIR i VALOR=8" << endl;
				valid = false;
			}
			trobat = m3.getVal(75877, 75876, valor);
			if (trobat)
				if (valor == 1)
				{
					cout << "Comment :=>> OK VAL(75877,75876): " << valor << endl;
					grade++;
				}
				else
				{
					cout << "Comment :=>> OK EXISTEIX VAL(75877,75876): Pero ERROR val " << valor << " Valor correcte 1" << endl;
					valid = false;
				}
			else
			{
				cout << "Comment :=>> ERROR VAL(75877,75876): NO EXISTEIX. HAURIA D'EXISTIR i VALOR=1" << endl;
				valid = false;
			}

			trobat = m3.getVal(75885, 16086, valor);
			if (trobat)
				if (valor == 1)
				{
					cout << "Comment :=>> OK VAL(75885,16086): " << valor << endl;
					grade++;
				}
				else
				{
					cout << "Comment :=>> OK EXISTEIX VAL(75885,16086): Pero ERROR val " << valor << " Valor correcte 1" << endl;
					valid = false;
				}
			else
			{
				cout << "Comment :=>> ERROR VAL(75885,16086): NO EXISTEIX. HAURIA D'EXISTIR i VALOR=1" << endl;
				valid = false;
			}

			trobat = m3.getVal(8677, 1768, valor);
			if (trobat)
				if (valor == 11)
				{
					cout << "Comment :=>> OK VAL(8677, 1768): " << valor << endl;
					grade++;
				}
				else
				{
					cout << "Comment :=>> OK EXISTEIX VAL(8677, 1768): Pero ERROR val " << valor << " Valor correcte 11" << endl;
					valid = false;
				}
			else
			{
				cout << "Comment :=>> ERROR VAL(8677, 1768): NO EXISTEIX. HAURIA D'EXISTIR i VALOR=11" << endl;
				valid = false;
			}

			cout << "Comment :=>> ==========================================" << endl;
			cout << "Comment :=>> VALIDANT * per float......................" << endl;
			MatriuSparse mProdEpiOrdenat = m3 * 6;

			ofstream fProdEpiOrdenat("MatProdEpiOrdenatCOORD.txt", ios::out);
			fProdEpiOrdenat << mProdEpiOrdenat;
			fProdEpiOrdenat.close();
			if (IgualFitxers("MatProdEpiOrdenatCOORD.txt", "MatProdEpiOrdenatCOORDEsperat.txt", "ResCompMatProdEpiOrdenat.txt"))
			{
				cout << "Comment :=>> Prod per 6 Epi Ordenat OK ....................... " << endl;
				grade++;
			}
			else
			{
				cout << "Comment :=>> ERROR Prod per 6 Epi Ordenat ....................... " << endl;
				valid = false;
			}

			cout << "Comment :=>> ==========================================" << endl;
			cout << "Comment :=>> VALIDANT / per float......................" << endl;
			MatriuSparse mDivEpiOrdenatProd = mProdEpiOrdenat / 3;
			ofstream fDivEpiOrdenat("MatDivEpiOrdenatCOORD.txt", ios::out);
			fDivEpiOrdenat << mDivEpiOrdenatProd;
			fDivEpiOrdenat.close();
			if (IgualFitxers("MatDivEpiOrdenatCOORD.txt", "MatDivEpiOrdenatCOORDEsperat.txt", "ResCompMatDivEpiOrdenat.txt"))
			{
				cout << "Comment :=>> (EpiOrdenat*6)/3 OK ....................... " << endl;
				grade++;
			}
			else
			{
				cout << "Comment :=>> ERROR (EpiOrdenat*6)/3 ....................... " << endl;
				valid = false;
			}
			cout << "Comment :=>> ==========================================" << endl;
			cout << "Comment :=>> VALIDANT * per vector....................." << endl;
			vector<float> vEpiOrdenat;
			vEpiOrdenat.resize(m3.getNFiles(), 1);

			vector<float> v2EpiOrdenat = m3*vEpiOrdenat;
			ofstream fProdVEpiOrdenat("VectProdEpiOrdenatCOORD.txt", ios::out);
			for (int i = 0; i < v2EpiOrdenat.size(); i++)
			{
				if (v2EpiOrdenat[i] != 0)
				{
					fProdVEpiOrdenat << "[POS: " << i << " ; VAL: " << v2EpiOrdenat[i] << " ] " << endl;
				}
			}
			fProdVEpiOrdenat.close();
			if (IgualFitxers("VectProdEpiOrdenatCOORD.txt", "VectProdEpiOrdenatCOORDEsperat.txt", "ResCompVectProdEpiOrdenat.txt"))
			{
				cout << "Comment :=>> (EpiOrdenat*vector) OK ....................... " << endl;
				grade++;
			}
			else
			{
				cout << "Comment :=>> ERROR (EpiOrdenat*vector) ....................... " << endl;
				valid = false;
			}

		if (valid)
			cout << "Comment :=>> Final del test sense errors" << endl;
		if (grade < 0)
			grade = 0;
		cout << endl << "Grade :=>> " << grade << endl;
		return 0;
	}
	catch (std::bad_alloc)
	{
		cout << "Comment ==> error allocatant memoria:" << endl;
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << '\n';
	}
	catch (const char* msg)
	{
		cout << msg << endl;
	}

}

//*/