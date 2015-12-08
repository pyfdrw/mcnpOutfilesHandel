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

//������output�ļ�
//mcnp��output�ļ���1��ͷ���зָ�ÿ�����֣�������*���������е�������ĸһ���ǿո�
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
			commandstr = marklineHandel(linetmp);   // ʶ����1��ͷ���У���������ļ�ÿ�����ֵķָ��ߣ����������������������1��֮�����ݵ���Ϣ
			strcpy(linetmp, infoGet(ALLINFO, commandstr, linetmp, filehandel).c_str());
		}
		else
		{
			filehandel.getline(linetmp, 299);
		}

		if (true == filehandel.eof())  //��ֹ��infoGet�����г�����ѭ��
		{
			break;
		}
	}

	filehandel.close();
	
	return 0;
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

// ����commondstr�����������Ϣ
// ������һ�������е�string����
std::string infoGet(AllInfo& ALLINFO, std::string& commandstr, std::string linetmp, std::ifstream& filehandel)
{
	
	if (!commandstr.compare("cell volumes and masses"))  // �������������
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

// ���������������Ϣ
// ������ļ������Ѿ���λ��Ŀ����
// ������һ�������е�string����
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

		if ('1' == linetmptmp[0])  // ����һ����־����
		{
			return linetmptmp;
		}

		for (int i = 0; i < linetmptmp.length(); i++)
		{
			if (' ' == linetmptmp[0])
			{
				linetmptmp.erase(0, 1);    // ɾ����ͷ�Ŀո�
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
	// ���������
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