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
	std::string name;
	std::string ageGet();
	std::string dirGet();
	std::string ergGet();
};



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

