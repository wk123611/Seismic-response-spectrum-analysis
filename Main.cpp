
#include "iostream"
#include "Main.h"
#include "fstream"
#include "stdlib.h"  //
#include "stdio.h"
#include "math.h"

using namespace std;

int main() {



	int nTotalNode;                //节点总数
	int nConstrainedNode;           //受约束节点总数
	int nTotalElem;                 //单元总数
	int EffectiveDOF1;              //自由节点自由度数（划行划列后矩阵维数）
	int EffectiveDOF2;
	int nMaterialType;             //材料种类数
	int nSectionType;             //截面集合特性种类数
	int nTotalDOF;               //总自由度数
	int nFreeDOF;                //独立自由度数

	int i;                        //循环控制变量
	int iBuf;


	ifstream fin0("INPUT.txt");
	if (!fin0)
	{
		cout << "原始数据文件打开失败" << endl;
		exit(-1);
	}
	ofstream fout0("Results.dat");
	if (!fout0)
	{
		cout << "计算结构输出文件打开失败" << endl;
		exit(-1);
	}

	fin0 >> nTotalNode >> nConstrainedNode >> nTotalElem >> nMaterialType >> nSectionType; //输入 >> nLoad
	//内存分配----------------------------------------------------------------------------------------
	Node* pNode = new Node[nTotalNode];
	ConstrainedNode* pConsNode = new ConstrainedNode[nConstrainedNode];
	Element* pElem = new Element[nTotalElem];
	Material* pMate = new Material[nMaterialType];
	Section* pSect = new Section[nSectionType];
	ARS* pARS = new ARS[1];
	int** pElemDOF = TwoArrayIntbAlloc(nTotalElem, 6);
	//---------------------------------------------------------------------------------------------
	//读入结构描述数据
	for (i = 0; i < nTotalNode; i++)     //读入节点数据
		fin0 >> (pNode + i)->iType >> (pNode + i)->dX >> (pNode + i)->dY;
	for (i = 0; i < nConstrainedNode; i++)  //读入受约束节点数据
		fin0 >> (pConsNode + i)->iNode >> (pConsNode + i)->iaConstrainedDOF[0] >> (pConsNode + i)->iaConstrainedDOF[1] >> (pConsNode + i)->iaConstrainedDOF[2];
	for (i = 0; i < nTotalElem; i++)  //读入单元数据
		fin0 >> (pElem + i)->iType >> (pElem + i)->iaNode[0] >> (pElem + i)->iaNode[1]
		>> (pElem + i)->iSection >> (pElem + i)->iMaterial;
	for (i = 0; i < nMaterialType; i++)  //读入材料数据
		fin0 >> (pMate + i)->dE >> (pMate + i)->dMu >> (pMate + i)->dr;
	for (i = 0; i < nSectionType; i++)  //读入截面数据
		fin0 >> (pSect + i)->dA >> (pSect + i)->dIz ;          
	
	fin0 >> pARS->cdlx >> pARS->ld >> pARS->Tg1 >> pARS->Ci >> pARS->Cd >> pARS->Cz;
	//。。。。读入载荷数据
	//-------------------------------------------------------------------------------------------
	//输出原始数据
	//-------------------------------------------------------------------------------------------

	LengthSinCosCalcu(nTotalElem, pElem, pNode);
	//计算总自由度数 节点自由度和单元定位向量---------------------------------------------------
	EffectiveDOF1 = EffectiveDOF(nFreeDOF, nTotalNode, nConstrainedNode, pConsNode, pNode);
	EffectiveDOF2 = EffectiveDOF1 / 3;
	nTotalDOF = DOFIndexCalcu(nFreeDOF, nTotalNode, nConstrainedNode, pConsNode, pNode);   //总自由度数
	ElementDOFCalcu(nTotalElem, pNode, pElem, pElemDOF);
	
	//--------------------------------------------------------------------------------------------
	int* pDiag = new int[nTotalDOF];           //存放主元地址
	BandAndDiagCalcu(nTotalElem, nTotalDOF, pElem, pElemDOF, pDiag);    //计算半带宽和主元地址
	TwoArrayFree(nTotalElem, pElemDOF);        //释放单元定位向量数组的内存
	iBuf = pDiag[nTotalDOF - 1] + 1;           //计算带内元数总数
	double* pGK = new double[iBuf];            //一维变带宽存放总刚度矩阵的下三角部分
	double* pGM = new double[iBuf];            //一维变带宽存放总质量矩阵的下三角部分
	VectorZeroize(iBuf, pGK);                  //总刚置零		
	VectorZeroize(iBuf, pGM);                  //质量矩阵置零
	GKAssembly(nTotalDOF, nTotalElem, pElem, pNode, pMate, pSect, pDiag, pGK);    //总装总刚
	GMAssembly(nTotalDOF, nTotalElem, pElem, pNode, pMate, pSect, pDiag, pGM);    //总装总刚
	double** K = TwoArrayDoubAlloc( nTotalDOF, nTotalDOF);   //总刚度矩阵
	double** M = TwoArrayDoubAlloc(nTotalDOF, nTotalDOF);    //总质量矩阵
	double** Msave = TwoArrayDoubAlloc(nTotalDOF, nTotalDOF);    //对总质量矩阵进行存储
	int* BDK = new int[nTotalDOF];            //半带宽数组
	VectorZeroize(nTotalDOF, BDK);
	GetK(EffectiveDOF1, pDiag, pGK, BDK, K);  
	GetM(EffectiveDOF1, pDiag, pGM, BDK,M,Msave);
	double* E = new double[EffectiveDOF1];            //特征值开方>>圆频率
	double* T = new double[EffectiveDOF1];            //周期
	double* ZLXL = new double[EffectiveDOF1];         //质量向量
	double* ZXCYXS = new double[EffectiveDOF1];       //振型参与系数
	double* ZXCYZLx = new double[EffectiveDOF1];      //X方向振型参与质量
	double* ZXCYZLxB = new double[EffectiveDOF1];      //X方向振型参与质量比
	double** X = TwoArrayDoubAlloc(EffectiveDOF1, EffectiveDOF1);    //振型矩阵
	double** SN = TwoArrayDoubAlloc(EffectiveDOF1, EffectiveDOF1);   //质量矩阵*振型矩阵*每阶振型参与系数
	double** SNx = TwoArrayDoubAlloc(EffectiveDOF2, EffectiveDOF1);  //从SN中提取X方向自由度数据
	double** F = TwoArrayDoubAlloc(EffectiveDOF1, EffectiveDOF1);    //等效地震力
	double** Fx = TwoArrayDoubAlloc(EffectiveDOF2, EffectiveDOF1);   //顺桥向地震力
	VectorZeroize(EffectiveDOF1, ZXCYXS);
	VectorZeroize(EffectiveDOF1, E);
	VectorZeroize(EffectiveDOF1, T);
	VectorZeroize(EffectiveDOF1, ZLXL);
	VectorZeroize(EffectiveDOF1, ZXCYZLx);
	VectorZeroize(EffectiveDOF1, ZXCYZLxB);
	MatrixZeroize(EffectiveDOF1, EffectiveDOF1, X);
	MatrixZeroize(EffectiveDOF1, EffectiveDOF1, F);
	MatrixZeroize(EffectiveDOF1, EffectiveDOF1, SN);
	MatrixZeroize(EffectiveDOF2, EffectiveDOF1, SNx);
	MatrixZeroize(EffectiveDOF2, EffectiveDOF1, Fx);
	RotationTransformation(M, K, EffectiveDOF1, E, T,X,Msave,SN,ZXCYXS);  //计算特征值
	AccelerationResponseSpectrum(EffectiveDOF1,T,F,SN,pARS);              //计算地震力
	ExtractX(EffectiveDOF1,SN,SNx,ZXCYZLx,ZXCYZLxB,F, Fx);                //提取顺桥向结果    
	Conversion(EffectiveDOF1, E, T, ZXCYZLx, ZXCYZLxB, X, Fx);            //转换



/////输出结果=========================================================================
 	ofstream mycout("OUTPUT.txt");
	mycout << "输出频率(单位：rad/sec)" << endl;
 	for (i=0;i<EffectiveDOF1;i++)
 	{
 		
 		mycout << E[i] << "\t" <<endl;
 	}
 	mycout << "输出周期(单位：s)" << endl;
 	for (i = 0; i < EffectiveDOF1; i++)
 	{
 
		mycout << T[i] << "\t"<<endl;
	}

	mycout << "输出振型矩阵" << endl;
	for (i = 0; i < EffectiveDOF1; i++)
	{
		for (int j = 0; j < EffectiveDOF1; j++)
		{
			mycout << X[i][j] << "      \t";
		}
		mycout << endl;
	}
 	mycout << "输出顺桥向振型参与质量（单位：Kg）" << endl;
 	for (i = 0; i < EffectiveDOF1; i++)
 	{
 
 		mycout << ZXCYZLx[i] << "\t" << endl;
 	}
 	mycout << "输出顺桥向振型参与质量比例" << endl;
	for (i = 0; i < EffectiveDOF1; i++)
	{
 
 		mycout << ZXCYZLxB[i] << "%\t" << endl;
 	}
 
 	mycout << "输出顺桥向等效地震力（单位：kN）" << endl;
 	for (i=0;i<EffectiveDOF2;i++)
 	{
 		for (int j=0;j<EffectiveDOF1;j++)
 		{
 			mycout << Fx[i][j] << "      \t";
 		}
 		mycout << endl;
 	}

	cout << "请查看结果文件OUTPUT" << endl;
	system("pause");
	return 0;
}

