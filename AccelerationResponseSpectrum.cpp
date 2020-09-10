//按照公路桥梁抗震设计规范JTG T2231 - 01 - 2020反应谱计算地震力

#include "Main.h"
using namespace std;


void AccelerationResponseSpectrum(int EffectiveDOF1,double* T,double** FF,double** SN,ARS* pARS)
{
	int I;
	int J;
	int N = EffectiveDOF1 - 1;
	//公路桥梁抗震设计规范JTG T2231 - 01 - 2020反应谱计算//////////////////
	//输入：1、场地类别 2、特征周期Tg 3、抗震设防烈度 4、水平or竖向 5、Ci、Cd、T0
	//输出：Cs、A、Smax、S（T）
	// --------------------------------------------------------------------------------------------------
	int cdlx ;    ////////////////////////场地类型有0~4五种
	char ld;    ////////////////////////抗震设防烈度有6~9四种	
					  //A=0.05,B=0.1,C=0.15,D=0.2,E=0.3,F=0.4
	char Tg1 ;  //////////////////////按《中国地震动参数区划表》查取
					 //A=0.35,B=0.4,C=0.45				
	double A ; //基本地震峰值加速度
//	double lx ;   //////////////////////////水平方向类型为lx = 1，竖向类型为lx = 2
	double Ci ; //抗震重要性系数，按规范3.1.3 - 2取值
	double Cd ; //阻尼调整系数,按5.2.4取值暂取1。
	double T0 ; //反应谱上升段的最大周期，取0.1。
	double Tg;//特征周期？0.35\0.40\0.45
	double Cs;//   场地系数
	double Cz;//综合影响系数（考虑允许结构发生塑性变形对地震荷载的折减）
	double Smax;
	vector <double> S(EffectiveDOF1);    //抗震反应谱计算得到的加速度

	cdlx = pARS->cdlx;
	ld = pARS->ld;
	Tg1 = pARS->Tg1;
	Ci = pARS->Ci;
	Cd = pARS->Cd;
	T0 = pARS->T0;
	Cz = pARS->Cz;
// 	cout << cdlx << endl;
// 	cout << ld << endl;
// 	cout << Tg1 << endl;
// 	cout << A << endl;
// 	cout << Ci << endl;
// 	cout << Cd << endl;
// 	cout << T0 << endl;
// 	cout << Cz << endl;
	///////////////////////////////////////////////////////////////////////////////////////////
	switch (Tg1)
	{
	case 'A':
		switch (cdlx) {
		case 0: Tg = 0.2;	break;
		case 1: Tg = 0.25;	break;
		case 2: Tg = 0.35;	break;
		case 3: Tg = 0.45;	break;
		case 4: Tg = 0.65;	break;
		}
		break;
	case 'B':
		switch (cdlx) {
		case 0: Tg = 0.25;	break;
		case 1: Tg = 0.30;	break;
		case 2: Tg = 0.40;	break;
		case 3: Tg = 0.55;	break;
		case 4: Tg = 0.75;	break;
		}
		break;
	case 'C':
		switch (cdlx) {
	case 0: Tg = 0.30;	break;
	case 1: Tg = 0.35;	break;
	case 2: Tg = 0.45;	break;
	case 3: Tg = 0.65;	break;
	case 4: Tg = 0.90;	break;
	}
			 break;
	}
	switch (ld)
	{
	case 'A':
		A = 0.05;
		switch (cdlx) {
		case 0: Cs = 0.72;	break;
		case 1: Cs = 0.80;	break;
		case 2: Cs = 1.00;	break;
		case 3: Cs = 1.30;	break;
		case 4: Cs = 1.25;	break;
		}
		break;
	case 'B':
		A = 0.1;
		switch (cdlx) {
		case 0: Cs = 0.74;	break;
		case 1: Cs = 0.82;	break;
		case 2: Cs = 1.00;	break;
		case 3: Cs = 1.25;	break;
		case 4: Cs = 1.20;	break;
		}
		break;
	case 'C':
		A = 0.15;
		switch (cdlx) {
		case 0: Cs = 0.75;	break;
		case 1: Cs = 0.83;	break;
		case 2: Cs = 1.00;	break;
		case 3: Cs = 1.15;	break;
		case 4: Cs = 1.10;	break;
		}
		break;
	case 'D':
		A = 0.2;
		switch (cdlx) {
		case 0: Cs = 0.76;	break;
		case 1: Cs = 0.85;	break;
		case 2: Cs = 1.00;	break;
		case 3: Cs = 1.00;	break;
		case 4: Cs = 1.00;	break;
		}
		break;
	case 'E':
		A = 0.3;
		switch (cdlx) {
		case 0: Cs = 0.85;	break;
		case 1: Cs = 0.95;	break;
		case 2: Cs = 1.00;	break;
		case 3: Cs = 1.00;	break;
		case 4: Cs = 0.95;	break;
		}
		break;
	case 'F':
		A = 0.4;
		switch (cdlx) {
		case 0: Cs = 0.90;	break;
		case 1: Cs = 1.00;	break;
		case 2: Cs = 1.00;	break;
		case 3: Cs = 1.00;	break;
		case 4: Cs = 0.90;	break;
		}
		break;
	}
	//带入参数计算地震等效加速度
	for (I = 0; I <= N; I++)
	{
		Smax = 2.5*Ci*Cs*Cd*A;
		if (T[I] < T0)
		{
			S[I] = Smax*(0.6*T[I] / T0 + 0.4);
		}
		else if (T[I] < Tg)
		{
			S[I] = Smax;
		}
		else
		{
			S[I] = Smax*Tg / T[I];
		}	
	}


	//计算各节点地震力
 	for (I = 0; I <= N; I++)
	{
		for (J = 0; J <= N; J++)
		{
			FF[J][I] = Cz * S[I] *SN[J][I]*9.8/1000;
		}
	}

}
