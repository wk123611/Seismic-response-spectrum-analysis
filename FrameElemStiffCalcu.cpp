
#include "Main.h"
#include "iostream"
#include "fstream"
using namespace std;
//计算刚架单元刚度矩阵

void FrameElemStiffCalcu(ofstream&fout1, Element* pElem, Material* pMate, Section* pSect, double** pKe)
{
	double dBuf, dLength;
	double dE;
	double dA, dIz, dSin, dCos;
	int iSectType, iMateType;
	double** pT = TwoArrayDoubAlloc(6, 6);
	double** pTT = TwoArrayDoubAlloc(6, 6);
	double** pTemp = TwoArrayDoubAlloc(6, 6);

	iSectType = pElem->iSection;
	iMateType = pElem->iMaterial;
	dA = (pSect + iSectType)->dA;
	dIz = (pSect + iSectType)->dIz;
	dE = (pMate + iMateType)->dE;
	dLength = pElem->dLength;

	MatrixZeroize(6, 6, pKe);
	dBuf = dE*dA / dLength;
	pKe[0][0] = pKe[3][3] = dBuf;
	pKe[0][3] = pKe[3][0] = -dBuf;
	dBuf = dLength*dLength*dLength;
	dBuf = 12.0*dE*dIz / dBuf;
	pKe[1][1] = pKe[4][4] = dBuf;
	pKe[1][4] = pKe[4][1] = -dBuf;
	dBuf = dLength*dLength;
	dBuf = 6.0*dE*dIz / dBuf;
	pKe[1][2] = pKe[2][1] = pKe[1][5] = pKe[5][1] = dBuf;
	pKe[2][4] = pKe[4][2] = pKe[4][5] = pKe[5][4] = -dBuf;
	dBuf = 4.0*dE*dIz / dLength;
	pKe[2][2] = pKe[5][5] = dBuf;
	pKe[5][2] = pKe[2][5] = dBuf / 2.0;

 	
	//将局部坐标系单刚记盘转换到整体坐标系
	dSin = pElem->dSin;
	dCos = pElem->dCos;
	MatrixZeroize(6, 6, pT);
	pT[2][2] = pT[5][5] = 1.0;
	pT[0][0] = pT[1][1] = pT[3][3] = pT[4][4] = dCos;
	pT[0][1] = pT[3][4] = dSin;
	pT[1][0] = pT[4][3] = -dSin;

	MatrixTrans(6, 6, pT, pTT);
	MatrixMultiply(6, 6, pTT, pKe, pTemp);
	MatrixMultiply(6, 6, pTemp, pT, pKe);   //书上是否写错了

	TwoArrayFree(6, pT);
	TwoArrayFree(6, pTT);
	TwoArrayFree(6, pTemp);
 	


}