#include "tallyclass.h"

// 用于处理和输出数据的函数

//返回值 -1 体模信息不存在  0 正常 正数 信息缺失数目
int showSinglePhantomInfo(std::string phantomname, AllInfo InfoForAll)
{
	//排列顺序
	// organID    density    volume    mass
	std::map<std::string, PhantomInfo>::iterator phantom_p;

	//返回值 -1 体模信息不存在  0 正常 正数 信息缺失数目
	int returnindex = 0;

	//所查询的年龄不存在
	if (InfoForAll.phantominfo_count_all.end() == (phantom_p = InfoForAll.phantominfo_count_all.find(phantomname)))
	{
		returnindex = -1;
		return returnindex;
	}
	else
	{
		std::map<int, float>::iterator thepantom_p;
		std::vector<int> organid_sum; organid_sum.clear();  // 用于存储所有的器官代号列表

		thepantom_p = phantom_p->second.gramdensity_count.begin();

		while (thepantom_p != phantom_p->second.gramdensity_count.end()) // 遍历gramdensity_count，记录所有的器官编号于organid_sum
		{
			organid_sum.push_back(thepantom_p->first);
			thepantom_p++;
		}

		std::cout << "============================" << std::endl;
		std::cout << "Info for " << phantomname << std::endl;
		std::cout << "ID     density(g*cm3)    volume(cm3)    mass(g)" << std::endl;
		std::vector<int>::iterator organid_sum_p; organid_sum_p = organid_sum.begin();
		float densitytmp = 0; float volumetmp = 0; float weighttmp = 0;
		while (organid_sum.end() != organid_sum_p)
		{
			if (phantom_p->second.gramdensity_count.end() != phantom_p->second.gramdensity_count.find(*organid_sum_p) &&
				phantom_p->second.organvol_count.end() != phantom_p->second.organvol_count.find(*organid_sum_p) &&
				phantom_p->second.organwet_count.end() != phantom_p->second.organwet_count.find(*organid_sum_p))
			{
				densitytmp = (phantom_p->second.gramdensity_count.find(*organid_sum_p))->second;
				volumetmp = (phantom_p->second.organvol_count.find(*organid_sum_p))->second;
				weighttmp = (phantom_p->second.organwet_count.find(*organid_sum_p))->second;
			}
			else
			{
				returnindex++;
			}

			std::cout << std::setw(5) << *organid_sum_p
				<< std::setprecision(5) << std::setw(10) << densitytmp
				<< std::setprecision(5) << std::setw(10) << volumetmp
				<< std::setprecision(5) << std::setw(10) << weighttmp
				<< std::endl;
			organid_sum_p++;
		}
	}

	return returnindex;
}

// 用于输出单个文件的包含的信息
int showSinglePhantomInfo(PhantomAgeDirErg instancename, AllInfo InfoForAll)
{
	std::string nametmp = instancename.name;
	std::map<std::string, TallyInfoInAPhantom>::iterator tallies_p;

	std::cout << "TallyType    Cell    Vlaue    Error" << std::endl;

	if (InfoForAll.tally_count_all.end() != (tallies_p = InfoForAll.tally_count_all.find(nametmp))) // 该实例存在
	{
		std::map<int, TallyInfo>::iterator onetally_p;
		onetally_p = (tallies_p->second.tallyinaphantom_count).begin();
		while (tallies_p->second.tallyinaphantom_count.end() != onetally_p) // 遍历TallyInfoInAPhantom
		{
			std::map<int, std::pair<float, float>>::iterator onecelltally_p;
			onecelltally_p = onetally_p->second.valerr_count.begin();
			if (true == onetally_p->second.isStar)
			{
				std::cout << "*";
			}
			else
			{
				std::cout << " ";
			}
			std::cout << std::setw(10) << onetally_p->first;  // tally type
			bool isCellSingle = true;
			while (onetally_p->second.valerr_count.end() != onecelltally_p)  //遍历TallyInfo
			{
				if (isCellSingle)
				{
					std::cout << std::setw(5) << onecelltally_p->first
						<< std::setw(15) << std::setprecision(5) << (onecelltally_p->second).first
						<< std::setw(15) << std::setprecision(5) << (onecelltally_p->second).second
						<< std::endl;

					isCellSingle = false;
				}
				else
				{
					std::cout << "           " << std::setw(5) << onecelltally_p->first
						<< std::setw(15) << std::setprecision(5) << (onecelltally_p->second).first
						<< std::setw(15) << std::setprecision(5) << (onecelltally_p->second).second
						<< std::endl;
				}
				onecelltally_p++;
			}
			onetally_p++;
		}
		
		return 0;
	}
	else
	{
		return -1;
	}
	
}

// 器官输入顺序无所谓,不包括红骨髓部分
float *calDCCKWithOutShow(int organleft, int organright, std::string agetmp, AllInfo InfoForAll)
{
	// 获得器官的质量信息
	float organleftmass = 0;
	float organrightmass = 0;

	if (InfoForAll.phantominfo_count_all.end() != InfoForAll.phantominfo_count_all.find(agetmp))
	{
		std::map <std::string, PhantomInfo>::iterator onephantom_p;
		onephantom_p = InfoForAll.phantominfo_count_all.find(agetmp);
		std::map<int, float>::iterator infotmp_p;
		if ((onephantom_p->second).organwet_count.end() != (infotmp_p = (onephantom_p->second.organwet_count.find(organleft)))) // 不存在这个器官
		{
			organleftmass = infotmp_p->second;
		}
		else
		{
			return 0;
		}
		if ((onephantom_p->second).organwet_count.end() != (infotmp_p = (onephantom_p->second.organwet_count.find(organright)))) // 不存在这个器官
		{
			organrightmass = infotmp_p->second;
		}
		else
		{
			return 0;
		}
	}
	else // 不存在这个年龄段
	{
		return 0;
	}

	// 创建储存转换系数的数组
	float* conversioncoefficients = new float[dirall.size() * ergall.size()]; // 能量作为列，方向作为行

																			  // DCC? = Dt / ? = 1.602 * 1e2 * ES / m 
																			  // ?通量， E MeV, S cm2, m g, DCC? pGy*cm^2
																			  // DCCk = Dt/Ka = DCC? / (Ka/?)
																			  // (Ka/?) 对应于 KERMAFREEINAIR的数值

	float dcctongliang = 0;
	for (int i = 0; i < ergall.size(); i++)  // 能量遍历
	{
		for (int j = 0; j < dirall.size(); j++)  // 方向遍历
		{
			std::string singleinstence; singleinstence.clear();
			float tallyforleft = 0; float tallyforright = 0;
			singleinstence = agetmp + "_" + dirall[j] + "_" + ergall[i];
			std::map<std::string, TallyInfoInAPhantom>::iterator tallinaphantom_p;
			if (InfoForAll.tally_count_all.end() != (tallinaphantom_p = InfoForAll.tally_count_all.find(singleinstence))) // 找到这个文件
			{
				std::map<int, TallyInfo>::iterator onetall_p;
				if (tallinaphantom_p->second.tallyinaphantom_count.end() != (onetall_p = tallinaphantom_p->second.tallyinaphantom_count.find(1008)))
				{
					if (onetall_p->second.valerr_count.end() != onetall_p->second.valerr_count.find(organleft))
					{
						tallyforleft = (onetall_p->second.valerr_count.find(organleft))->second.first;
					}
					if (onetall_p->second.valerr_count.end() != onetall_p->second.valerr_count.find(organright))
					{
						tallyforright = (onetall_p->second.valerr_count.find(organright))->second.first;
					}
				}
			}
			// 至此完成两个器官的f8的读数的确定
			dcctongliang = (tallyforleft + tallyforright) / (organleftmass + organrightmass) * 1.602 * 100 * SOURCEAREA[j];
			conversioncoefficients[j + i * dirall.size()] = dcctongliang;  // DCC?
		}
	}

	// DCCk的求取
	for (int i = 0; i < ergall.size(); i++)  // 能量遍历
	{
		for (int j = 0; j < dirall.size(); j++)  // 方向遍历
		{
			conversioncoefficients[j + i * dirall.size()] = conversioncoefficients[j + i * dirall.size()] / KERMAFREEINAIR[i];
		}
	}

	// 输出部分
	// std::cout << "------------------" << std::endl;
	// std::cout << "Organ     " << organleft << "  +  " << organright << std::endl;
	// std::cout << "Erenge(MeV)       AP       PA     LLAT     RLAT      ROT      ISO" << std::endl;
	// for (int i = 0; i < ergall.size(); i++)  // 能量遍历
	// {
	// 	std::cout << std::setw(11) << std::setprecision(3) << std::left << ergall_val[i];
	// 	for (int j = 0; j < dirall.size(); j++)  // 方向遍历
	// 	{
	// 		std::cout << std::setw(9) << std::setprecision(3) << std::right << conversioncoefficients[j + i * dirall.size()];
	// 	}
	// 	std::cout << std::endl;
	// }


	return conversioncoefficients;
}

