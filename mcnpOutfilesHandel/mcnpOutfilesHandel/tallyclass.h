#pragma once
// tally 类的相关定义
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <iomanip>

class PhantomInfo
{
public:
	std::map<int, float> gramdensity_count; // 密度信息
	std::map<int, float> organvol_count;    // 体积信息
	std::map<int, float> organwet_count;    // 质量信息
	PhantomInfo();
};

class TallyInfo
{
public:
	int tallytype;
	bool isStar;
	int cellserial;
	float value;
	float relativeerror;
	TallyInfo();
};

class AllInfo
{
public:
	std::vector<TallyInfo> tallyinfo;
	PhantomInfo phantominfo;
};

//class TallySum
//{
//public:
//	int numoftally;   // tally 总数
//	//int 
////private:
//};

std::string filedirGet();
int singlefileHandel(std::string filepath);
// 识别以1开头的行代表什么类型的信息，
// 以1开头的行是输出文件每个部分的分隔线，后面紧跟的内容是两个带1行之间内容的信息
std::string marklineHandel(char *marklineline);

// 按照commondstr的命令，储存信息
std::string infoGet(AllInfo& ALLINFO, std::string& commandstr, std::string linetmp, std::ifstream& filehandel);

// 储存质量和体积信息
// 传入的文件对象已经定位到目标行
// 返回下一个命令行的string对象
std::string weightandvolumeStore(AllInfo& ALLINFO, std::ifstream& filehandel);

// show phantomInfo
void phantomInfoShow(AllInfo& ALLINFO);