#pragma once
// tally �����ض���
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <iomanip>
#include <utility>

class PhantomInfo
{
public:
	std::map<int, float> gramdensity_count; // �ܶ���Ϣ
	std::map<int, float> organvol_count;    // �����Ϣ
	std::map<int, float> organwet_count;    // ������Ϣ
	PhantomInfo();
};

class TallyInfo // ���浥��tally����Ϣ
{
public:
	bool isStar;
	std::map<int, std::pair<float, float>> valerr_count;  // (cell {value, error})
	TallyInfo();
};

class TallyInfoInAPhantom // ����һ����ģ�ڲ�������tallyֵ���洢��һ��map�����У����м�Ϊtally������(1004��14��������mcnp tally�����ͬ)
{
public:
	std::map<int, TallyInfo> tallyinaphantom_count;
	TallyInfoInAPhantom();
};

class AllInfo
{
public:
	std::map<std::string, TallyInfoInAPhantom> tally_count_all;
	std::map<std::string, PhantomInfo> phantominfo_count_all;
	AllInfo();
};

class PhantomAgeDirErg
{
public:
	PhantomAgeDirErg();
	PhantomAgeDirErg(std::string agedirerg);
	std::string name;
	std::string ageGet();
	std::string dirGet();
	std::string ergGet();
};

// age : rpiam rpiaf am af 15m 15f 10m 10f 5m 5f
// dir : ap pa llat rlat rot iso
// erg : 0 ~ 19
const std::vector<std::string> ageall = { "rpiam", "rpiaf", "am", "af", "15m", "15f", "10m", "10f", "5m", "5f" };
const std::vector<std::string> dirall = { "ap", "pa", "llat", "rlat", "rot", "iso" };
const std::vector<std::string> ergall = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19" };
const std::vector<std::string> ergall_val = { "0.02", "0.03", "0.04" , "0.05" , "0.06" , "0.08" , "0.1" , "0.15" , "0.2" , "0.3" , "0.4" , "0.5" , "0.6" , "0.8" , "1" , "2" , "4" , "6" , "8" , "10" };

// �����ķֲ�ֵ��ע���ܰٷֱȲ���1
// const std::vector<double>  FractionRBMForAdult = { 2.3,  0.8,  7.6 ,  6.7,  0.8,  17.5,  16.1,  2.8,  3.9,  16.1,  12.3,  9.9,  3.1 };
const std::vector<double>  FractionRBMForAdult = { 2.5 ,0.8 ,7.7 ,7.4 ,0.8 ,19.5 ,15.2 ,2.9 ,3.7 ,15.3 ,11.7 ,9.4 ,3 };
const std::vector<double>  FractionRBMFor15 = { 3.1,  1  ,  9.2 ,  9.2,  0.9,  18.5,  13.6,  3.3,  3.3,  13.7,  10.5,  8.4,  2.7 } ;
const std::vector<double>  FractionRBMFor10 = { 2.5,  0.9,  11.6,  9.4,  1.1,  15.6,  10.9,  2.9,  2.7,  10.9,  8.4 ,  6.7,  2.1 };
const std::vector<double>  FractionRBMFor5 = { 2.4,  0.9,  15.9,  6.8,  1.6,  13.1,  8.8 ,  2.7,  2.2,  8.9 ,  6.8 ,  5.5,  1.7 };

// ��֯Ȩ������
const double TISSUESWEIGHTINGFACTOR[4] = { 0.12, 0.08, 0.04, 0.01 };

// Ka, pGy*cm^2
const std::vector<double>  KERMAFREEINAIR = {1.68, 0.721, 0.429, 0.323, 0.289, 0.307, 0.371, 0.599, 0.856, 1.38, 1.89, 2.38, 2.84, 3.69, 4.47, 7.54, 12.1, 16.1, 20.1, 24};

// Դ���������"ap", "pa", "llat", "rlat", "rot", "iso"��˳��, cm2
// const std::vector<double> SOURCEAREA = {12104, 12104, 7476, 7476, 39160, 113411.49};
const std::vector<double> SOURCEAREA = { 12104, 12104, 7476, 7476, 9790, 28352.87 };
//class TallySum
//{
//public:
//	int numoftally;   // tally ����
//	//int 
////private:
//};

int allinfoStore(AllInfo& InfoForAll, std::string dirpath);

std::string filedirGet();
// ʶ����1��ͷ���д���ʲô���͵���Ϣ��
// ��1��ͷ����������ļ�ÿ�����ֵķָ��ߣ����������������������1��֮�����ݵ���Ϣ
std::string marklineHandel(char *marklineline);

int singlefileHandel(AllInfo& InfoForAll, std::string ageinfo, std::string dirinfo, std::string erginfo,
	std::string filepath, bool ifhandelgeo);

//SF == single file
std::string infoInSFGet(std::fstream& filein, std::string commandtmp, char* linetmp, 
	PhantomInfo& phantominfo, TallyInfoInAPhantom& phantomtallies, bool ifhandelgeo);

