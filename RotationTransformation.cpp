#include "Main.h"

//********************初始化旋转变化矩阵和开始迭代*******************

void RotationTransformation(double** M,double** K,int EffectiveDOF1, double* E, double* T, 
	double**X,double**Msave, double**SN,double*ZXCYXS)
{
	int NSWEEP = 0;
	double RTOL = pow(0.1, 12);//指定精度
	int NSMAX = 40000;//最大允许扫描次数
	int ND = EffectiveDOF1;//质量、刚度矩阵维数(N DIMENSION)
	int N = ND - 1;//数组最大标号
	int NR = N - 1;//数组元素检查时用标号
	double EPS = 0;//判断是否需要归零元素的参数
	double CA = 0; //旋转参数a
	double CG = 0; //旋转参数b
	int JJ,L,J,I;  //计数参数
	double* D = new double[ND];     
	double* EIGV = new double[ND];    //特征值
 	double** XT = TwoArrayDoubAlloc(ND,ND);      //振型矩阵X的转置
	double* ZLXL = new double[ND];      //质量向量					
	VectorZeroize(ND, D);
	VectorZeroize(ND, EIGV);
	VectorZeroize(ND, ZLXL);
 	MatrixZeroize(ND,ND,XT);
//初始化矩阵和向量----------------------------------------------------------------
	for (I = 0; I <= N; I++)
		for (J = 0; J <= N; J++)
		{
			X[I][J] = 0;
			X[I][I] = 1;
		}
	for (J = 0; J <= N; J++)
	{
		D[J] = 0.0;
	}
	for (I = 0; I <= N; I++)
	{
		if (K[I][I] > 0 && M[I][I] > 0)
		{
			D[I] = K[I][I] / M[I][I];
			EIGV[I] = D[I];
			E[I] = sqrt(EIGV[I]);
			T[I] = 2 * 3.1415926 / E[I];
		}
	}
	//计算质量向量
	for (I = 0; I <= N; I++)
	{
		for (J = 0; J <= N; J++)
		{
			ZLXL[I] = M[I][J] + ZLXL[I];
			
		}
	}

//**CHEACK IF PRESENT OFF - DIAGONAL  ELEMENT IS LARGR ENOUGH TO REQUIRE ZEROIN

part0:
	NSWEEP = NSWEEP + 1;
	EPS = pow(0.01, NSWEEP * 3);
	for (J = 0; J <= NR; J++)
	{
		JJ = J + 1;
		for (L = JJ; L <= N; L++)
		{
			double EPTOLK = (K[J][L] * K[J][L] / (K[J][J] * K[L][L]));  //被检查元素K[J,L]
			double EPTOLM = (M[J][L] * M[J][L] / (M[J][J] * M[L][L]));  //被检查元素M[J,L]
			if (EPTOLK >= EPS || EPTOLM >= EPS)
			{
				//检查未通过进行旋转变换，定义旋转参数CA、CG
				double KLL = K[L][L] * M[J][L] - M[L][L] * K[J][L];
				double KJJ = K[J][J] * M[J][L] - M[J][J] * K[J][L];
				double KM = K[J][J] * M[L][L] - M[J][J] * K[L][L];
				double CHECK = (KM*KM + 4 * KLL*KJJ) / 4;
				if (CHECK < 0)
				{
					cout << "check wrong" << endl;
				}
				else {
					double D1 = KM / 2 + sqrt(CHECK);
					double D2 = KM / 2 - sqrt(CHECK);
					double DEN = D1;
					if (abs(D2) > abs(D1))
					{
						DEN = D2;
					}
					if (DEN == 0)
					{
						CA = 0;
						CG = -K[J][L] / K[L][L];
					}
					else {
						CA = KLL / DEN;
						CG = -KJJ / DEN;
						//旋转参数CA、CG定义完成
					}
				}
				///////////
				int L1 = L + 1;
				int L2 = L - 1;
				int J1 = J + 1;
				int J2 = J - 1;

				if (J > 0)
				{
					goto part1;
				}
				else
				{
					goto part2;
				}
				//PART1
			part1:
				for (int I = 0; I < J; I++)
				{
					double KJ = K[I][J];
					double MJ = M[I][J];
					double KL = K[I][L];
					double ML = M[I][L];
					K[I][J] = KJ + CG*KL;
					M[I][J] = MJ + CG*ML;
					K[I][L] = KL + CA*KJ;
					M[I][L] = ML + CA*MJ;
				}
				//PART2:
			part2:
				int JUDGE = L - N + 1;
				if (JUDGE <= 0)
				{
					for (int I = L1; I <= N; I++)
					{
						double KJ = K[J][I];
						double MJ = M[J][I];
						double KL = K[L][I];
						double ML = M[L][I];
						K[J][I] = KJ + CG*KL;
						M[J][I] = MJ + CG*ML;
						K[L][I] = KL + CA*KJ;
						M[L][I] = ML + CA*MJ;
					}
				}
				else
				{
					goto part3;

				}
				//PART3:
			part3:
				if (J1 - L2 <= 0)
				{
					for (int I = J1; I <= L2; I++)
					{
						double KJ = K[J][I];
						double MJ = M[J][I];
						double KL = K[I][L];
						double ML = M[I][L];
						K[J][I] = KJ + CG*KL;
						M[J][I] = MJ + CG*ML;
						K[I][L] = KL + CA*KJ;
						M[I][L] = ML + CA*MJ;
					}
				}
				else {
					goto part4;
				}
				//PART4:
			part4:
				double KL = K[L][L];
				double ML = M[L][L];
				K[L][L] = KL + 2 * CA*K[J][L] + CA*CA*K[J][J];
				M[L][L] = ML + 2 * CA*M[J][L] + CA*CA*M[J][J];
				K[J][J] = K[J][J] + 2 * CG*K[J][L] + CG*CG*KL;
				M[J][J] = M[J][J] + 2 * CG*M[J][L] + CG*CG*ML;
				K[J][L] = 0;
				M[J][L] = 0;
				//旋转完成后更新X矩阵
				for (int I = 0; I <= N; I++)
				{
					double XJ = X[I][J];
					double XL = X[I][L];
					X[I][J] = XJ + CG*XL;
					X[I][L] = XL + CA*XJ;
				}
			}
		}
	}

	//旋转完成后更新特征值数组
	for (int I = 0; I <= N; I++)
	{
 		if (K[I][I] > 0 && M[I][I] > 0)
		{
			EIGV[I] = K[I][I] / M[I][I];	
		}
 		else
		{
 			cout << EIGV[I] << endl;
			cout << "eigv wrong...STOP" << endl;
 		}
	}
	//判断是否收敛
	for (int I = 0; I <= N; I++)
	{
		double TOL = RTOL*D[I];
		double DIF = abs(EIGV[I] - D[I]);
		if (DIF > TOL)
		{
			for (int I = 0; I <= N; I++)
			{
				D[I] = EIGV[I];
			}
			if (NSWEEP < NSMAX)
			{
				goto part0;
			}
			else
			{
				cout << "计算失败，扫描次数达到最大" << endl;
				/*break;*/
			}
		}
		else
		{
			//检查非对角线元素是否需要再次归零
			EPS = pow(RTOL, 2);
			for (int J = 0; J <= NR; J++)
			{
				JJ = J + 1;
				for (L = JJ; L <= N; L++)
				{
					double EPSK = K[J][L] * K[J][L] / (K[J][J] * K[L][L]);
					double EPSM = M[J][L] * M[J][L] / (M[J][J] * M[L][L]);
					if (EPSK < EPS&&EPSM < EPS)
					{
						//修改矩阵下三角元素，
						for (int I = 0; I <= N; I++);
						{
							for (int J = 0; J <= N; J++)
							{
								K[J][I] = K[I][J];
								M[J][I] = M[I][J];
							}
						}
					}
					else {
						for (int I = 0; I <= N; I++)
						{
							D[I] = EIGV[I];
							if (NSWEEP < NSMAX)
							{
								goto part0;
							}
							else {
								cout << "计算失败，扫描次数达到最大" << endl;
								break;
							}
						}
					}
				}
			}
		}
	}



	for (int I = 0; I <= N; I++)
	{
		double MM = sqrt(M[I][I]);
		E[I] = sqrt(EIGV[I]);
		T[I] = 2 * 3.1415926 / E[I];
		for (int J = 0; J <= N; J++)
		{
			X[J][I] = X[J][I] / MM;
		}
	}

	MatrixTrans(ND, ND, X, XT);  //振型转置
	MatrixVectorMultiply1(ND, ND, XT, ZLXL, ZXCYXS);  //计算振型参与系数
	MatrixMultiply(ND, ND, Msave, X, SN);   //M*X

	for (I=0;I<ND;I++)
	{
		for (J=0;J<ND;J++)
		{
			SN[J][I] = SN[J][I] * ZXCYXS[I];
		}	
	}
}



				


