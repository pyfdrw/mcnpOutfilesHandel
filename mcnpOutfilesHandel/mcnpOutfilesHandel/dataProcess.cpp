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