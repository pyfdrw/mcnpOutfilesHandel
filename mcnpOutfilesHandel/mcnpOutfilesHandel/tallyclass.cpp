#include "tallyclass.h"

PhantomInfo::PhantomInfo()
{
	organvol_count.clear();
	organwet_count.clear();
}


TallyInfo::TallyInfo()
{
    isStar = false;
	valerr_count.clear();
}

TallyInfoInAPhantom::TallyInfoInAPhantom()
{
	tallyinaphantom_count.clear();
}

AllInfo::AllInfo()
{
	tally_count_all.clear();
	phantominfo_count_all.clear();
}

PhantomAgeDirErg::PhantomAgeDirErg()
{
	name.clear();
}

PhantomAgeDirErg::PhantomAgeDirErg(std::string agedirerg)
{
	name = agedirerg;
}

std::string PhantomAgeDirErg::ageGet()
{
	std::string agetmp;
	int indextmp1,indextmp2;
	indextmp1 = name.find("_");
	indextmp2 = name.find("_", indextmp1 + 1);
	if (-1 == indextmp1 || -1 == indextmp2)
	{
		return " ";
	}
	else
	{
		agetmp.clear();
		agetmp.append(name, 0, indextmp1);
		return agetmp;
	}
}
std::string PhantomAgeDirErg::dirGet()
{
	std::string agetmp;
	int indextmp1, indextmp2;
	indextmp1 = name.find("_");
	indextmp2 = name.find("_", indextmp1 + 1);
	if (-1 == indextmp1 || -1 == indextmp2)
	{
		return " ";
	}
	else
	{
		agetmp.clear();
		agetmp.append(name, indextmp1 + 1, indextmp2 - indextmp1 - 1);
		return agetmp;
	}
}
std::string PhantomAgeDirErg::ergGet()
{
	std::string agetmp;
	int indextmp1, indextmp2;
	indextmp1 = name.find("_");
	indextmp2 = name.find("_", indextmp1 + 1);
	if (-1 == indextmp1 || -1 == indextmp2)
	{
		return " ";
	}
	else
	{
		agetmp.clear();
		agetmp.append(name, indextmp2 + 1, name.length() - indextmp2 - 1);
		return agetmp;
	}
}