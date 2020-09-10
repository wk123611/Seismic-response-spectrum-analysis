#include "fstream"
#include "Main.h"
using namespace std;
//计算桁架单元刚度矩阵

void TrussElemStiffcalcu(ofstream&fout1, Element* pElem, Material* pMate, Section * pSect, double** pKe)
{
	int i, j;
	double dBuf, dBuf1;
	double dE, dA, dLength, dSin, dCos;
	int iSectType, iMateType;
	iSectType = pElem->iSection;
	iMateType = pElem->iMaterial;
	dA = (pSect + iSectType)->dA;
	dE = (pMate + iMateType)->dE;
	dLength = pElem->dLength;

	MatrixZeroize(4, 4, pKe);

	dBuf = dE*dA / dLength;

	pKe[0][0] = pKe[2][2] = dBuf;
	pKe[0][2] = pKe[2][0] = -dBuf;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			fout1.write((char*)&pKe[i][j], sizeof(double));
	//将局部坐标系单刚记盘转换到整体坐标系
	dSin = pElem->dSin;
	dCos = pElem->dCos;
	dBuf1 = dCos*dCos*dBuf;
	pKe[0][0] = pKe[2][2] = dBuf1;
	pKe[0][2] = pKe[2][0] = -dBuf1;
	dBuf1 = dSin*dSin*dBuf;
	pKe[1][1] = pKe[3][3] = dBuf1;
	pKe[1][3] = pKe[3][1] = -dBuf1;
	dBuf1 = dCos*dSin*dBuf;
	pKe[1][0] = pKe[0][1] = pKe[2][3] = pKe[3][2] = dBuf1;
	pKe[0][3] = pKe[3][0] = pKe[1][2] = pKe[2][1] = -dBuf1;


}