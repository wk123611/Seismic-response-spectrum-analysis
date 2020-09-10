#include "Main.h"

//自由节点自由度编号程序


int EffectiveDOF(int& iBuf0, int nTotalNode, int nConstrainedNode, ConstrainedNode* pConsNode, Node* pNode)
{
	int i, j;
	int iBuf;     //总自由度数
	for (i = 0; i < nTotalNode; i++)
		for (j = 0; j < 3; j++)
			(pNode + i)->iaDOFIndex[j] = 0;   //将各节点自由度编号置零
	for (i = 0; i < nConstrainedNode; i++)
	{
		iBuf = (pConsNode + i)->iNode;			 //受约束节点号
		for (j = 0; j < 3; j++)		           //受约束节点号置特征数
			pNode[iBuf].iaDOFIndex[j] = (pConsNode + i)->iaConstrainedDOF[j];
	}
	iBuf = 0;
	for (i = 0; i < nTotalNode; i++)
	{
		if ((pNode + i)->iType == FRAME_NODE)
		{
			for (j = 0; j < 3; j++)         //对刚架节点未知自由度编号
			{
				if ((pNode + i)->iaDOFIndex[j] == 0)
					(pNode + i)->iaDOFIndex[j] = iBuf++;
			}
		}
		else
		{
			for (j = 0; j < 2; j++)               //对桁架节点未知自由度编号
			{
				if ((pNode + i)->iaDOFIndex[j] == 0)
					(pNode + i)->iaDOFIndex[j] = iBuf++;
			}
		}
	}	
	return iBuf;
}