// 器官输入顺序无所谓,不包括红骨髓部分
float *calDCCKWithOutShow(int organ, std::string agetmp, AllInfo InfoForAll)
{
	// 获得器官的质量信息
	float organmass = 0;

	if (InfoForAll.phantominfo_count_all.end() != InfoForAll.phantominfo_count_all.find(agetmp))
	{
		std::map <std::string, PhantomInfo>::iterator onephantom_p;
		onephantom_p = InfoForAll.phantominfo_count_all.find(agetmp);
		std::map<int, float>::iterator infotmp_p;
		if ((onephantom_p->second).organwet_count.end() != (infotmp_p = (onephantom_p->second.organwet_count.find(organ)))) // 不存在这个器官
		{
			organmass = infotmp_p->second;
		}
		else
		{
			return 0;
		}
	}
	else // 不存在这个年龄段
	{
		return 0;
	}

	// 创建储存转换系数的数组
	float* conversioncoefficients = new float[dirall.size() * ergall.size()]; // 能量作为列，方向作为行

																			  // DCC? = Dt / ? = 1.602 * 1e2 * ES / m 
																			  // ?通量， E MeV, S cm2, m g, DCC? pGy*cm^2
																			  // DCCk = Dt/Ka = DCC? / (Ka/?)
																			  // (Ka/?) 对应于 KERMAFREEINAIR的数值

	float dcctongliang = 0;
	for (int i = 0; i < ergall.size(); i++)  // 能量遍历
	{
		for (int j = 0; j < dirall.size(); j++)  // 方向遍历
		{
			std::string singleinstence; singleinstence.clear();
			float tallyfororgan = 0;
			singleinstence = agetmp + "_" + dirall[j] + "_" + ergall[i];
			std::map<std::string, TallyInfoInAPhantom>::iterator tallinaphantom_p;
			if (InfoForAll.tally_count_all.end() != (tallinaphantom_p = InfoForAll.tally_count_all.find(singleinstence))) // 找到这个文件
			{
				std::map<int, TallyInfo>::iterator onetall_p;
				if (tallinaphantom_p->second.tallyinaphantom_count.end() != (onetall_p = tallinaphantom_p->second.tallyinaphantom_count.find(1008)))
				{
					if (onetall_p->second.valerr_count.end() != onetall_p->second.valerr_count.find(organ))
					{
						tallyfororgan = (onetall_p->second.valerr_count.find(organ))->second.first;
					}
				}
			}
			// 至此完成两个器官的f8的读数的确定
			dcctongliang = tallyfororgan / organmass * 1.602 * 100 * SOURCEAREA[j];
			conversioncoefficients[j + i * dirall.size()] = dcctongliang;  // DCC?
		}
	}

	// DCCk的求取
	for (int i = 0; i < ergall.size(); i++)  // 能量遍历
	{
		for (int j = 0; j < dirall.size(); j++)  // 方向遍历
		{
			conversioncoefficients[j + i * dirall.size()] = conversioncoefficients[j + i * dirall.size()] / KERMAFREEINAIR[i];
		}
	}

	// 输出部分
	// std::cout << "------------------" << std::endl;
	// std::cout << "Organ    " << organ << std::endl;
	// std::cout << "Erenge(MeV)       AP       PA     LLAT     RLAT      ROT      ISO" << std::endl;
	// for (int i = 0; i < ergall.size(); i++)  // 能量遍历
	// {
	// 	std::cout << std::setw(11) << std::setprecision(3) << std::left << ergall_val[i];
	// 	for (int j = 0; j < dirall.size(); j++)  // 方向遍历
	// 	{
	// 		std::cout << std::setw(9) << std::setprecision(3) << std::right << conversioncoefficients[j + i * dirall.size()];
	// 	}
	// 	std::cout << std::endl;
	// }


	return conversioncoefficients;
}


