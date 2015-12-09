#include "tallyclass.h"

std::string filedirGet()
{
	std::cout << "Please input file dir path: " << std::endl;
	std::string filepath;
	//std::cin >> filepath;
	char dirtmp[300];
	std::cin.getline(dirtmp, 300);
	filepath = dirtmp;
	if ('\\' != filepath[filepath.length() - 1])
	{
		filepath.append("\\");
	}

	return filepath;
}

int allinfoStore(AllInfo& InfoForAll, std::string dirpath)
{
	// age : rpiam rpiaf am af 15m 15f 10m 10f 5m 5f
	// dir : ap pa llat rlat rot iso
	// erg : 0 ~ 19
	std::vector<std::string> ageall = { "rpiam", "rpiaf", "am", "af", "15m", "15f", "10m", "10f", "5m", "5f" };
	std::vector<std::string> dirall = { "ap", "pa", "llat", "rlat", "rot", "iso" };
	std::vector<std::string> ergall = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19" };

	int filelostcount = 0;

	// for (int i = 0; i < ageall.size(); i++)
	int i = 2;     // only for test "am"
	{
		for (int j = 0; j < dirall.size(); j++)
		{
			for (int k = 0; k < ergall.size(); k++)
			{
				PhantomAgeDirErg agedirerg;
				agedirerg.name = ageall[i] + "_" + dirall[j] + "_" + ergall[k] + "o";
				std::string filepath; filepath.clear();
				filepath = dirpath + agedirerg.name;
				if (j == 0 && k == 0) // 储存体模信息
				{
					if (-1 == singlefileHandel(InfoForAll, ageall[i], dirall[j], ergall[k], filepath, true))
					{
						filelostcount++;
					}
				}
				else                  //每个年龄性别体模信息只储存一次就行
				{
					if (-1 == singlefileHandel(InfoForAll, ageall[i], dirall[j], ergall[k], filepath, false))
					{
						filelostcount++;
					}
				}
			}
		}
	}

	std::cout << "--------------------" << std::endl;
	std::cout << filelostcount << "files have not found!" << std::endl;
	return 0;
}

//处理单个output文件
//mcnp的output文件以1开头的行分割每个部分，除少数*符号外所有的行首字母一定是空格
int singlefileHandel(AllInfo& InfoForAll, std::string ageinfo, std::string dirinfo, std::string erginfo,
	std::string filepath, bool ifhandelgeo)
{
	std::fstream filein;
	filein.open(filepath);

	if (filein.fail())
	{
		std::cout << filepath << " is not exist! " << "Skip and do nothing! " << std::endl;
		return -1;
	}
	else
	{
		std::cout << filepath << " handeling ~~" << std::endl;
		char* linetmp = new char[300]; linetmp[0] = '\0';
		std::string commandtmp; commandtmp.clear();
		while (!filein.eof())
		{
			filein.getline(linetmp, 299);
			if ('1' == linetmp[0]) // 目的是找到第一个以1开头的行
			{
				break;
			}
		}

		std::string tmpforlinetmp;
		PhantomInfo phantominfo;
		TallyInfoInAPhantom phantomtallies;

		while (1)
		{
			tmpforlinetmp.clear();
			commandtmp = marklineHandel(linetmp);
			
			tmpforlinetmp = infoInSFGet(filein, commandtmp, linetmp,
				phantominfo, phantomtallies, ifhandelgeo);  // 读取体模信息和相关的tally信息,返回下一个以1开头的行

			strcpy(linetmp, tmpforlinetmp.c_str());
			if (filein.eof() || !strcmp("#####", linetmp)) //遇到文件结尾或者数据处理中返回的五个#，结束循环
			{
				break;
			}
		}
		
		if (true == ifhandelgeo && (phantominfo.gramdensity_count.empty()
			|| phantominfo.organvol_count.empty() || phantominfo.organwet_count.empty()))// 没有读出体模信息
		{
			std::cout << "Can't get Phantom vol&wet&density info from this file" << std::endl;
		}
		else
		{
			InfoForAll.phantominfo_count_all.insert({ ageinfo, phantominfo });
		}
		if (true == phantomtallies.tallyinaphantom_count.empty())                       // 没有读出任何tally信息
		{
			std::cout << "Can't get Phantom tally info from this file" << std::endl;
		}
		else
		{
			InfoForAll.tally_count_all.insert({ ageinfo + "_" + dirinfo + "_" + erginfo, phantomtallies });
		}

	}

	filein.close();
	
	return 0;
}

std::string infoInSFGet(std::fstream& filein, std::string commandtmp, char* linetmp,
	PhantomInfo& phantominfo, TallyInfoInAPhantom& phantomtallies, bool ifhandelgeo)
{
	std::string nextcommandline;
	if (!commandtmp.compare("cell volumes and masses") && true == ifhandelgeo)  // 需要获得体模的信息
	{
		nextcommandline = phantominfoGETSF(filein, linetmp, phantominfo);
	}
	else if (0 == commandtmp.find("tally"))     // tally行,分f4和f8两种，可能出现“1tally fluctuation charts”这种情况，需要做判断
	{
		TallyInfo onetally;
		nextcommandline = tallyinfoGetSF(filein, linetmp, phantomtallies);
	}

	else
	{
		while (true != filein.eof())
		{
			filein.getline(linetmp, 299);
			if ('1' == *linetmp)   // 到了下一个命令行
			{
				nextcommandline = linetmp;
				return nextcommandline;
			}
		}
	}
	
	return nextcommandline;
}

