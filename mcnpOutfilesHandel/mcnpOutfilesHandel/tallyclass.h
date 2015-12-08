#pragma once
// tally �����ض���
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
	std::map<int, float> gramdensity_count; // �ܶ���Ϣ
	std::map<int, float> organvol_count;    // �����Ϣ
	std::map<int, float> organwet_count;    // ������Ϣ
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
//	int numoftally;   // tally ����
//	//int 
////private:
//};

std::string filedirGet();
int singlefileHandel(std::string filepath);
// ʶ����1��ͷ���д���ʲô���͵���Ϣ��
// ��1��ͷ����������ļ�ÿ�����ֵķָ��ߣ����������������������1��֮�����ݵ���Ϣ
std::string marklineHandel(char *marklineline);

// ����commondstr�����������Ϣ
std::string infoGet(AllInfo& ALLINFO, std::string& commandstr, std::string linetmp, std::ifstream& filehandel);

// ���������������Ϣ
// ������ļ������Ѿ���λ��Ŀ����
// ������һ�������е�string����
std::string weightandvolumeStore(AllInfo& ALLINFO, std::ifstream& filehandel);

// show phantomInfo
void phantomInfoShow(AllInfo& ALLINFO);