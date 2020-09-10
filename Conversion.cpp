//按特征值大小顺序转换频率，周期，振型矩阵，地震力，振型质量，振型参与质量比例等等。
#include "Main.h"
//	将自振频率、周期、振型排序
void Conversion(int EffectiveDOF1, double* E, double* T, double* ZXCYZLx,
	double* ZXCYZLxB, double** X, double** Fx)
{
	int N = EffectiveDOF1 - 1;
	int I, J, Q;
	for (I = 1; I <= N; I++)
	{
		for (J = I - 1; J >= 0; J--)
		{
			if (E[J + 1] < E[J])
			{
				//频率、周期、振型排序
				double tempE = E[J];
				E[J] = E[J + 1];
				E[J + 1] = tempE;
				double tempT = T[J];
				T[J] = T[J + 1];
				T[J + 1] = tempT;
				for (Q = 0; Q <= N; Q++)
				{
					double tempX = X[Q][J];
					X[Q][J] = X[Q][J + 1];
					X[Q][J + 1] = tempX;
				}
			}
		}
	}
	
		for (I = 1; I <= EffectiveDOF1; I++)
		{
			for (J = I - 1; J >= 0; J--)
			{
				if (ZXCYZLx[J] < ZXCYZLx[J + 1])
				{
					//振型参与质量和振型参与质量比例排序
					double tempOx = ZXCYZLx[J];
					ZXCYZLx[J] = ZXCYZLx[J + 1];
					ZXCYZLx[J + 1] = tempOx;
					double tempMPBx = ZXCYZLxB[J];
					ZXCYZLxB[J] = ZXCYZLxB[J + 1];
					ZXCYZLxB[J + 1] = tempMPBx;
					for (Q = 0; Q < EffectiveDOF1 / 3; Q++)
					{
						//地震力排序
						double tempFx = Fx[Q][J];
						Fx[Q][J] = Fx[Q][J + 1];
						Fx[Q][J + 1] = tempFx;
					}
				}
			}
		}
	}