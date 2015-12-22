#pragma once
// tally 类的相关定义
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
	std::map<int, float> gramdensity_count; // 密度信息
	std::map<int, float> organvol_count;    // 体积信息
	std::map<int, float> organwet_count;    // 质量信息
	PhantomInfo();
};

class TallyInfo // 储存单个tally的信息
{
public:
	bool isStar;
	std::map<int, std::pair<float, float>> valerr_count;  // (cell {value, error})
	TallyInfo();
};

class TallyInfoInAPhantom // 储存一个体模内部的所有tally值，存储在一个map对象中，其中键为tally的类型(1004或14或其他和mcnp tally序号相同)
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

// 红骨髓的分布值，注意总百分比不是1
// const std::vector<double>  FractionRBMForAdult = { 2.3,  0.8,  7.6 ,  6.7,  0.8,  17.5,  16.1,  2.8,  3.9,  16.1,  12.3,  9.9,  3.1 };
const std::vector<double>  FractionRBMForAdult = { 2.5 ,0.8 ,7.7 ,7.4 ,0.8 ,19.5 ,15.2 ,2.9 ,3.7 ,15.3 ,11.7 ,9.4 ,3 };
const std::vector<double>  FractionRBMFor15 = { 3.1,  1  ,  9.2 ,  9.2,  0.9,  18.5,  13.6,  3.3,  3.3,  13.7,  10.5,  8.4,  2.7 } ;
const std::vector<double>  FractionRBMFor10 = { 2.5,  0.9,  11.6,  9.4,  1.1,  15.6,  10.9,  2.9,  2.7,  10.9,  8.4 ,  6.7,  2.1 };
const std::vector<double>  FractionRBMFor5 = { 2.4,  0.9,  15.9,  6.8,  1.6,  13.1,  8.8 ,  2.7,  2.2,  8.9 ,  6.8 ,  5.5,  1.7 };

// 组织权重因子
const double TISSUESWEIGHTINGFACTOR[4] = { 0.12, 0.08, 0.04, 0.01 };

// Ka, pGy*cm^2
const std::vector<double>  KERMAFREEINAIR = {1.68, 0.721, 0.429, 0.323, 0.289, 0.307, 0.371, 0.599, 0.856, 1.38, 1.89, 2.38, 2.84, 3.69, 4.47, 7.54, 12.1, 16.1, 20.1, 24};

// 源的面积按照"ap", "pa", "llat", "rlat", "rot", "iso"的顺序, cm2
// const std::vector<double> SOURCEAREA = {12104, 12104, 7476, 7476, 39160, 113411.49};
const std::vector<double> SOURCEAREA = { 12104, 12104, 7476, 7476, 9790, 28352.87 };
//class TallySum
//{
//public:
//	int numoftally;   // tally 总数
//	//int 
////private:
//};

int allinfoStore(AllInfo& InfoForAll, std::string dirpath);

std::string filedirGet();
// 识别以1开头的行代表什么类型的信息，
// 以1开头的行是输出文件每个部分的分隔线，后面紧跟的内容是两个带1行之间内容的信息
std::string marklineHandel(char *marklineline);

int singlefileHandel(AllInfo& InfoForAll, std::string ageinfo, std::string dirinfo, std::string erginfo,
	std::string filepath, bool ifhandelgeo);

//SF == single file
std::string infoInSFGet(std::fstream& filein, std::string commandtmp, char* linetmp, 
	PhantomInfo& phantominfo, TallyInfoInAPhantom& phantomtallies, bool ifhandelgeo);

//SF == single file
//返回下一个以1开头的命令行的字符串
std::string phantominfoGETSF(std::fstream& filein, char* linetmp, PhantomInfo& phantominfo);
std::string tallyinfoGetSF(std::fstream& filein, char* linetmp, TallyInfoInAPhantom& phantomtallies);

// 体模信息输出，分文件输出和屏幕显示两种
// phantomname可取"am" "af" ~~ "5m" "5f" "rpiam" "rpiaf"
int showSinglePhantomInfo(std::string phantomname, AllInfo InfoForAll, std::string outputfilepath);
int showSinglePhantomInfo(std::string phantomname, AllInfo InfoForAll);

