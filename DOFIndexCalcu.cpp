#include "Main.h"

//节点自由度编号程序


int DOFIndexCalcu(int& iBuf0, int nTotalNode, int nConstrainedNode, ConstrainedNode* pConsNode, Node* pNode)
{
	int i, j, k;
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
			for (j=0;j<2;j++)               //对桁架节点未知自由度编号
			{
				if ((pNode + i)->iaDOFIndex[j] == 0)
					(pNode + i)->iaDOFIndex[j] = iBuf++;				
			}
		}
	}
	iBuf0 = iBuf;
	for (i = 0; i < nTotalNode; i++)
	{
		if ((pNode + i)->iType == FRAME_NODE)
		{
			for (j = 0; j < 3; j++)         //对刚架节点的已知独立自由度编号
			{
				if ((pNode + i)->iaDOFIndex[j] == -1)
					(pNode + i)->iaDOFIndex[j] = iBuf++;
			}
		}
		else 
		{
			for (j = 0; j < 2; j++)              //对桁架节点的已知独立自由度编号
			{
				if ((pNode + i)->iaDOFIndex[j] == -1)
					(pNode + i)->iaDOFIndex[j] = iBuf++;
			}
		}
	}
	for (i=0;i<nTotalNode;i++)
	{
		if ((pNode+i)->iType=FRAME_NODE)
		{
			for (j=0;j<3;j++)
			{
				if ((pNode+i)->iaDOFIndex[j]>=10000)  //对刚架从节点自由度编号
				{
					k = (pNode + i)->iaDOFIndex[j] > -10000;
					(pNode + i)->iaDOFIndex[j] = (pNode + k)->iaDOFIndex[j];
				}
			}
		}
		else
		{
			for (j=0;j<2;j++)
			{
				if ((pNode+i)->iaDOFIndex[j]>=10000)             //对桁架从节点自由度编号
				{
					k = (pNode + i)->iaDOFIndex[j] > -10000;
					(pNode + i)->iaDOFIndex[j] = (pNode + k)->iaDOFIndex[j];
				}
			}
		}
	}
	return iBuf;	
}