int calDCCK(int organleft, int organright, std::string agetmp, AllInfo InfoForAll, std::string outputfilepath)
{
	// 获得器官的质量信息
	float organleftmass = 0;
	float organrightmass = 0;

	if (InfoForAll.phantominfo_count_all.end() != InfoForAll.phantominfo_count_all.find(agetmp))
	{
		std::map <std::string, PhantomInfo>::iterator onephantom_p;
		onephantom_p = InfoForAll.phantominfo_count_all.find(agetmp);
		std::map<int, float>::iterator infotmp_p;
		if ((onephantom_p->second).organwet_count.end() != (infotmp_p = (onephantom_p->second.organwet_count.find(organleft)))) // 不存在这个器官
		{
			organleftmass = infotmp_p->second;
		}
		else
		{
			return -1;
		}
		if ((onephantom_p->second).organwet_count.end() != (infotmp_p = (onephantom_p->second.organwet_count.find(organright)))) // 不存在这个器官
		{
			organrightmass = infotmp_p->second;
		}
		else
		{
			return -1;
		}
	}
	else // 不存在这个年龄段
	{
		return -1;
	}

	// 创建储存转换系数的数组
	float* conversioncoefficients = new float[dirall.size() * ergall.size()]; // 能量作为列，方向作为行

																			  // DCC? = Dt / ? = 1.602 * 1e2 * ES / m 
																			  // ?通量， E MeV, S cm2, m g, DCC? pGy*cm^2
																			  // DCCk = Dt/Ka = DCC? / (Ka/?)
																			  // (Ka/?) 对应于 KERMAFREEINAIR的数值

	float dcctongliang = 0;
	for (int i = 0; i < ergall.size(); i++)  // 能量遍历
	{
		for (int j = 0; j < dirall.size(); j++)  // 方向遍历
		{
			std::string singleinstence; singleinstence.clear();
			float tallyforleft = 0; float tallyforright = 0;
			singleinstence = agetmp + "_" + dirall[j] + "_" + ergall[i];
			std::map<std::string, TallyInfoInAPhantom>::iterator tallinaphantom_p;
			if (InfoForAll.tally_count_all.end() != (tallinaphantom_p = InfoForAll.tally_count_all.find(singleinstence))) // 找到这个文件
			{
				std::map<int, TallyInfo>::iterator onetall_p;
				if (tallinaphantom_p->second.tallyinaphantom_count.end() != (onetall_p = tallinaphantom_p->second.tallyinaphantom_count.find(1008)))
				{
					if (onetall_p->second.valerr_count.end() != onetall_p->second.valerr_count.find(organleft))
					{
						tallyforleft = (onetall_p->second.valerr_count.find(organleft))->second.first;
					}
					if (onetall_p->second.valerr_count.end() != onetall_p->second.valerr_count.find(organright))
					{
						tallyforright = (onetall_p->second.valerr_count.find(organright))->second.first;
					}
				}
			}
			// 至此完成两个器官的f8的读数的确定
			dcctongliang = (tallyforleft + tallyforright) / (organleftmass + organrightmass) * 1.602 * 100 * SOURCEAREA[j];
			// dcctongliang = (tallyforleft + tallyforright) / (organleftmass + organrightmass) * 1.602 * 100 * SOURCEAREA[j];
			conversioncoefficients[j + i * dirall.size()] = dcctongliang;  // DCC?
		}
	}

	// DCCk的求取
	for (int i = 0; i < ergall.size(); i++)  // 能量遍历
	{
		for (int j = 0; j < dirall.size(); j++)  // 方向遍历
		{
			conversioncoefficients[j + i * dirall.size()] = conversioncoefficients[j + i * dirall.size()] / KERMAFREEINAIR[i];
		}
	}

	// 输出部分
	std::cout << "------------------" << std::endl;
	std::cout << "Organ     " << organleft << "  +  " << organright << std::endl;
	std::cout << "Erenge(MeV)       AP       PA     LLAT     RLAT      ROT      ISO" << std::endl;
	for (int i = 0; i < ergall.size(); i++)  // 能量遍历
	{
		std::cout << std::setw(11) << std::setprecision(3) << std::left << ergall_val[i];
		for (int j = 0; j < dirall.size(); j++)  // 方向遍历
		{
			std::cout << std::setw(9) << std::setprecision(3) << std::right << conversioncoefficients[j + i * dirall.size()];
		}
		std::cout << std::endl;
	}

	// 输出到文件
	std::fstream fileout;
	fileout.open(outputfilepath, std::ios::out);
	if (fileout.fail()) // 不能打开文件
	{
		return -2;
	}

	fileout << "------------------" << '\n';
	fileout << "Organ    " << organleft << " + " << organright << '\n';
	fileout << "Erenge(MeV)       AP       PA     LLAT     RLAT      ROT      ISO" << '\n';
	for (int i = 0; i < ergall.size(); i++)  // 能量遍历
	{
		fileout << std::setw(11) << std::setprecision(3) << std::left << ergall_val[i];
		for (int j = 0; j < dirall.size(); j++)  // 方向遍历
		{
			fileout << std::setw(9) << std::setprecision(3) << std::right << conversioncoefficients[j + i * dirall.size()];
		}
		fileout << std::endl;
	}

	return 0;
}
// 单个文件输出到路径outputfilepath
int calDCCK(int organ, std::string agetmp, AllInfo InfoForAll, std::string outputfilepath)
{
	// 获得器官的质量信息
	float organmass = 0;

	if (InfoForAll.phantominfo_count_all.end() != InfoForAll.phantominfo_count_all.find(agetmp))
	{
		std::map <std::string, PhantomInfo>::iterator onephantom_p;
		onephantom_p = InfoForAll.phantominfo_count_all.find(agetmp);
		std::map<int, float>::iterator infotmp_p;
		if ((onephantom_p->second).organwet_count.end() != (infotmp_p = (onephantom_p->second.organwet_count.find(organ)))) // 不存在这个器官
		{
			organmass = infotmp_p->second;
		}
		else
		{
			return -1;
		}
	}
	else // 不存在这个年龄段
	{
		return -1;
	}

	// 创建储存转换系数的数组
	float* conversioncoefficients = new float[dirall.size() * ergall.size()]; // 能量作为列，方向作为行

																			  // DCC? = Dt / ? = 1.602 * 1e2 * ES / m 
																			  // ?通量， E MeV, S cm2, m g, DCC? pGy*cm^2
																			  // DCCk = Dt/Ka = DCC? / (Ka/?)
																			  // (Ka/?) 对应于 KERMAFREEINAIR的数值

	float dcctongliang = 0;
	for (int i = 0; i < ergall.size(); i++)  // 能量遍历
	{
		for (int j = 0; j < dirall.size(); j++)  // 方向遍历
		{
			std::string singleinstence; singleinstence.clear();
			float tallyfororgan = 0;
			singleinstence = agetmp + "_" + dirall[j] + "_" + ergall[i];
			std::map<std::string, TallyInfoInAPhantom>::iterator tallinaphantom_p;
			if (InfoForAll.tally_count_all.end() != (tallinaphantom_p = InfoForAll.tally_count_all.find(singleinstence))) // 找到这个文件
			{
				std::map<int, TallyInfo>::iterator onetall_p;
				if (tallinaphantom_p->second.tallyinaphantom_count.end() != (onetall_p = tallinaphantom_p->second.tallyinaphantom_count.find(1008)))
				{
					if (onetall_p->second.valerr_count.end() != onetall_p->second.valerr_count.find(organ))
					{
						tallyfororgan = (onetall_p->second.valerr_count.find(organ))->second.first;
					}
				}
			}
			// 至此完成两个器官的f8的读数的确定
			dcctongliang = tallyfororgan / organmass * 1.602 * 100 * SOURCEAREA[j];
			conversioncoefficients[j + i * dirall.size()] = dcctongliang;  // DCC?
		}
	}

	// DCCk的求取
	for (int i = 0; i < ergall.size(); i++)  // 能量遍历
	{
		for (int j = 0; j < dirall.size(); j++)  // 方向遍历
		{
			conversioncoefficients[j + i * dirall.size()] = conversioncoefficients[j + i * dirall.size()] / KERMAFREEINAIR[i];
		}
	}

	// 输出部分
	std::cout << "------------------" << std::endl;
	std::cout << "Organ    " << organ << std::endl;
	std::cout << "Erenge(MeV)       AP       PA     LLAT     RLAT      ROT      ISO" << std::endl;
	for (int i = 0; i < ergall.size(); i++)  // 能量遍历
	{
		std::cout << std::setw(11) << std::setprecision(3) << std::left << ergall_val[i];
		for (int j = 0; j < dirall.size(); j++)  // 方向遍历
		{
			std::cout << std::setw(9) << std::setprecision(3) << std::right << conversioncoefficients[j + i * dirall.size()];
		}
		std::cout << std::endl;
	}

	// 输出到文件
	std::fstream fileout;
	fileout.open(outputfilepath, std::ios::out);
	if (fileout.fail()) // 不能打开文件
	{
		return -2;
	}

	fileout << "------------------" << '\n';
	fileout << "Organ    " << organ << '\n';
	fileout << "Erenge(MeV)       AP       PA     LLAT     RLAT      ROT      ISO" << '\n';
	for (int i = 0; i < ergall.size(); i++)  // 能量遍历
	{
		fileout << std::setw(11) << std::setprecision(3) << std::left << ergall_val[i];
		for (int j = 0; j < dirall.size(); j++)  // 方向遍历
		{
			fileout << std::setw(9) << std::setprecision(3) << std::right << conversioncoefficients[j + i * dirall.size()];
		}
		fileout << std::endl;
	}


	return 0;
}