// 一个具体的例子的所有tally信息输出，分显示和输出到文件两种
// instancename为一个具体的文件名，可取例如"am_ap_0"，即体模名_方向_能量的组合
int showSinglePhantomInfo(PhantomAgeDirErg instancename, AllInfo InfoForAll, std::string outputfilepath);
int showSinglePhantomInfo(PhantomAgeDirErg instancename, AllInfo InfoForAll);

// 生成光子比释动能到器官吸收剂量的转换系数DCCk
int calDCCK(int organ, std::string agetmp, AllInfo InfoForAll, std::string outputfilepath);
int calDCCK(int organleft, int organright, std::string agetmp, AllInfo InfoForAll, std::string outputfilepath);
float *calDCCKWithOutShow(int organ, std::string agetmp, AllInfo InfoForAll);
float *calDCCKWithOutShow(int organleft, int organright, std::string agetmp, AllInfo InfoForAll);
float *calDCCKWithShow(int organleft, int organright, std::string agetmp, AllInfo InfoForAll); // 做展示用

// 计算骨髓的吸收剂量转换系数DCCk RBM
int calDCCKRBMWithShow(PhantomAgeDirErg instancename, AllInfo InfoForAll, std::string outputfilepath);
int calDCCKRBMWithShow(PhantomAgeDirErg instancename, AllInfo InfoForAll);
float calDCCKRBMWithOutShow(PhantomAgeDirErg instancename, AllInfo InfoForAll); // 返回骨髓的DCCKRBM

//0 红骨髓 结肠 肺 胃 乳腺 其余组织
//1 性腺
//2 膀胱 食道 肝脏 甲状腺
//3 骨表面 脑 唾液腺 皮肤

// 计算性别平均的有效剂量
// ICRP 20 WT 性腺 0.25 红骨髓 0.12 肺 0.12 甲状腺 0.03 乳腺 0.15 骨表面 0.03 其余组织 0.30 全身 1.00
// ICRP 60 WT 性腺 0.2  红骨髓 0.12 肺 0.12 胃 0.12 结肠 0.12 甲状腺 0.05 乳腺 0.05 食道 0.05 肝 0.05 膀胱 0.05 骨表面 0.01 皮肤 0.01 其余组织 0.05 全身 1.00
// ICRP 103 WT 红骨髓 结肠 肺 胃 乳腺 其余组织 0.12 性腺 0.08 膀胱 食道 肝 甲状腺 0.04 骨表面 脑 唾液腺 皮肤 0.01
// ICRP 103 其余组织标称WT用于14个组织的平均剂量
// 13(男女各少一个)个器官当量剂量取平均
// 红骨髓(在松质骨中)
// 结肠 76 78 80 82 84 肺 97 99 胃 72 乳腺 63 65 
// 性腺 129 130(睾丸) 111 112(卵巢) 
// 膀胱 137 食道 110 肝 95 甲状腺 132 
// 骨表面 脑 61 唾液腺 120 121 皮肤 125
// 暂时只计算103的数据
float calEffectiveDose(PhantomAgeDirErg instancename, AllInfo InfoForAll);
float calEffectiveDose(PhantomAgeDirErg instancename, AllInfo InfoForAll, std::string outputfilepath);

// 返回器官能量沉积MeV
std::pair<float, float> getSingleOrganErg(PhantomAgeDirErg instancename, AllInfo& InfoForAll, int organID);
// 返回器官质量
float getSingleOrganWet(PhantomAgeDirErg instancename, AllInfo& InfoForAll, int organID);

// 收集需要的信息
int dataControlFun(std::string dirpath, AllInfo& InfoForAll);
int organCompareOut(float *ratiodata, std::string filename);

// 生成对比数据
float* organCompare(int organID1, int organID2, std::string compare1, std::string compare2, AllInfo& InfoForAll);
float* organCompare(int organID, std::string compare1, std::string compare2, AllInfo& InfoForAll);

int outControl(float* ratiodata, int organID, std::string compare1, std::string compare2, std::string workpath, AllInfo& InfoForAll);
int outControl(float* ratiodata, int organID1, int organID2, std::string compare1, std::string compare2, std::string workpath, AllInfo& InfoForAll);