#include "fstream"
#include"iostream"
#include "vector"
using namespace std;

#define TRUSS              0
#define FRAME              1

#define TRUSS_NODE         0
#define FRAME_NODE         1

#define DIRECT_X           0
#define DIRECT_Y           1
#define DIRECT_R           2

struct Material
{
	double dE;                //弹性模量
	double dMu;               //泊松比
	double dr;                //密度
};
struct Section
{
	double dA;                //横截面面积
	double dIz;               //横截面惯性矩
	//double dH;                //横截面高？
};
struct Node
{
	int iType;                //节点类型
	double dX, dY;            //节点坐标
	int iaDOFIndex[3];     //节点自由度编号
};
struct Element
{
	int iType;                //单元类型号
	int iaNode[2];         //单元两端节点编号
	int iSection;          //单元截面索引号
	int iMaterial;         //单元材料索引号
	double dLength;         //单元长度
	double dSin, dCos;      //单元局部坐标x轴与整体坐标轴x轴的夹角的正余弦
	double daEndInterForce;  //单元杆端力向量
};
struct ConstrainedNode
{
	int iNode;                   //受约束节点号
	int iaConstrainedDOF[3];       //受约束节点自由度特性；0-位移位置，-1位移已知
};


struct ARS
{
	int cdlx;      ////////////////////////场地类型有0~4五种
	char ld;       //////////////////////////抗震设防烈度有6~9四种	
 				  //A=0.05,B=0.1,C=0.15,D=0.2,E=0.3,F=0.4
    char Tg1;  //////////////////////按《中国地震动参数区划表》查取
 				 //A=0.35,B=0.4,C=0.45	
    double Ci; //抗震重要性系数，按规范3.1.3 - 2取值
	double Cd = 1.0; //阻尼调整系数
	double T0 = 0.1; //反应谱上升段的最大周期
	double Tg;//特征周期0.35\0.40\0.45
	double Cs;// 场地系数
    double Cz ;//综合影响系数（考虑允许结构发生塑性变形对地震荷载的折减）

};


int DOFIndexCalcu(int& iBuf0, int nTotalNode, int nConstrainedNode, ConstrainedNode* pConsNode, Node* pNode);
int EffectiveDOF(int& iBuf0, int nTotalNode, int nConstrainedNode, ConstrainedNode* pConsNode, Node* pNode);
void ElementDOFCalcu(int nTotalElem, Node* pNode, Element* pElem, int** pElemDOF);
void BandAndDiagCalcu(int nTotalElem, int nTotalDOF, Element* pElem, int** pElemDOF, int* pDiag);
double** TwoArrayDoubAlloc(int nRow, int nCol);
int ** TwoArrayIntbAlloc(int nRow, int nCol);
template<class T>
void TwoArrayFree(int nRow, T** pdi) 
{
	for (int i = 0; i < nRow; i++)
		delete[]pdi[i];   //回收列空间	
	delete[]pdi;      //回收行空间

};
template<class T>
void MatrixZeroize(int nRow, int nCol, T** pT)
{
	for (int i = 0; i < nRow; i++)
	{
		for (int j = 0; j < nCol; j++)
		{
			pT[i][j] = 0;
		}
	}
};
template<class T>
void VectorZeroize(int n, T* pT)
{
	for (int i = 0; i < n; i++)
		pT[i] = 0;
};
void MatrixMultiply(int nRow, int nCol, double** pA, double** pB, double** PC);
void MatrixVectorMultiply1(int nRow, int nCol, double** pA, double* pB, double* pC);
void MatrixVectorMultiply2(int nRow, int nCol, double** pA, double* pB, double* pC);
void MatrixTrans(int nRow, int nCol, double** pA, double** pAT);
void LengthSinCosCalcu(int nTotalElem, Element* pElem, Node*  pNode);
void TrussElemStiffcalcu(ofstream&fout1, Element* pElem, Material* pMate, Section * pSect, double** pKe);
void FrameElemStiffCalcu(ofstream&fout1, Element* pElem, Material* pMate, Section* pSect, double** pKe);
void GKAssembly(int nToallDOF, int nToallElem, Element* pElem, Node* pNode, Material* pMate, Section* pSect, int* pDiag, double* pGK);
void GetK(int iBuf, int* pDiag, double* pGK, int* BDK, double** K);
void GetM(int iBuf, int* pDiag, double* pGM, int* BDK, double** M, double** Msave);
void FrameElemMassCalcu(ofstream&fout1, Element* pElem, Material* pMate, Section* pSect, double** pMe);
void GMAssembly(int nToallDOF, int nToallElem, Element* pElem, Node* pNode, Material* pMate, Section* pSect, int* pDiag, double* pGM);
void RotationTransformation(double** M, double** K, int EffectiveDOF1, double* E, double* T,
	double**X, double**Msave, double**SN, double*ZXCYXS);
void AccelerationResponseSpectrum(int EffectiveDOF1, double* T, double** FF, double** SN, ARS* pARS);
void ExtractX(int EffectiveDOF1, double** SN, double** SNx, double* ZXCYZLx,
	double* ZXCYZLB, double** F, double** Fx);
void Conversion(int EffectiveDOF1, double* E, double* T, double* ZXCYZLx,
	double* ZXCYZLxB, double** X, double** Fx);