int calDCCKRBMWithShow(PhantomAgeDirErg instancename, AllInfo InfoForAll)
{
	// only cell 14 25 27 29 40 42 44 45 46  50  52  54  56
	// 对应tally 14 24 34 44 54 64 74 84 94 101 114 124 134
	const int tallyNUM = 13;
	int tallysum[tallyNUM] = { 14, 24, 34, 44, 54, 64, 74, 84, 94, 104, 114, 124, 134 };
	float tallyvalue[tallyNUM] = { 0 };
	float sumpercentRBM = 0;
	if (!instancename.ageGet().compare("am") || !instancename.ageGet().compare("af") || !instancename.ageGet().compare("rpiaf") || !instancename.ageGet().compare("rpiam"))
	{
		for (int i = 0; i < FractionRBMForAdult.size(); i++)
		{
			sumpercentRBM += FractionRBMForAdult[i];
		}
	}
	else if (!instancename.ageGet().compare("15m") || !instancename.ageGet().compare("15f"))
	{
		for (int i = 0; i < FractionRBMForAdult.size(); i++)
		{
			sumpercentRBM += FractionRBMFor15[i];
		}
	}
	else if (!instancename.ageGet().compare("10m") || !instancename.ageGet().compare("10f"))
	{
		for (int i = 0; i < FractionRBMForAdult.size(); i++)
		{
			sumpercentRBM += FractionRBMFor10[i];
		}
	}
	else if (!instancename.ageGet().compare("5m") || !instancename.ageGet().compare("5f"))
	{
		for (int i = 0; i < FractionRBMForAdult.size(); i++)
		{
			sumpercentRBM += FractionRBMFor5[i];
		}
	}
	else
	{
		return -1;
	}

	float absorbdose = 0;
	std::map< std::string, TallyInfoInAPhantom>::iterator phantomtally_p;
	if (InfoForAll.tally_count_all.end() == (phantomtally_p = InfoForAll.tally_count_all.find(instancename.name)))
	{
		return -1;
	}
	else
	{
		std::map<int, TallyInfo>::iterator onttally_p;
		for (int i = 0; i < tallyNUM; i++)
		{
			if (phantomtally_p->second.tallyinaphantom_count.end() == (onttally_p = phantomtally_p->second.tallyinaphantom_count.find(tallysum[i])))
			{
				return -1;
			}
			else
			{
				tallyvalue[i] = (onttally_p->second.valerr_count.begin()->second.first);
			}
		}
	}

	// 至此完成所有f4的记录，存放在tallyvalue中
	if (!instancename.ageGet().compare("am") || !instancename.ageGet().compare("af") || !instancename.ageGet().compare("rpiaf") || !instancename.ageGet().compare("rpiam"))
	{
		for (int i = 0; i < tallyNUM; i++)
		{
			absorbdose += tallyvalue[i] * FractionRBMForAdult[i] / sumpercentRBM;
		}
	}
	else if (!instancename.ageGet().compare("15m") || !instancename.ageGet().compare("15f"))
	{
		for (int i = 0; i < tallyNUM; i++)
		{
			absorbdose += tallyvalue[i] * FractionRBMFor15[i] / sumpercentRBM;
		}
	}
	else if (!instancename.ageGet().compare("10m") || !instancename.ageGet().compare("10f"))
	{
		for (int i = 0; i < tallyNUM; i++)
		{
			absorbdose += tallyvalue[i] * FractionRBMFor10[i] / sumpercentRBM;
		}
	}
	else if (!instancename.ageGet().compare("5m") || !instancename.ageGet().compare("5f"))
	{
		for (int i = 0; i < tallyNUM; i++)
		{
			absorbdose += tallyvalue[i] * FractionRBMFor5[i] / sumpercentRBM;
		}
	}
	else
	{
		return -1;
	}

	// 
	std::cout << "-----------------" << std::endl;
	std::cout << "Absorb dose for REM in phantom " << instancename.name << "equals to: " << absorbdose << std::endl;

	std::string particleenerge; int energeindex = 0;
	std::stringstream sstmp; sstmp.clear();
	sstmp.str(instancename.ergGet());
	sstmp >> energeindex;
	particleenerge = ergall_val[energeindex];
	std::string particledir; int dirindex = 0;
	sstmp.clear();
	sstmp.str(instancename.dirGet());
	sstmp >> particledir;
	for (int i = 0; i < dirall.size(); i++)
	{
		if (0 == dirall[i].find(particledir))
		{
			dirindex = i;
			break;
		}
	}

	float airkarthiserg = 0;
	airkarthiserg = KERMAFREEINAIR[energeindex];

	float dcck = 0;
	dcck = absorbdose * SOURCEAREA[dirindex] / KERMAFREEINAIR[energeindex] * 1.602 * 1e2;  // 

	std::cout << "-----------------" << std::endl;
	std::cout << "Absorbed dose per unit kerma free-in-air for " << instancename.name << "equals to:" << dcck << std::endl;

	return 0;
}

