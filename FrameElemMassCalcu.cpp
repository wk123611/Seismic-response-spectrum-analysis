
#include "Main.h"
#include "iostream"
#include "fstream"
using namespace std;
//计算刚架单元质量矩阵

void FrameElemMassCalcu(ofstream&fout1, Element* pElem, Material* pMate, Section* pSect, double** pMe)
{
	double RAL, dLength;   //RAL=密度*截面积*长度
	double dE;//弹性模量
	double dr;//密度
	double dA, dIz, dSin, dCos;  //面积、惯性矩
	int iSectType, iMateType;
	double** pT = TwoArrayDoubAlloc(6, 6);
	double** pTT = TwoArrayDoubAlloc(6, 6);
	double** pTemp = TwoArrayDoubAlloc(6, 6);

	iSectType = pElem->iSection;
	iMateType = pElem->iMaterial;
	dA = (pSect + iSectType)->dA;
	dIz = (pSect + iSectType)->dIz;
	dE = (pMate + iMateType)->dE;
	dr = (pMate + iMateType)->dr;
	dLength = pElem->dLength;

	MatrixZeroize(6, 6, pMe);

	RAL = dr*dA*dLength;

	pMe[0][0] = pMe[3][3] = RAL / 3;
	pMe[3][0] = pMe[0][3] = RAL / 6;
	pMe[1][1] = pMe[4][4] = 13 * RAL / 35 + 6 * dIz*dr / (5 * dLength);
	pMe[2][1] = pMe[1][2] = 11 * dLength*RAL / 210 + dIz*dr / 10;
	pMe[2][2] = pMe[5][5] = dLength*dLength*RAL / 105 + 2 * dIz *dr*dLength / 15;
	pMe[4][1] = pMe[1][4] = 9 * RAL / 70 - 6 * dIz*dr / (5 * dLength);
	pMe[4][2] = pMe[2][4] = 13 * dLength*RAL / 420 - dIz*dr / 10;
	pMe[5][1] = pMe[1][5] = -13 * dLength*RAL / 420 + dIz*dr / 10;
	pMe[5][2] = pMe[2][5] = -dLength*dLength*RAL / 140 - dIz*dr*dLength / 30;
	pMe[5][4] = pMe[4][5] = -11 * dLength*RAL / 210 - dIz*dr / 10;



	//将局部坐标系单刚记盘转换到整体坐标系
	dSin = pElem->dSin;
	dCos = pElem->dCos;
	MatrixZeroize(6, 6, pT);
	pT[2][2] = pT[5][5] = 1.0;
	pT[0][0] = pT[1][1] = pT[3][3] = pT[4][4] = dCos;
	pT[0][1] = pT[3][4] = dSin;
	pT[1][0] = pT[4][3] = -dSin;


	MatrixTrans(6, 6, pT, pTT);
	MatrixMultiply(6, 6, pTT, pMe, pTemp);
	MatrixMultiply(6, 6, pTemp, pT, pMe);

	TwoArrayFree(6, pT);
	TwoArrayFree(6, pTT);
	TwoArrayFree(6, pTemp);




// 	ofstream mycout("Me");
// 	for (int i = 0; i < 6; i++)
// 	{
// 		for (int j = 0; j < 6; j++)
// 		{
// 			mycout << pMe[i][j] << "     ";
// 		}
// 		mycout << endl;


	
}