//SF == single file
//������һ����1��ͷ�������е��ַ���
std::string phantominfoGETSF(std::fstream& filein, char* linetmp, PhantomInfo& phantominfo);
std::string tallyinfoGetSF(std::fstream& filein, char* linetmp, TallyInfoInAPhantom& phantomtallies);

// ��ģ��Ϣ��������ļ��������Ļ��ʾ����
// phantomname��ȡ"am" "af" ~~ "5m" "5f" "rpiam" "rpiaf"
int showSinglePhantomInfo(std::string phantomname, AllInfo InfoForAll, std::string outputfilepath);
int showSinglePhantomInfo(std::string phantomname, AllInfo InfoForAll);

// һ����������ӵ�����tally��Ϣ���������ʾ��������ļ�����
// instancenameΪһ��������ļ�������ȡ����"am_ap_0"������ģ��_����_���������
int showSinglePhantomInfo(PhantomAgeDirErg instancename, AllInfo InfoForAll, std::string outputfilepath);
int showSinglePhantomInfo(PhantomAgeDirErg instancename, AllInfo InfoForAll);

// ���ɹ��ӱ��Ͷ��ܵ��������ռ�����ת��ϵ��DCCk
int calDCCK(int organ, std::string agetmp, AllInfo InfoForAll, std::string outputfilepath);
int calDCCK(int organleft, int organright, std::string agetmp, AllInfo InfoForAll, std::string outputfilepath);
float *calDCCKWithOutShow(int organ, std::string agetmp, AllInfo InfoForAll);
float *calDCCKWithOutShow(int organleft, int organright, std::string agetmp, AllInfo InfoForAll);
float *calDCCKWithShow(int organleft, int organright, std::string agetmp, AllInfo InfoForAll); // ��չʾ��

// �����������ռ���ת��ϵ��DCCk RBM
int calDCCKRBMWithShow(PhantomAgeDirErg instancename, AllInfo InfoForAll, std::string outputfilepath);
int calDCCKRBMWithShow(PhantomAgeDirErg instancename, AllInfo InfoForAll);
float calDCCKRBMWithOutShow(PhantomAgeDirErg instancename, AllInfo InfoForAll); // ���ع����DCCKRBM

//0 ����� �᳦ �� θ ���� ������֯
//1 ����
//2 ���� ʳ�� ���� ��״��
//3 �Ǳ��� �� ��Һ�� Ƥ��

// �����Ա�ƽ������Ч����
// ICRP 20 WT ���� 0.25 ����� 0.12 �� 0.12 ��״�� 0.03 ���� 0.15 �Ǳ��� 0.03 ������֯ 0.30 ȫ�� 1.00
// ICRP 60 WT ���� 0.2  ����� 0.12 �� 0.12 θ 0.12 �᳦ 0.12 ��״�� 0.05 ���� 0.05 ʳ�� 0.05 �� 0.05 ���� 0.05 �Ǳ��� 0.01 Ƥ�� 0.01 ������֯ 0.05 ȫ�� 1.00
// ICRP 103 WT ����� �᳦ �� θ ���� ������֯ 0.12 ���� 0.08 ���� ʳ�� �� ��״�� 0.04 �Ǳ��� �� ��Һ�� Ƥ�� 0.01
// ICRP 103 ������֯���WT����14����֯��ƽ������
// 13(��Ů����һ��)�����ٵ�������ȡƽ��
// �����(�����ʹ���)
// �᳦ 76 78 80 82 84 �� 97 99 θ 72 ���� 63 65 
// ���� 129 130(غ��) 111 112(�ѳ�) 
// ���� 137 ʳ�� 110 �� 95 ��״�� 132 
// �Ǳ��� �� 61 ��Һ�� 120 121 Ƥ�� 125
// ��ʱֻ����103������
float calEffectiveDose(PhantomAgeDirErg instancename, AllInfo InfoForAll);
float calEffectiveDose(PhantomAgeDirErg instancename, AllInfo InfoForAll, std::string outputfilepath);

// ����������������MeV
std::pair<float, float> getSingleOrganErg(PhantomAgeDirErg instancename, AllInfo& InfoForAll, int organID);
// ������������
float getSingleOrganWet(PhantomAgeDirErg instancename, AllInfo& InfoForAll, int organID);

// �ռ���Ҫ����Ϣ
int dataControlFun(std::string dirpath, AllInfo& InfoForAll);
int organCompareOut(float *ratiodata, std::string filename);

// ���ɶԱ�����
float* organCompare(int organID1, int organID2, std::string compare1, std::string compare2, AllInfo& InfoForAll);
float* organCompare(int organID, std::string compare1, std::string compare2, AllInfo& InfoForAll);

int outControl(float* ratiodata, int organID, std::string compare1, std::string compare2, std::string workpath, AllInfo& InfoForAll);
int outControl(float* ratiodata, int organID1, int organID2, std::string compare1, std::string compare2, std::string workpath, AllInfo& InfoForAll);