float calDCCKRBMWithoutShow(PhantomAgeDirErg instancename, AllInfo InfoForAll)
{
	// only cell 14 25 27 29 40 42 44 45 46  50  52  54  56
	// 对应tally 14 24 34 44 54 64 74 84 94 101 114 124 134
	int energeindex = 0;
	std::stringstream sstmp; sstmp.clear();
	sstmp.str(instancename.ergGet());
	sstmp >> energeindex;

	const int tallyNUM = 13;
	int tallysum[tallyNUM] = { 14, 24, 34, 44, 54, 64, 74, 84, 94, 104, 114, 124, 134 };
	float tallyvalue[tallyNUM] = { 0 };
	float sumpercentRBM = 0;
	
	if (!instancename.ageGet().compare("am") || !instancename.ageGet().compare("af") || !instancename.ageGet().compare("rpiaf") || !instancename.ageGet().compare("rpiam"))
	{
		for (int i = 0; i < FractionRBMForAdult.size(); i++)
		{
			sumpercentRBM += FractionRBMForAdult[i];
		}
	}
	else if (!instancename.ageGet().compare("15m") || !instancename.ageGet().compare("15f"))
	{
		for (int i = 0; i < FractionRBMForAdult.size(); i++)
		{
			sumpercentRBM += FractionRBMFor15[i];
		}
	}
	else if (!instancename.ageGet().compare("10m") || !instancename.ageGet().compare("10f"))
	{
		for (int i = 0; i < FractionRBMForAdult.size(); i++)
		{
			sumpercentRBM += FractionRBMFor10[i];
		}
	}
	else if (!instancename.ageGet().compare("5m") || !instancename.ageGet().compare("5f"))
	{
		for (int i = 0; i < FractionRBMForAdult.size(); i++)
		{
			sumpercentRBM += FractionRBMFor5[i];
		}
	}
	else
	{
		return -1;
	}

	float absorbdose = 0;
	std::map< std::string, TallyInfoInAPhantom>::iterator phantomtally_p;
	if (InfoForAll.tally_count_all.end() == (phantomtally_p = InfoForAll.tally_count_all.find(instancename.name)))
	{
		return -1;
	}
	else
	{
		std::map<int, TallyInfo>::iterator onttally_p;
		for (int i = 0; i < tallyNUM; i++)
		{
			if (phantomtally_p->second.tallyinaphantom_count.end() == (onttally_p = phantomtally_p->second.tallyinaphantom_count.find(tallysum[i])))
			{
				return -1;
			}
			else
			{
				tallyvalue[i] = (onttally_p->second.valerr_count.begin()->second.first);
			}
		}
	}

	// 至此完成所有f4的记录，存放在tallyvalue中
	if (!instancename.ageGet().compare("am") || !instancename.ageGet().compare("af") || !instancename.ageGet().compare("rpiaf") || !instancename.ageGet().compare("rpiam"))
	{
		for (int i = 0; i < tallyNUM; i++)
		{
			absorbdose += tallyvalue[i] * FractionRBMForAdult[i] / sumpercentRBM;
		}
	}
	else if (!instancename.ageGet().compare("15m") || !instancename.ageGet().compare("15f"))
	{
		for (int i = 0; i < tallyNUM; i++)
		{
			absorbdose += tallyvalue[i] * FractionRBMFor15[i] / sumpercentRBM;
		}
	}
	else if (!instancename.ageGet().compare("10m") || !instancename.ageGet().compare("10f"))
	{
		for (int i = 0; i < tallyNUM; i++)
		{
			absorbdose += tallyvalue[i] * FractionRBMFor10[i] / sumpercentRBM;
		}
	}
	else if (!instancename.ageGet().compare("5m") || !instancename.ageGet().compare("5f"))
	{
		for (int i = 0; i < tallyNUM; i++)
		{
			absorbdose += tallyvalue[i] * FractionRBMFor5[i] / sumpercentRBM;
		}
	}
	else
	{
		return -1;
	}

	// 
	// std::cout << "-----------------" << std::endl;
	// std::cout << "Absorb dose for REM in phantom " << instancename.name << "equals to: " << absorbdose << std::endl;

	std::string particleenerge; 
	particleenerge = ergall_val[energeindex];
	std::string particledir; int dirindex = 0;
	sstmp.clear();
	sstmp.str(instancename.dirGet());
	sstmp >> particledir;
	for (int i = 0; i < dirall.size(); i++)
	{
		if (0 == dirall[i].find(particledir))
		{
			dirindex = i;
			break;
		}
	}

	float airkarthiserg = 0;
	airkarthiserg = KERMAFREEINAIR[energeindex];

	float dcck = 0;
	dcck = absorbdose * SOURCEAREA[dirindex] / KERMAFREEINAIR[energeindex] * 160.22;  // 

	return dcck;
}

