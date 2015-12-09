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
	std::string name;
	std::string ageGet();
	std::string dirGet();
	std::string ergGet();
};



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

