//组装总质量矩阵并压缩为一维数组

#include "Main.h"
#include "fstream"
#include "iostream"

using namespace std;


void GMAssembly(int nToallDOF, int nToallElem, Element* pElem, Node* pNode, Material* pMate, Section* pSect, int* pDiag, double* pGM)
{
	int i, j, m, iNode0, iNode1, GKi, GKj, GKij;
	double** pMe0 = TwoArrayDoubAlloc(4, 4);
	double** pMe1 = TwoArrayDoubAlloc(6, 6);
	int iaDOFIndex[6];
	ofstream fout1("ElemStiff.dat", ios::binary);
	if (!fout1)
	{
		cout << "单元刚度矩阵输出文件打开失败！" << endl;
		exit(-1);
	}
	for (m = 0; m < nToallElem; m++)
	{
// 		switch ((pElem + m)->iType)
// 		{
// 		case TRUSS:
// 			TrussElemMasscalcu(fout1, pElem + m, pMate, pSect, pMe0);
// 			iNode0 = (pElem + m)->iaNode[0];
// 			iNode1 = (pElem + m)->iaNode[1];
// 			for (i = 0; i < 2; i++)
// 			{
// 				iaDOFIndex[i] = (pNode + iNode0)->iaDOFIndex[i];
// 				iaDOFIndex[i + 2] = (pNode + iNode1)->iaDOFIndex[i];
// 			}
// 			for (i = 0; i < 4; i++)
// 			{
// 				GKi = iaDOFIndex[i];
// 				for (j = 0; j < 4; j++)
// 				{
// 					GKj = iaDOFIndex[j];
// 					if (GKi >= GKj)
// 					{
// 						GKij = pDiag[GKi] - GKi + GKj;
// 						pGM[GKij] += pMe0[i][j];
// 					}
// 				}
// 			}
/*			break;*/
// 		case FRAME:
			FrameElemMassCalcu(fout1, pElem + m, pMate, pSect, pMe1);
			iNode0 = (pElem + m)->iaNode[0];
			iNode1 = (pElem + m)->iaNode[1];
			for (i = 0; i < 3; i++)
			{
				iaDOFIndex[i] = (pNode + iNode0)->iaDOFIndex[i];
				iaDOFIndex[i + 3] = (pNode + iNode1)->iaDOFIndex[i];
			}
			for (i = 0; i < 6; i++)
			{
				GKi = iaDOFIndex[i];
				for (j = 0; j < 6; j++)
				{
					GKj = iaDOFIndex[j];
					if (GKi >= GKj)
					{
						GKij = pDiag[GKi] - GKi + GKj;
						pGM[GKij]= pMe1[i][j]+ pGM[GKij];
					}
				}
			}
		}
	}