// 性别的平均值
float calEffectiveDose(PhantomAgeDirErg instancename, AllInfo InfoForAll)
{
	// instancename1 & instancename1 represent male & female respectively
	int dirindex; int ergindex;
	for (int i = 0; i < dirall.size(); i++) // 记录方向
	{
		if (0 == dirall[i].compare(instancename.dirGet()))
		{
			dirindex = i;
			break;
		}
	}
	for (int i = 0; i < ergall.size(); i++) // 记录方向
	{
		if (0 == ergall[i].compare(instancename.ergGet()))
		{
			ergindex = i;
			break;
		}
	}

	PhantomAgeDirErg instancename2; // for female
	PhantomAgeDirErg instancename1; // for male
	std::string agechange; agechange.clear();
	if (instancename.ageGet()[instancename.ageGet().length() - 1] == 'f')
	{
		agechange.append(instancename.ageGet(), 0, instancename.ageGet().length() - 1);
		agechange.append("m");
		agechange = agechange + "_" + instancename.dirGet() + "_" + instancename.ergGet();
		instancename1.name = agechange;
		instancename2.name = instancename.name;
	}
	else
	{
		agechange.append(instancename.ageGet(), 0 , instancename.ageGet().length() - 1);
		agechange.append("f");
		agechange = agechange + "_" + instancename.dirGet() + "_" + instancename.ergGet();
		instancename2.name = agechange;
		instancename1.name = instancename.name;
	}



	// 红骨髓
	float rbmeqdoseMale = calDCCKRBMWithoutShow(instancename1, InfoForAll);
	float rbmeqdoseFemale = calDCCKRBMWithoutShow(instancename2, InfoForAll);
	float rbmeqdose = (rbmeqdoseFemale + rbmeqdoseMale) / 2;

	// 结肠 76 78 80 82 84
	float coloneqdoseMale = 0; float coloneqdoseFemale = 0;
	float organmass1[5] = { getSingleOrganWet(instancename1, InfoForAll, 76), getSingleOrganWet(instancename1, InfoForAll, 78),
		getSingleOrganWet(instancename1, InfoForAll, 80), getSingleOrganWet(instancename1, InfoForAll, 82), getSingleOrganWet(instancename1, InfoForAll, 84)};
	float organmass2[5] = { getSingleOrganWet(instancename2, InfoForAll, 76), getSingleOrganWet(instancename2, InfoForAll, 78),
		getSingleOrganWet(instancename2, InfoForAll, 80), getSingleOrganWet(instancename2, InfoForAll, 82), getSingleOrganWet(instancename2, InfoForAll, 84) };
	float ergdeposit1[5] = { getSingleOrganErg(instancename1, InfoForAll, 76).first, getSingleOrganErg(instancename1, InfoForAll, 78).first,
		getSingleOrganErg(instancename1, InfoForAll, 80).first, getSingleOrganErg(instancename1, InfoForAll, 82).first, getSingleOrganErg(instancename1, InfoForAll, 84).first};
	float ergdeposit2[5] = { getSingleOrganErg(instancename2, InfoForAll, 76).first, getSingleOrganErg(instancename2, InfoForAll, 78).first,
		getSingleOrganErg(instancename2, InfoForAll, 80).first, getSingleOrganErg(instancename2, InfoForAll, 82).first, getSingleOrganErg(instancename2, InfoForAll, 84).first };
	
	coloneqdoseMale = (ergdeposit1[0] + ergdeposit1[1] + ergdeposit1[2] + ergdeposit1[3] + ergdeposit1[4])
		/ (organmass1[0] + organmass1[1] + organmass1[2] + organmass1[3] + organmass1[4]);

	coloneqdoseFemale = (ergdeposit2[0] + ergdeposit2[1] + ergdeposit2[2] + ergdeposit2[3] + ergdeposit2[4])
		/ (organmass2[0] + organmass2[1] + organmass2[2] + organmass2[3] + organmass2[4]);

	coloneqdoseMale = SOURCEAREA[dirindex] * coloneqdoseMale * 160.22;
	coloneqdoseFemale = SOURCEAREA[dirindex] * coloneqdoseFemale * 160.22;

	float coloneqdose = (coloneqdoseMale + coloneqdoseFemale) / 2 / KERMAFREEINAIR[ergindex];

	//  肺
	float lungdoseMale = 0; float lungdoseFemale = 0;
	lungdoseMale = (getSingleOrganErg(instancename1, InfoForAll, 97).first + getSingleOrganErg(instancename1, InfoForAll, 99).first)
		/ (getSingleOrganWet(instancename1, InfoForAll, 97) + getSingleOrganWet(instancename1, InfoForAll, 99));
	lungdoseFemale = (getSingleOrganErg(instancename2, InfoForAll, 97).first + getSingleOrganErg(instancename2, InfoForAll, 99).first)
		/ (getSingleOrganWet(instancename2, InfoForAll, 97) + getSingleOrganWet(instancename2, InfoForAll, 99));
	
	lungdoseMale = lungdoseMale * 160.22 * SOURCEAREA[dirindex];
	lungdoseFemale = lungdoseFemale * 160.22 * SOURCEAREA[dirindex];

	float lungdose = (lungdoseMale + lungdoseFemale) / 2 / KERMAFREEINAIR[ergindex];

	// 胃 
	float stomachdoseMale = getSingleOrganErg(instancename1, InfoForAll, 72).first / getSingleOrganWet(instancename1, InfoForAll, 72);
	float stomachdoseFemale = getSingleOrganErg(instancename2, InfoForAll, 72).first / getSingleOrganWet(instancename2, InfoForAll, 72);
	stomachdoseMale = stomachdoseMale * 160.22 * SOURCEAREA[dirindex];
	stomachdoseFemale = stomachdoseFemale * 160.22 * SOURCEAREA[dirindex];
	float stomachdose = (stomachdoseMale + stomachdoseFemale) / 2 / KERMAFREEINAIR[ergindex];

	// 乳腺 63 65
	float breastdoseMale = 0; float breastdoseFemale = 0;
	breastdoseMale = (getSingleOrganErg(instancename1, InfoForAll, 63).first + getSingleOrganErg(instancename1, InfoForAll, 65).first)
		/ (getSingleOrganWet(instancename1, InfoForAll, 63) + getSingleOrganWet(instancename1, InfoForAll, 65));
	breastdoseFemale = (getSingleOrganErg(instancename2, InfoForAll, 63).first + getSingleOrganErg(instancename2, InfoForAll, 65).first)
		/ (getSingleOrganWet(instancename2, InfoForAll, 63) + getSingleOrganWet(instancename2, InfoForAll, 65));
	breastdoseMale = breastdoseMale * 160.22 * SOURCEAREA[dirindex];
	breastdoseFemale = breastdoseFemale * 160.22 * SOURCEAREA[dirindex];
	float breastdose = (breastdoseMale + breastdoseFemale) / 2 / KERMAFREEINAIR[ergindex];

	// 其余组织
	float residualdoseMale[13] = { 0 }; float residualdoseFemale[13] = { 0 };
    // male
	residualdoseMale[0] = (getSingleOrganErg(instancename1, InfoForAll, 1).first + getSingleOrganErg(instancename1, InfoForAll, 2).first) /
		(getSingleOrganWet(instancename1, InfoForAll, 1) + getSingleOrganWet(instancename1, InfoForAll, 2));
	residualdoseMale[1] = getSingleOrganErg(instancename1, InfoForAll, 3).first /  getSingleOrganWet(instancename1, InfoForAll, 3);
	residualdoseMale[2] = getSingleOrganErg(instancename1, InfoForAll, 70).first / getSingleOrganWet(instancename1, InfoForAll, 70);
	residualdoseMale[3] = getSingleOrganErg(instancename1, InfoForAll, 87).first / getSingleOrganWet(instancename1, InfoForAll, 87);
	residualdoseMale[4] = (getSingleOrganErg(instancename1, InfoForAll, 89).first + getSingleOrganErg(instancename1, InfoForAll, 90).first +
		getSingleOrganErg(instancename1, InfoForAll, 91).first + getSingleOrganErg(instancename1, InfoForAll, 92).first +
		getSingleOrganErg(instancename1, InfoForAll, 93).first + getSingleOrganErg(instancename1, InfoForAll, 94).first) /
		(getSingleOrganWet(instancename1, InfoForAll, 89) + getSingleOrganWet(instancename1, InfoForAll, 90) +
			getSingleOrganWet(instancename1, InfoForAll, 91) + getSingleOrganWet(instancename1, InfoForAll, 92) +
			getSingleOrganWet(instancename1, InfoForAll, 93) + getSingleOrganWet(instancename1, InfoForAll, 94));
	residualdoseMale[5] = (getSingleOrganErg(instancename1, InfoForAll, 100).first + getSingleOrganErg(instancename1, InfoForAll, 101).first +
		getSingleOrganErg(instancename1, InfoForAll, 102).first + getSingleOrganErg(instancename1, InfoForAll, 103).first +
		getSingleOrganErg(instancename1, InfoForAll, 104).first + getSingleOrganErg(instancename1, InfoForAll, 105).first) /
		(getSingleOrganWet(instancename1, InfoForAll, 100) + getSingleOrganWet(instancename1, InfoForAll, 101) +
			getSingleOrganWet(instancename1, InfoForAll, 102) + getSingleOrganWet(instancename1, InfoForAll, 103) +
			getSingleOrganWet(instancename1, InfoForAll, 104) + getSingleOrganWet(instancename1, InfoForAll, 105));
	residualdoseMale[6] = (getSingleOrganErg(instancename1, InfoForAll, 106).first + getSingleOrganErg(instancename1, InfoForAll, 107).first +
		getSingleOrganErg(instancename1, InfoForAll, 108).first + getSingleOrganErg(instancename1, InfoForAll, 109).first) /
		(getSingleOrganWet(instancename1, InfoForAll, 106) + getSingleOrganWet(instancename1, InfoForAll, 107) +
			getSingleOrganWet(instancename1, InfoForAll, 108) + getSingleOrganWet(instancename1, InfoForAll, 109));
	residualdoseMale[7] = getSingleOrganErg(instancename1, InfoForAll, 5).first / getSingleOrganWet(instancename1, InfoForAll, 5);
	residualdoseMale[8] = getSingleOrganErg(instancename1, InfoForAll, 113).first / getSingleOrganWet(instancename1, InfoForAll, 113);
	residualdoseMale[9] = getSingleOrganErg(instancename1, InfoForAll, 75).first / getSingleOrganWet(instancename1, InfoForAll, 75);
	residualdoseMale[10] = getSingleOrganErg(instancename1, InfoForAll, 127).first / getSingleOrganWet(instancename1, InfoForAll, 127);
	residualdoseMale[11] = getSingleOrganErg(instancename1, InfoForAll, 131).first / getSingleOrganWet(instancename1, InfoForAll, 131);
	residualdoseMale[12] = getSingleOrganErg(instancename1, InfoForAll, 115).first / getSingleOrganWet(instancename1, InfoForAll, 115); // 前列腺
																																		// male
	// female
	residualdoseFemale[0] = (getSingleOrganErg(instancename2, InfoForAll, 1).first + getSingleOrganErg(instancename2, InfoForAll, 2).first) /
		(getSingleOrganWet(instancename2, InfoForAll, 1) + getSingleOrganWet(instancename2, InfoForAll, 2));
	residualdoseFemale[1] = getSingleOrganErg(instancename2, InfoForAll, 3).first / getSingleOrganWet(instancename2, InfoForAll, 3);
	residualdoseFemale[2] = getSingleOrganErg(instancename2, InfoForAll, 70).first / getSingleOrganWet(instancename2, InfoForAll, 70);
	residualdoseFemale[3] = getSingleOrganErg(instancename2, InfoForAll, 87).first / getSingleOrganWet(instancename2, InfoForAll, 87);
	residualdoseFemale[4] = (getSingleOrganErg(instancename2, InfoForAll, 89).first + getSingleOrganErg(instancename2, InfoForAll, 90).first +
		getSingleOrganErg(instancename2, InfoForAll, 91).first + getSingleOrganErg(instancename2, InfoForAll, 92).first +
		getSingleOrganErg(instancename2, InfoForAll, 93).first + getSingleOrganErg(instancename2, InfoForAll, 94).first) /
		(getSingleOrganWet(instancename2, InfoForAll, 89) + getSingleOrganWet(instancename2, InfoForAll, 90) +
			getSingleOrganWet(instancename2, InfoForAll, 91) + getSingleOrganWet(instancename2, InfoForAll, 92) +
			getSingleOrganWet(instancename2, InfoForAll, 93) + getSingleOrganWet(instancename2, InfoForAll, 94));
	residualdoseFemale[5] = (getSingleOrganErg(instancename2, InfoForAll, 100).first + getSingleOrganErg(instancename2, InfoForAll, 101).first +
		getSingleOrganErg(instancename2, InfoForAll, 102).first + getSingleOrganErg(instancename2, InfoForAll, 103).first +
		getSingleOrganErg(instancename2, InfoForAll, 104).first + getSingleOrganErg(instancename2, InfoForAll, 105).first) /
		(getSingleOrganWet(instancename2, InfoForAll, 100) + getSingleOrganWet(instancename2, InfoForAll, 101) +
			getSingleOrganWet(instancename2, InfoForAll, 102) + getSingleOrganWet(instancename2, InfoForAll, 103) +
			getSingleOrganWet(instancename2, InfoForAll, 104) + getSingleOrganWet(instancename2, InfoForAll, 105));
	residualdoseFemale[6] = (getSingleOrganErg(instancename2, InfoForAll, 106).first + getSingleOrganErg(instancename2, InfoForAll, 107).first +
		getSingleOrganErg(instancename2, InfoForAll, 108).first + getSingleOrganErg(instancename2, InfoForAll, 109).first) /
		(getSingleOrganWet(instancename2, InfoForAll, 106) + getSingleOrganWet(instancename2, InfoForAll, 107) +
			getSingleOrganWet(instancename2, InfoForAll, 108) + getSingleOrganWet(instancename2, InfoForAll, 109));
	residualdoseFemale[7] = getSingleOrganErg(instancename2, InfoForAll, 5).first / getSingleOrganWet(instancename2, InfoForAll, 5);
	residualdoseFemale[8] = getSingleOrganErg(instancename2, InfoForAll, 113).first / getSingleOrganWet(instancename2, InfoForAll, 113);
	residualdoseFemale[9] = getSingleOrganErg(instancename2, InfoForAll, 75).first / getSingleOrganWet(instancename2, InfoForAll, 75);
	residualdoseFemale[10] = getSingleOrganErg(instancename2, InfoForAll, 127).first / getSingleOrganWet(instancename2, InfoForAll, 127);
	residualdoseFemale[11] = getSingleOrganErg(instancename2, InfoForAll, 131).first / getSingleOrganWet(instancename2, InfoForAll, 131);
	residualdoseFemale[12] = getSingleOrganErg(instancename2, InfoForAll, 139).first / getSingleOrganWet(instancename2, InfoForAll, 139); // 前列腺

	float dosesumtmp1 = 0; float dosesumtmp2 = 0;
	for (int i = 0; i < 13; i++)
	{
		dosesumtmp1 += residualdoseMale[i];
	}
	dosesumtmp1 /= 13;
	dosesumtmp1 = dosesumtmp1 * 160.22 * SOURCEAREA[dirindex];
	for (int i = 0; i < 13; i++)
	{
		dosesumtmp2 += residualdoseFemale[i];
	}
	dosesumtmp2 /= 13;
	dosesumtmp2 = dosesumtmp2 * 160.22 * SOURCEAREA[dirindex];

	float residualdose = (dosesumtmp1 + dosesumtmp2) / 2 / KERMAFREEINAIR[ergindex];
	

	// 性腺
	float gonaddoseMale = 0; float gonaddoseFemale = 0;
	gonaddoseMale = (getSingleOrganErg(instancename1, InfoForAll, 129).first + getSingleOrganErg(instancename1, InfoForAll, 130).first)
		/ (getSingleOrganWet(instancename1, InfoForAll, 129) + getSingleOrganWet(instancename1, InfoForAll, 130));
	gonaddoseFemale = (getSingleOrganErg(instancename2, InfoForAll, 111).first + getSingleOrganErg(instancename2, InfoForAll, 112).first)
		/ (getSingleOrganWet(instancename2, InfoForAll, 111) + getSingleOrganWet(instancename2, InfoForAll, 112));
	gonaddoseMale = gonaddoseMale * 160.22 * SOURCEAREA[dirindex];
	gonaddoseFemale = gonaddoseFemale * 160.22 * SOURCEAREA[dirindex];

	float gonaddose = (gonaddoseMale + gonaddoseFemale) / 2 / KERMAFREEINAIR[ergindex];

	// 膀胱
	float urinarydoseMale = 0; float urinarydoseFemale = 0;
	urinarydoseMale = (getSingleOrganErg(instancename1, InfoForAll, 137).first) / (getSingleOrganWet(instancename1, InfoForAll, 137));
	urinarydoseFemale = (getSingleOrganErg(instancename2, InfoForAll, 137).first) / (getSingleOrganWet(instancename2, InfoForAll, 137));
	urinarydoseMale = urinarydoseMale * 160.22 * SOURCEAREA[dirindex];
	urinarydoseFemale = urinarydoseFemale * 160.22 * SOURCEAREA[dirindex];

	float urinarydose = (urinarydoseMale + urinarydoseFemale) / 2 / KERMAFREEINAIR[ergindex];

	// 食道
	float oesophagusdoseMale = 0; float oesophagusdoseFemale = 0;
	oesophagusdoseMale = (getSingleOrganErg(instancename1, InfoForAll, 110).first) / (getSingleOrganWet(instancename1, InfoForAll, 110));
	oesophagusdoseFemale = (getSingleOrganErg(instancename2, InfoForAll, 110).first) / (getSingleOrganWet(instancename2, InfoForAll, 110));
	oesophagusdoseMale = oesophagusdoseMale * 160.22 * SOURCEAREA[dirindex];
	oesophagusdoseFemale = oesophagusdoseFemale * 160.22 * SOURCEAREA[dirindex];

	float oesophagusdose = (oesophagusdoseMale + oesophagusdoseFemale) / 2 / KERMAFREEINAIR[ergindex];

	// 肝脏
	float liverdoseMale = 0; float liverdoseFemale = 0;
	liverdoseMale = (getSingleOrganErg(instancename1, InfoForAll, 95).first) / (getSingleOrganWet(instancename1, InfoForAll, 95));
	liverdoseFemale = (getSingleOrganErg(instancename2, InfoForAll, 95).first) / (getSingleOrganWet(instancename2, InfoForAll, 95));
	liverdoseMale = liverdoseMale * 160.22 * SOURCEAREA[dirindex];
	liverdoseFemale = liverdoseFemale * 160.22 * SOURCEAREA[dirindex];

	float liverdose = (liverdoseMale + liverdoseFemale) / 2 / KERMAFREEINAIR[ergindex];
	// 甲状腺
	float thyroiddoseMale = 0; float thyroiddoseFemale = 0;
	thyroiddoseMale = (getSingleOrganErg(instancename1, InfoForAll, 132).first) / (getSingleOrganWet(instancename1, InfoForAll, 132));
	thyroiddoseFemale = (getSingleOrganErg(instancename2, InfoForAll, 132).first) / (getSingleOrganWet(instancename2, InfoForAll, 132));
	thyroiddoseMale = thyroiddoseMale * 160.22 * SOURCEAREA[dirindex];
	thyroiddoseFemale = thyroiddoseFemale * 160.22 * SOURCEAREA[dirindex];

	float thyroiddose = (thyroiddoseMale + thyroiddoseFemale) / 2 / KERMAFREEINAIR[ergindex];
	// 骨表面
	// only cell 14 25 27 29 40 42 44 45 46  50  52  54  56, 对应tally *f1008
	std::vector<int> bonesurface = { 14, 25, 27, 29, 40, 42, 44, 45, 46,  50,  52,  54,  56 };
	float BSdoseMale = 0; float BSdoseFemale = 0;
	float ergsum1 = 0; float ergsum2 = 0;
	float wetsum1 = 0; float wetsum2 = 0;
	for (int i = 0; i < bonesurface.size(); i++)
	{
		ergsum1 += getSingleOrganErg(instancename1, InfoForAll, bonesurface[i]).first;
		wetsum1 += getSingleOrganWet(instancename1, InfoForAll, bonesurface[i]);
	}
	for (int i = 0; i < bonesurface.size(); i++)
	{
		ergsum2 += getSingleOrganErg(instancename2, InfoForAll, bonesurface[i]).first;
		wetsum2 += getSingleOrganWet(instancename2, InfoForAll, bonesurface[i]);
	}
	BSdoseMale = ergsum1 / wetsum1;
	BSdoseFemale = ergsum2 / wetsum2;
	BSdoseMale = BSdoseMale * 160.22 * SOURCEAREA[dirindex];
	BSdoseFemale = BSdoseFemale * 160.22 * SOURCEAREA[dirindex];
	float BSdose = (BSdoseMale + BSdoseFemale) / 2 / KERMAFREEINAIR[ergindex];

	// 脑
	float braindoseMale = 0; float braindoseFemale = 0;
	braindoseMale = (getSingleOrganErg(instancename1, InfoForAll, 61).first) / (getSingleOrganWet(instancename1, InfoForAll, 61));
	braindoseFemale = (getSingleOrganErg(instancename2, InfoForAll, 61).first) / (getSingleOrganWet(instancename2, InfoForAll, 61));
	braindoseMale = braindoseMale * 160.22 * SOURCEAREA[dirindex];
	braindoseFemale = braindoseFemale * 160.22 * SOURCEAREA[dirindex];

	float braindose = (braindoseMale + braindoseFemale) / 2 / KERMAFREEINAIR[ergindex];

	// 唾液腺
	float salivaryglandsMale = 0; float salivaryglandsFemale = 0;
	salivaryglandsMale = (getSingleOrganErg(instancename1, InfoForAll, 120).first + getSingleOrganErg(instancename1, InfoForAll, 121).first)
		/ (getSingleOrganWet(instancename1, InfoForAll, 120) + getSingleOrganWet(instancename1, InfoForAll, 121));
	salivaryglandsFemale = (getSingleOrganErg(instancename2, InfoForAll, 120).first + getSingleOrganErg(instancename2, InfoForAll, 121).first)
		/ (getSingleOrganWet(instancename2, InfoForAll, 120) + getSingleOrganWet(instancename2, InfoForAll, 121));
	salivaryglandsMale = salivaryglandsMale * 160.22 * SOURCEAREA[dirindex];
	salivaryglandsFemale = salivaryglandsFemale * 160.22 * SOURCEAREA[dirindex];

	float salivaryglandsdose = (salivaryglandsMale + salivaryglandsFemale) / 2 / KERMAFREEINAIR[ergindex];

	// 皮肤
	float skinMale = 0; float skinFemale = 0;
	skinMale = (getSingleOrganErg(instancename1, InfoForAll, 125).first) / (getSingleOrganWet(instancename1, InfoForAll, 125));
	skinFemale = (getSingleOrganErg(instancename2, InfoForAll, 125).first) / (getSingleOrganWet(instancename2, InfoForAll, 125));
	skinMale = skinMale * 160.22 * SOURCEAREA[dirindex];
	skinFemale = skinFemale * 160.22 * SOURCEAREA[dirindex];

	float skindose = (skinMale + skinFemale) / 2 / KERMAFREEINAIR[ergindex];

	float equlvantdose =  ((rbmeqdose * TISSUESWEIGHTINGFACTOR[0] + coloneqdose * TISSUESWEIGHTINGFACTOR[0] + 
		lungdose * TISSUESWEIGHTINGFACTOR[0] + stomachdose * TISSUESWEIGHTINGFACTOR[0] + 
		breastdose * TISSUESWEIGHTINGFACTOR[0] + residualdose * TISSUESWEIGHTINGFACTOR[0] + 
		gonaddose * TISSUESWEIGHTINGFACTOR[1] + urinarydose * TISSUESWEIGHTINGFACTOR[2] + 
		oesophagusdose * TISSUESWEIGHTINGFACTOR[2] + liverdose * TISSUESWEIGHTINGFACTOR[2] + 
		thyroiddose * TISSUESWEIGHTINGFACTOR[2] + BSdose * TISSUESWEIGHTINGFACTOR[3] + 
		braindose * TISSUESWEIGHTINGFACTOR[3] + salivaryglandsdose * TISSUESWEIGHTINGFACTOR[3] + skindose * TISSUESWEIGHTINGFACTOR[3])) / 
		KERMAFREEINAIR[ergindex];

	return equlvantdose;
}

