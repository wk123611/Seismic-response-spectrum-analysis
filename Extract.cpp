#include "Main.h"
//计算振型质量和比例
//提取单自由度方向结果    

void ExtractX(int EffectiveDOF1,double** SN,double** SNx,double* ZXCYZLx,
	double* ZXCYZLB,double** F,double** Fx)
{
	double SumZXCYZLx=0;  //x方向参与质量之和
	for (int i = 0; i < EffectiveDOF1; i=i+3)
	{	
		for (int j = 0; j < EffectiveDOF1; j++)
		{
			SNx[i / 3][j] = SN[i][j];
			Fx[i / 3][j] = F[i][j];
		}
	}

	for (int i = 0; i < EffectiveDOF1; i++)
	{
		for (int j = 0; j < EffectiveDOF1 / 3; j++)
		{
			ZXCYZLx[i] = SNx[j][i] + ZXCYZLx[i];
		}
	}

	for (int i = 0; i<EffectiveDOF1; i++)
	{
	
		SumZXCYZLx += ZXCYZLx[i];            //计算x方向参与总质量
	}
	//计算振型参与质量比例
	for (int i = 0; i < EffectiveDOF1; i++)	
	{	
		ZXCYZLB[i] = ZXCYZLx[i] * 100 / SumZXCYZLx;
	}
}