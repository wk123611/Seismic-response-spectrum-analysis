#include "Main.h"
#include "iostream"
using namespace std;
//定义双精度二维数组==============================================================
double** TwoArrayDoubAlloc(int nRow, int nCol)
{
	double** pd;
	pd = new double*[nRow];      //申请行数
	if (!pd)
	{
		cout << "内存分配失败" << endl;
		exit(-1);
	}
	for (int j = 0; j < nRow; j++)
	{
		pd[j] = new double[nCol];          //申请列数
		if (!pd[j])
		{
			cout << "内存分配失败" << endl;
			exit(-1);
		}
	}
	return pd;
}
//定义单精度二维数组==============================================================
int** TwoArrayIntbAlloc(int nRow, int nCol)
{
	int** pi;
	pi = new int* [nRow];          //申请行数
	if (!pi)
	{
		cout << "内存分配失败" << endl;
		exit(-1);
	}
	for (int j = 0; j < nRow; j++)
	{
		pi[j] = new int[nCol];          //申请列数
		if (!pi[j])
		{
			cout << "内存分配失败" << endl;
			exit(-1);
		}
	}
	return pi;
}

//释放内存空间==============================================================
/*template<class T>
void TwoArrayFree(int nRow, T** pdi)
{
	for (int i = 0; i < nRow; i++)
	{
		delete[]pdi[i];   //回收列空间
		delete[]pdi;      //回收行空间
	}
}

//矩阵置零==============================================================
template<class T>
void MatrixZeroize(int nRow, int nCol, T** pT)
{
	for (int i = 0; i < nRow; i++)
	{
		for (int j = 0; j < nCol; j++)
		{
			pT[i][j] = 0;
		}
	}
}

//向量置零==============================================================
template<class T>
void VectorZeroize(int n, T* pT)
{
	for (int i = 0; i < n; i++)
		pT[i] = 0;
}
*/
//两矩阵相乘==============================================================
void MatrixMultiply(int nRow, int nCol, double** pA, double** pB, double** pC)
{
	double dTemp;
	for (int i = 0; i < nRow; i++)
	{
		for (int j = 0; j < nCol; j++)
		{
			dTemp = 0.0;
			for (int k = 0; k < nCol; k++)
			{
				dTemp += pA[i][k] * pB[k][j];
				pC[i][j] = dTemp;
			}
		}
	}
}

//矩阵左乘向量==============================================================
void MatrixVectorMultiply1(int nRow, int nCol, double** pA, double* pB, double* pC)
{
	double dTemp;
	for (int i = 0; i < nRow; i++)
	{
		dTemp = 0.0;
		for (int j = 0; j < nCol; j++)
		{
			dTemp += pA[i][j] * pB[j];
			pC[i] = dTemp;
		}
	}
}
//矩阵右乘向量
void MatrixVectorMultiply2(int nRow, int nCol, double** pA, double* pB, double* pC)
{
	double dTemp;
	for (int i = 0; i < nRow; i++)
	{
		dTemp = 0.0;
		for (int j = 0; j < nCol; j++)
		{
			dTemp += pA[j][i] * pB[j];
			pC[i] = dTemp;
		}
	}
}

//矩阵转置==============================================================
void MatrixTrans(int nRow, int nCol, double** pA, double** pAT)
{
	for (int i = 0; i < nRow; i++)
	{
		for (int j = 0; j < nCol; j++)
		{
			pAT[j][i] = pA[i][j];
		}
	}
}
