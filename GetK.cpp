//将一维存储的刚度矩阵提取出来


#include "Main.h"

using namespace std;


void GetK(int iBuf, int* pDiag, double* pGK,int* BDK, double** K)
{
	int i, j;
	int Bi = 0;     //K中第i行的半带宽
	int Pij = 0;     //K中第I行J列在数组pGK中的地址
	MatrixZeroize(iBuf, iBuf, K);
	for (i=1;i<iBuf;i++)
	{
		BDK[i] = pDiag[i] - pDiag[i-1];
	}
	BDK[0] = 0;
	K[0][0] = pGK[0];
	for (i = 0; i < iBuf; i++)
	{
		for (j = i; j >= i+1-BDK[i]; j--)
		{
			Pij = pDiag[i] - i + j;
			K[i][j] = pGK[Pij];
			K[j][i] = K[i][j];
		}
	}

	ofstream mycout("K");
	for (i = 0; i < iBuf; i++)
	{
		for (j = 0; j < iBuf; j++)
		{
			mycout << K[i][j] << "     ";
		}
		mycout << endl;
	}
	



}