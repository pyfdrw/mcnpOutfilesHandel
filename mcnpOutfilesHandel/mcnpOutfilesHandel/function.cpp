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
				if (j == 0 && k == 0) // ������ģ��Ϣ
				{
					if (-1 == singlefileHandel(InfoForAll, ageall[i], dirall[j], ergall[k], filepath, true))
					{
						filelostcount++;
					}
				}
				else                  //ÿ�������Ա���ģ��Ϣֻ����һ�ξ���
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

//������output�ļ�
//mcnp��output�ļ���1��ͷ���зָ�ÿ�����֣�������*���������е�������ĸһ���ǿո�
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
			if ('1' == linetmp[0]) // Ŀ�����ҵ���һ����1��ͷ����
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
				phantominfo, phantomtallies, ifhandelgeo);  // ��ȡ��ģ��Ϣ����ص�tally��Ϣ,������һ����1��ͷ����

			strcpy(linetmp, tmpforlinetmp.c_str());
			if (filein.eof() || !strcmp("#####", linetmp)) //�����ļ���β�������ݴ����з��ص����#������ѭ��
			{
				break;
			}
		}
		
		if (true == ifhandelgeo && (phantominfo.gramdensity_count.empty()
			|| phantominfo.organvol_count.empty() || phantominfo.organwet_count.empty()))// û�ж�����ģ��Ϣ
		{
			std::cout << "Can't get Phantom vol&wet&density info from this file" << std::endl;
		}
		else
		{
			InfoForAll.phantominfo_count_all.insert({ ageinfo, phantominfo });
		}
		if (true == phantomtallies.tallyinaphantom_count.empty())                       // û�ж����κ�tally��Ϣ
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
	if (!commandtmp.compare("cell volumes and masses") && true == ifhandelgeo)  // ��Ҫ�����ģ����Ϣ
	{
		nextcommandline = phantominfoGETSF(filein, linetmp, phantominfo);
	}
	else if (0 == commandtmp.find("tally"))     // tally��,��f4��f8���֣����ܳ��֡�1tally fluctuation charts�������������Ҫ���ж�
	{
		TallyInfo onetally;
		nextcommandline = tallyinfoGetSF(filein, linetmp, phantomtallies);
	}

	else
	{
		while (true != filein.eof())
		{
			filein.getline(linetmp, 299);
			if ('1' == *linetmp)   // ������һ��������
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
	std::string commandline = linetmp; // ������һ�������к���Ϊlinetmptmp����ʱ����
	char *linetmptmp = new char[300];
	std::stringstream sstmp; sstmp.clear();

	while (true != filein.eof())
	{
		filein.getline(linetmptmp, 299);
		if ('1' == *linetmptmp)   // ������һ��������
		{
			commandline = linetmptmp;
			return commandline;
		}
		else
		{
			for (int i = 0; i < strlen(linetmptmp); i++)
			{
				if (' ' != linetmptmp[i]) // ȥ����ͷ�����пո�
				{
					strcpy(linetmptmp, linetmptmp + i);
					break;
				}
			}
			if (linetmptmp[0] >= '0' && linetmptmp[0] <= '9')  //����
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
	int tallytype;  // ��¼tally������

	std::stringstream sstmp; sstmp.clear();
	commandline.erase(0, 6); // ɾ����ͷ��1tally
	int ii = 0;
	for (; ii < commandline.length(); ii++)
	{
		if (' ' != commandline[ii])
		{
			break;
		}
	}
	commandline.erase(0, ii);
	if (commandline[0] >='0' && commandline[0] <= '9')  // �����ų�������Ч��
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
				if (' ' != linetmptmp[i]) // ȥ����ͷ�����пո�
				{
					strcpy(linetmptmp, linetmptmp + i);
					break;
				}
			}

			commandline.clear();
			commandline = linetmptmp;

			if (0 == commandline.find("tally type ")) // tally�Ƿ�Ϊ*
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
				float tallyvalue = 0; // ��ʱ����ֵ��ͳ�����
				float tallyerror = 0; //
				commandline.erase(0, 6);
				int indextmp = commandline.find("<");
				std::string inindex; inindex.clear();
				inindex.append(commandline, 0, indextmp);
				sstmp.clear();
				sstmp.str(inindex);
				sstmp >> indextmp;  // cell ���

				// ��ȡ��һ�У�Ϊcell��������
				filein.getline(linetmptmp, 299);
				for (int i = 0; i < strlen(linetmptmp); i++)
				{
					if (' ' != linetmptmp[i]) // ȥ����ͷ�����пո�
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

// ʶ����1��ͷ���д���ʲô���͵���Ϣ��
// ��1��ͷ����������ļ�ÿ�����ֵķָ��ߣ����������������������1��֮�����ݵ���Ϣ
std::string marklineHandel(char *marklineline)
{
	std::string linetmp;
	std::string commandstr;

	linetmp = marklineline;   // '\0'û�б����ƽ��ַ���
	linetmp.erase(0, 1);      // ɾ���׸���ĸ1
	
	if (linetmp.empty())      // 1֮���ǿ��У����ؿո�
	{
		commandstr = " ";
		return commandstr;
	}
	for (int i = 0; i < linetmp.length(); ) // �ҳ����׸���ĸ��ʼ���Ե����ո����ӵ����е��ʣ���������ո�ֹͣ����
	{
		if (' ' == linetmp[0])
		{
			linetmp.erase(0, 1);    // ɾ����ͷ�Ŀո�
			i--;
			continue;
		}
		if (i < (linetmp.length() - 1) && linetmp[i+1] != ' ')
		{
			i = linetmp.find(" ", i + 1);
			if (-1 == i)   //û�пո����ʱ
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

	//���һ������֮��Ŀո���Ҫ��ȫ��ɾ��
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