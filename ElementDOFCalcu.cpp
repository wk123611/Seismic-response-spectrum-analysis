
//对节点自由度编号以后，根据单元两端的节点号就可以形成单元定位向量。
//单元定位向量存放在二维数组中，以备后面计算各行半带宽和对对角元地址时使用。

#include "Main.h"


void ElementDOFCalcu(int nTotalElem, Node* pNode, Element* pElem, int** pElemDOF)
{
	int iNode0, iNode1;
	int i, j;
	for (i = 0; i < nTotalElem; i++)
	{
		iNode0 = (pElem + i)->iaNode[0];
		iNode1 = (pElem + i)->iaNode[1];
		if ((pElem + i)->iType == TRUSS)   //对桁架单元
		{
			for (j = 0; j < 2; j++)
			{
				pElemDOF[i][j] = (pNode + iNode0)->iaDOFIndex[j];
				pElemDOF[i][j+2] = (pNode + iNode1)->iaDOFIndex[j];
			}
		}
		else
		{
			for (j = 0; j < 3; j++)   //对刚架单元
			{
				pElemDOF[i][j] = (pNode + iNode0)->iaDOFIndex[j];
				pElemDOF[i][j + 3] = (pNode + iNode1)->iaDOFIndex[j];
			}
		}
	}
}
