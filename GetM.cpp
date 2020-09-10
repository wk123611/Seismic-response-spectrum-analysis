////将一维存储的质量矩阵提取出来

#include "Main.h"

using namespace std;


void GetM(int iBuf, int* pDiag, double* pGM,int* BDK,double** M, double** Msave)
{
	int i, j;
	int Bi = 0;     //M中第i行的半带宽
	int Pij = 0;     //M中第I行J列在数组pGK中的地址
					 /*	double** M = TwoArrayDoubAlloc(nTotalDOF, nTotalDOF);*/
	MatrixZeroize(iBuf, iBuf, M);
	MatrixZeroize(iBuf, iBuf, Msave);
	for (i = 1; i < iBuf; i++)
	{
		BDK[i] = pDiag[i] - pDiag[i - 1];
	}
	BDK[0] = 0;
	M[0][0] = pGM[0];   
	for (i = 0; i < iBuf; i++)
	{
		for (j = i; j >= i + 1 - BDK[i]; j--)
		{
			Pij = pDiag[i] - i + j;
			M[i][j] = pGM[Pij];
			M[j][i] = M[i][j];
		}
	}
	for (i = 0; i < iBuf; i++)
	{
		for (j = 0; j < iBuf; j++)
		{
			Msave[i][j] = M[i][j];
		}
	}


	ofstream mycout("M");
	for (i = 0; i < iBuf; i++)
	{
		for (j = 0; j < iBuf; j++)
		{
			mycout << M[i][j] << "     ";
		}
		mycout << endl;
	}
}