#include "tallyclass.h"

std::string filedirGet()
{
	std::cout << "Please input file dir path: " << std::endl;
	std::string filepath;
	//std::cin >> filepath;
	char dirtmp[300];
	std::cin.getline(dirtmp, 300);
	filepath = dirtmp;

	return filepath;
}

//处理单个output文件
//mcnp的output文件以1开头的行分割每个部分，除少数*符号外所有的行首字母一定是空格
int singlefileHandel(std::string filepath)
{
	AllInfo ALLINFO;
	std::ifstream filehandel;
	filehandel.open(filepath,std::ios::in);

	if (filehandel.fail())
	{
		std::cout << filepath << " is not exist! Skip this file." << std::endl;
		return 1;
	}

	char *linetmp = new char[300];
	std::string commandstr;
	filehandel.getline(linetmp, 299);

	while (true != filehandel.eof())
	{
		if ('1' == *(linetmp))
		{
			commandstr = marklineHandel(linetmp);   // 识别以1开头的行，这是输出文件每个部分的分隔线，后面紧跟的内容是两个带1行之间内容的信息
			strcpy(linetmp, infoGet(ALLINFO, commandstr, linetmp, filehandel).c_str());
		}
		else
		{
			filehandel.getline(linetmp, 299);
		}

		if (true == filehandel.eof())  //防止在infoGet函数中出现死循环
		{
			break;
		}
	}

	filehandel.close();
	
	return 0;
}

// 识别以1开头的行代表什么类型的信息，
// 以1开头的行是输出文件每个部分的分隔线，后面紧跟的内容是两个带1行之间内容的信息
std::string marklineHandel(char *marklineline)
{
	std::string linetmp;
	std::string commandstr;

	linetmp = marklineline;   // '\0'没有被复制进字符串
	linetmp.erase(0, 1);      // 删除首个字母1
	
	if (linetmp.empty())      // 1之后是空行，返回空格
	{
		commandstr = " ";
		return commandstr;
	}
	for (int i = 0; i < linetmp.length(); ) // 找出从首个字母开始，以单个空格连接的所有单词，碰到多个空格停止搜索
	{
		if (' ' == linetmp[0])
		{
			linetmp.erase(0, 1);    // 删除开头的空格
			i--;
			continue;
		}
		if (i < (linetmp.length() - 1) && linetmp[i+1] != ' ')
		{
			i = linetmp.find(" ", i + 1);
			if (-1 == i)   //没有空格存在时
			{
				commandstr = linetmp;
				return commandstr;
			}
		}
		else
		{
			linetmp.erase(i, linetmp.length() - i);
			commandstr = linetmp;
			return commandstr;
		}
	}

	//最后一个单词之后的空格需要被全部删除
	for (int i = 0; i < linetmp.length(); i++)
	{
		if (' ' == linetmp[linetmp.length() - i - 1])
		{
			linetmp.erase(linetmp.length() - i - 1, 1);
			i--;
		}
	}
	commandstr = linetmp;
	return commandstr;
}

// 按照commondstr的命令，储存信息
// 返回下一个命令行的string对象
std::string infoGet(AllInfo& ALLINFO, std::string& commandstr, std::string linetmp, std::ifstream& filehandel)
{
	
	if (!commandstr.compare("cell volumes and masses"))  // 储存体积和质量
	{
		return weightandvolumeStore(ALLINFO, filehandel);
	}


	char *linetmptmp = new char[300];

	while (true != filehandel.eof())
	{
		filehandel.getline(linetmptmp, 299);
		if ('1' == *(linetmptmp))
		{
			linetmp = linetmptmp;
			return linetmp;
		}
	}
}

// 储存质量和体积信息
// 传入的文件对象已经定位到目标行
// 返回下一个命令行的string对象
std::string weightandvolumeStore(AllInfo& ALLINFO, std::ifstream& filehandel)
{
	char *linetmp = new char[300]; *linetmp = '\0';
	std::string linetmptmp;
	std::stringstream sstmp;
	sstmp.clear();

	while (!filehandel.eof())
	{
		filehandel.getline(linetmp, 299);
		linetmptmp.clear();
		linetmptmp = linetmp;

		if ('1' == linetmptmp[0])  // 到下一个标志行了
		{
			return linetmptmp;
		}

		for (int i = 0; i < linetmptmp.length(); i++)
		{
			if (' ' == linetmptmp[0])
			{
				linetmptmp.erase(0, 1);    // 删除开头的空格
				i--;
			}
		}

		if (linetmptmp[0] > '0' && linetmptmp[0] < '9')
		{
			sstmp.clear();
			int organserialtmp; float densitytmp; float volumetmp; float weighttmp;
			sstmp.str(linetmptmp);
			sstmp >> linetmptmp >> organserialtmp >> linetmptmp >> densitytmp >> volumetmp >> linetmptmp >> weighttmp;
			ALLINFO.phantominfo.gramdensity_count.insert({ organserialtmp, densitytmp });
			ALLINFO.phantominfo.organvol_count.insert({ organserialtmp, volumetmp });
			ALLINFO.phantominfo.organwet_count.insert({ organserialtmp, weighttmp });
		}
	}

	phantomInfoShow(ALLINFO);
	return "\n";
}

void phantomInfoShow(AllInfo& ALLINFO)
{
	// 定义迭代器
	std::map<int, float>::iterator p1,p2,p3;
	std::cout << "--------Information for current PHANTOM--------" << std::endl;
	std::cout << " OrganId " << "    Density    "<< "    Volume    " << "    Weight    " << std::endl;
	for (p1 = ALLINFO.phantominfo.gramdensity_count.begin(), 
		p2 = ALLINFO.phantominfo.organvol_count.begin(),
		p3 = ALLINFO.phantominfo.organwet_count.begin();
		p1 != ALLINFO.phantominfo.gramdensity_count.end(); 
		p1++)
	{
		std::cout << std::setw(9) << p1->first;
		std::cout << std::setw(15) << std::setprecision(6) << p1->second;
		std::cout << std::setw(15) << std::setprecision(6) << p2->second;
		std::cout << std::setw(15) << std::setprecision(6) << p3->second;
		std::cout << std::endl;
		p2++; p3++;
	}
}