// 返回器官能量沉积 MeV，即是*f1008的数值
std::pair<float, float> getSingleOrganErg(PhantomAgeDirErg instancename, AllInfo& InfoForAll, int organID)
{
	std::map<std::string, TallyInfoInAPhantom>::iterator phantomtally_p;
	if (InfoForAll.tally_count_all.end() == (phantomtally_p = InfoForAll.tally_count_all.find(instancename.name))) // 不能找到agetmp
	{
		return{ 0, 0 };
	}
	else
	{
		std::map<int, TallyInfo>::iterator onetall_p;
		if (phantomtally_p->second.tallyinaphantom_count.end() == (onetall_p = phantomtally_p->second.tallyinaphantom_count.find(1008)))
		{
			return{ 0, 0 };
		}
		else
		{
			std::map<int, std::pair<float, float>>::iterator celltally_p;
			if (onetall_p->second.valerr_count.end() == (celltally_p = onetall_p->second.valerr_count.find(organID)))
			{
				return{ 0, 0 };
			}
			else
			{
				return celltally_p->second;
			}
		}
	}
}

// 返回器官质量
float getSingleOrganWet(PhantomAgeDirErg instancename, AllInfo& InfoForAll, int organID)
{
	std::string agetmp;
	agetmp = instancename.ageGet();

	std::map<std::string, PhantomInfo>::iterator phantominfo_p;
	if (InfoForAll.phantominfo_count_all.end() == (phantominfo_p = InfoForAll.phantominfo_count_all.find(agetmp)))
	{
		return 0;
	}
	else
	{
		std::map<int, float>::iterator wetinaphantom_p;
		if (phantominfo_p->second.organwet_count.end() == (wetinaphantom_p = phantominfo_p->second.organwet_count.find(organID)))
		{
			return 0;
		}
		else
		{
			return wetinaphantom_p->second;
		}
	}
}