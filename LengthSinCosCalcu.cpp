#include "Main.h"
#include "math.h"
//计算杆单元长度和方向余弦
void LengthSinCosCalcu(int nTotalElem, Element* pElem, Node* pNode)
{
	int i;
	int iNode0, iNode1;
	double dDeltaX, dDeltaY;
	double dX0, dY0, dX1, dY1;
	for (i = 0; i < nTotalElem; i++)
	{
		iNode0 = (pElem + i)->iaNode[0];
		iNode1 = (pElem + i)->iaNode[1];
		dX0 = (pNode + iNode0)->dX;
		dY0 = (pNode + iNode0)->dY;
		dX1 = (pNode + iNode1)->dX;
		dY1 = (pNode + iNode1)->dY;
		dDeltaX = dX1 - dX0;
		dDeltaY = dY1 - dY0;
		(pElem + i)->dLength = sqrt(dDeltaX*dDeltaX + dDeltaY*dDeltaY);
		(pElem + i)->dSin = dDeltaY / (pElem + i)->dLength;
		(pElem + i)->dCos = dDeltaX / (pElem + i)->dLength;
	}
}