std::string phantominfoGETSF(std::fstream& filein, char* linetmp, PhantomInfo& phantominfo)
{
	std::string commandline = linetmp; // 储存下一个命令行和作为linetmptmp的临时储存
	char *linetmptmp = new char[300];
	std::stringstream sstmp; sstmp.clear();

	while (true != filein.eof())
	{
		filein.getline(linetmptmp, 299);
		if ('1' == *linetmptmp)   // 到了下一个命令行
		{
			commandline = linetmptmp;
			return commandline;
		}
		else
		{
			for (int i = 0; i < strlen(linetmptmp); i++)
			{
				if (' ' != linetmptmp[i]) // 去除开头的所有空格
				{
					strcpy(linetmptmp, linetmptmp + i);
					break;
				}
			}
			if (linetmptmp[0] >= '0' && linetmptmp[0] <= '9')  //数字
			{
				commandline = linetmptmp;
				sstmp.clear();
				sstmp.str(commandline);

				// A example
				//        cell     atom          gram         input       calculated                            reason volume
				//                density       density       volume        volume         mass       pieces    not calculated
				//    1   777  0.00000E+00   0.00000E+00   0.00000E+00   0.00000E+00   0.00000E+00      0      infinite
				float gramdensity = 0; float organvol = 0; float organwet = 0;
				int organID = 0;

				sstmp >> organID; sstmp >> organID;
				sstmp >> gramdensity; sstmp >> gramdensity;
				sstmp >> organvol;
				sstmp >> organwet; sstmp >> organwet;

				// Store phantom info line by line
				phantominfo.organwet_count.insert({ organID, organwet });
				phantominfo.organvol_count.insert({ organID,organvol });
				phantominfo.gramdensity_count.insert({ organID,gramdensity });
			}
		}
	}
	
	// reach the end of file
	commandline == "#####";
	return commandline;
}

std::string tallyinfoGetSF(std::fstream& filein, char* linetmp, TallyInfoInAPhantom& phantomtallies)
{
	std::string commandline = linetmp;
	char* linetmptmp = new char[300];
	TallyInfo onetally;
	int tallytype;  // 记录tally的类型

	std::stringstream sstmp; sstmp.clear();
	commandline.erase(0, 6); // 删除开头的1tally
	int ii = 0;
	for (; ii < commandline.length(); ii++)
	{
		if (' ' != commandline[ii])
		{
			break;
		}
	}
	commandline.erase(0, ii);
	if (commandline[0] >='0' && commandline[0] <= '9')  // 用于排除其他无效项
	{
		sstmp.str(commandline);

		sstmp >> tallytype;


		while (true != filein.eof())
		{
			filein.getline(linetmptmp, 299);
			if ('1' == *linetmptmp)
			{
				commandline = linetmptmp;
				break;
			}

			for (int i = 0; i < strlen(linetmptmp); i++)
			{
				if (' ' != linetmptmp[i]) // 去除开头的所有空格
				{
					strcpy(linetmptmp, linetmptmp + i);
					break;
				}
			}

			commandline.clear();
			commandline = linetmptmp;

			if (0 == commandline.find("tally type ")) // tally是否为*
			{
				commandline.erase(0, 12);
				if ('*' == commandline[0])
				{
					onetally.isStar = true;
				}
				else
				{
					onetally.isStar = false;
				}
			}

			if (0 == commandline.find("cell ("))
			{
				float tallyvalue = 0; // 暂时储存值和统计误差
				float tallyerror = 0; //
				commandline.erase(0, 6);
				int indextmp = commandline.find("<");
				std::string inindex; inindex.clear();
				inindex.append(commandline, 0, indextmp);
				sstmp.clear();
				sstmp.str(inindex);
				sstmp >> indextmp;  // cell 序号

				// 读取下一行，为cell的数据行
				filein.getline(linetmptmp, 299);
				for (int i = 0; i < strlen(linetmptmp); i++)
				{
					if (' ' != linetmptmp[i]) // 去除开头的所有空格
					{
						strcpy(linetmptmp, linetmptmp + i);
						break;
					}
				}
				sstmp.clear();
				sstmp.str(linetmptmp);
				sstmp >> tallyvalue;
				sstmp >> tallyerror;

				std::pair<float, float> val_err(tallyvalue, tallyerror);
				onetally.valerr_count.insert({ indextmp, val_err });
			}
		}

		phantomtallies.tallyinaphantom_count.insert({ tallytype , onetally });

		return commandline;
	}
	else
	{
		while (true != filein.eof())
		{
			filein.getline(linetmptmp, 299);
			if ('1' == *linetmptmp)
			{
				commandline = linetmptmp;
				return commandline;
			}
		}
	}
	
	// reach the end of file
	commandline == "#####";
	return commandline;
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