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

// ��������˳������ν,����������貿��
int calDCCK(int organleft, int organright, std::string agetmp, AllInfo InfoForAll)
{
	// ������ٵ�������Ϣ
	float organleftmass = 0;
	float organrightmass = 0;

	if (InfoForAll.phantominfo_count_all.end() != InfoForAll.phantominfo_count_all.find(agetmp))
	{
		std::map <std::string, PhantomInfo>::iterator onephantom_p;
		onephantom_p = InfoForAll.phantominfo_count_all.find(agetmp);
		std::map<int, float>::iterator infotmp_p;
		if ((onephantom_p->second).organwet_count.end() != (infotmp_p = (onephantom_p->second.organwet_count.find(organleft)))) // �������������
		{
			organleftmass = infotmp_p->second;
		}
		else
		{
			return -1;
		}
		if ((onephantom_p->second).organwet_count.end() != (infotmp_p = (onephantom_p->second.organwet_count.find(organright)))) // �������������
		{
			organrightmass = infotmp_p->second;
		}
		else
		{
			return -1;
		}
	}
	else // ��������������
	{
		return -1;
	}

	// ��������ת��ϵ��������
	float* conversioncoefficients = new float[dirall.size() * ergall.size()]; // ������Ϊ�У�������Ϊ��

	// DCC? = Dt / ? = 1.602 * 1e2 * ES / m 
	// ?ͨ���� E MeV, S cm2, m g, DCC? pGy*cm^2
	// DCCk = Dt/Ka = DCC? / (Ka/?)
	// (Ka/?) ��Ӧ�� KERMAFREEINAIR����ֵ

	float dcctongliang = 0;
	for (int i = 0; i < ergall.size(); i++)  // ��������
	{
		for (int j = 0; j < dirall.size(); j++)  // �������
		{
			std::string singleinstence; singleinstence.clear();
			float tallyforleft = 0; float tallyforright = 0;
			singleinstence = agetmp + "_" + dirall[j] + "_" + ergall[i];
			std::map<std::string, TallyInfoInAPhantom>::iterator tallinaphantom_p;
			if (InfoForAll.tally_count_all.end() != (tallinaphantom_p = InfoForAll.tally_count_all.find(singleinstence))) // �ҵ�����ļ�
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
			// ��������������ٵ�f8�Ķ�����ȷ��
			dcctongliang = (tallyforleft + tallyforright) / (organleftmass + organrightmass) * 1.602 * 100 * SOURCEAREA[j];
			conversioncoefficients[j + i * dirall.size()] = dcctongliang;  // DCC?
		}
	}

	// DCCk����ȡ
	for (int i = 0; i < ergall.size(); i++)  // ��������
	{
		for (int j = 0; j < dirall.size(); j++)  // �������
		{
			conversioncoefficients[j + i * dirall.size()] = conversioncoefficients[j + i * dirall.size()] / KERMAFREEINAIR[i];
		}
	}

	// �������
	std::cout << "------------------" << std::endl;
	std::cout << "Organ     " << organleft << "  +  " << organright << std::endl;
	std::cout << "Erenge(MeV)       AP       PA     LLAT     RLAT      ROT      ISO" << std::endl;
	for (int i = 0; i < ergall.size(); i++)  // ��������
	{
		std::cout << std::setw(11) << std::setprecision(3) << std::left << ergall_val[i];
		for (int j = 0; j < dirall.size(); j++)  // �������
		{
			std::cout << std::setw(9) << std::setprecision(3) << std::right << conversioncoefficients[j + i * dirall.size()];
		}
		std::cout << std::endl;
	}


	return 0;
}

// ��������˳������ν,����������貿��
int calDCCK(int organ, std::string agetmp, AllInfo InfoForAll)
{
	// ������ٵ�������Ϣ
	float organmass = 0;

	if (InfoForAll.phantominfo_count_all.end() != InfoForAll.phantominfo_count_all.find(agetmp))
	{
		std::map <std::string, PhantomInfo>::iterator onephantom_p;
		onephantom_p = InfoForAll.phantominfo_count_all.find(agetmp);
		std::map<int, float>::iterator infotmp_p;
		if ((onephantom_p->second).organwet_count.end() != (infotmp_p = (onephantom_p->second.organwet_count.find(organ)))) // �������������
		{
			organmass = infotmp_p->second;
		}
		else
		{
			return -1;
		}
	}
	else // ��������������
	{
		return -1;
	}

	// ��������ת��ϵ��������
	float* conversioncoefficients = new float[dirall.size() * ergall.size()]; // ������Ϊ�У�������Ϊ��

	// DCC? = Dt / ? = 1.602 * 1e2 * ES / m 
	// ?ͨ���� E MeV, S cm2, m g, DCC? pGy*cm^2
	// DCCk = Dt/Ka = DCC? / (Ka/?)
	// (Ka/?) ��Ӧ�� KERMAFREEINAIR����ֵ

	float dcctongliang = 0;
	for (int i = 0; i < ergall.size(); i++)  // ��������
	{
		for (int j = 0; j < dirall.size(); j++)  // �������
		{
			std::string singleinstence; singleinstence.clear();
			float tallyfororgan = 0;
			singleinstence = agetmp + "_" + dirall[j] + "_" + ergall[i];
			std::map<std::string, TallyInfoInAPhantom>::iterator tallinaphantom_p;
			if (InfoForAll.tally_count_all.end() != (tallinaphantom_p = InfoForAll.tally_count_all.find(singleinstence))) // �ҵ�����ļ�
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
			// ��������������ٵ�f8�Ķ�����ȷ��
			dcctongliang = tallyfororgan / organmass * 1.602 * 100 * SOURCEAREA[j];
			conversioncoefficients[j + i * dirall.size()] = dcctongliang;  // DCC?
		}
	}

	// DCCk����ȡ
	for (int i = 0; i < ergall.size(); i++)  // ��������
	{
		for (int j = 0; j < dirall.size(); j++)  // �������
		{
			conversioncoefficients[j + i * dirall.size()] = conversioncoefficients[j + i * dirall.size()] / KERMAFREEINAIR[i];
		}
	}

	// �������
	std::cout << "------------------" << std::endl;
	std::cout << "Organ    " << organ << std::endl;
	std::cout << "Erenge(MeV)       AP       PA     LLAT     RLAT      ROT      ISO" << std::endl;
	for (int i = 0; i < ergall.size(); i++)  // ��������
	{
		std::cout << std::setw(11) << std::setprecision(3) << std::left << ergall_val[i];
		for (int j = 0; j < dirall.size(); j++)  // �������
		{
			std::cout << std::setw(9) << std::setprecision(3) << std::right << conversioncoefficients[j + i * dirall.size()];
		}
		std::cout << std::endl;
	}


	return 0;
}


int calDCCK(int organleft, int organright, std::string agetmp, AllInfo InfoForAll, std::string outputfilepath)
{
	// ������ٵ�������Ϣ
	float organleftmass = 0;
	float organrightmass = 0;

	if (InfoForAll.phantominfo_count_all.end() != InfoForAll.phantominfo_count_all.find(agetmp))
	{
		std::map <std::string, PhantomInfo>::iterator onephantom_p;
		onephantom_p = InfoForAll.phantominfo_count_all.find(agetmp);
		std::map<int, float>::iterator infotmp_p;
		if ((onephantom_p->second).organwet_count.end() != (infotmp_p = (onephantom_p->second.organwet_count.find(organleft)))) // �������������
		{
			organleftmass = infotmp_p->second;
		}
		else
		{
			return -1;
		}
		if ((onephantom_p->second).organwet_count.end() != (infotmp_p = (onephantom_p->second.organwet_count.find(organright)))) // �������������
		{
			organrightmass = infotmp_p->second;
		}
		else
		{
			return -1;
		}
	}
	else // ��������������
	{
		return -1;
	}

	// ��������ת��ϵ��������
	float* conversioncoefficients = new float[dirall.size() * ergall.size()]; // ������Ϊ�У�������Ϊ��

																			  // DCC? = Dt / ? = 1.602 * 1e2 * ES / m 
																			  // ?ͨ���� E MeV, S cm2, m g, DCC? pGy*cm^2
																			  // DCCk = Dt/Ka = DCC? / (Ka/?)
																			  // (Ka/?) ��Ӧ�� KERMAFREEINAIR����ֵ

	float dcctongliang = 0;
	for (int i = 0; i < ergall.size(); i++)  // ��������
	{
		for (int j = 0; j < dirall.size(); j++)  // �������
		{
			std::string singleinstence; singleinstence.clear();
			float tallyforleft = 0; float tallyforright = 0;
			singleinstence = agetmp + "_" + dirall[j] + "_" + ergall[i];
			std::map<std::string, TallyInfoInAPhantom>::iterator tallinaphantom_p;
			if (InfoForAll.tally_count_all.end() != (tallinaphantom_p = InfoForAll.tally_count_all.find(singleinstence))) // �ҵ�����ļ�
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
			// ��������������ٵ�f8�Ķ�����ȷ��
			dcctongliang = (tallyforleft + tallyforright) / (organleftmass + organrightmass) * 1.602 * 100 * SOURCEAREA[j];
			conversioncoefficients[j + i * dirall.size()] = dcctongliang;  // DCC?
		}
	}

	// DCCk����ȡ
	for (int i = 0; i < ergall.size(); i++)  // ��������
	{
		for (int j = 0; j < dirall.size(); j++)  // �������
		{
			conversioncoefficients[j + i * dirall.size()] = conversioncoefficients[j + i * dirall.size()] / KERMAFREEINAIR[i];
		}
	}

	// �������
	std::cout << "------------------" << std::endl;
	std::cout << "Organ     " << organleft << "  +  " << organright << std::endl;
	std::cout << "Erenge(MeV)       AP       PA     LLAT     RLAT      ROT      ISO" << std::endl;
	for (int i = 0; i < ergall.size(); i++)  // ��������
	{
		std::cout << std::setw(11) << std::setprecision(3) << std::left << ergall_val[i];
		for (int j = 0; j < dirall.size(); j++)  // �������
		{
			std::cout << std::setw(9) << std::setprecision(3) << std::right << conversioncoefficients[j + i * dirall.size()];
		}
		std::cout << std::endl;
	}

	// ������ļ�
	std::fstream fileout;
	fileout.open(outputfilepath, std::ios::out);
	if (fileout.fail()) // ���ܴ��ļ�
	{
		return -2;
	}

	fileout << "------------------" << '\n';
	fileout << "Organ    " << organleft << " + " << organright << '\n';
	fileout << "Erenge(MeV)       AP       PA     LLAT     RLAT      ROT      ISO" << '\n';
	for (int i = 0; i < ergall.size(); i++)  // ��������
	{
		fileout << std::setw(11) << std::setprecision(3) << std::left << ergall_val[i];
		for (int j = 0; j < dirall.size(); j++)  // �������
		{
			fileout << std::setw(9) << std::setprecision(3) << std::right << conversioncoefficients[j + i * dirall.size()];
		}
		fileout << std::endl;
	}

	return 0;
}
// �����ļ������·��outputfilepath
int calDCCK(int organ, std::string agetmp, AllInfo InfoForAll, std::string outputfilepath)
{
	// ������ٵ�������Ϣ
	float organmass = 0;

	if (InfoForAll.phantominfo_count_all.end() != InfoForAll.phantominfo_count_all.find(agetmp))
	{
		std::map <std::string, PhantomInfo>::iterator onephantom_p;
		onephantom_p = InfoForAll.phantominfo_count_all.find(agetmp);
		std::map<int, float>::iterator infotmp_p;
		if ((onephantom_p->second).organwet_count.end() != (infotmp_p = (onephantom_p->second.organwet_count.find(organ)))) // �������������
		{
			organmass = infotmp_p->second;
		}
		else
		{
			return -1;
		}
	}
	else // ��������������
	{
		return -1;
	}

	// ��������ת��ϵ��������
	float* conversioncoefficients = new float[dirall.size() * ergall.size()]; // ������Ϊ�У�������Ϊ��

																			  // DCC? = Dt / ? = 1.602 * 1e2 * ES / m 
																			  // ?ͨ���� E MeV, S cm2, m g, DCC? pGy*cm^2
																			  // DCCk = Dt/Ka = DCC? / (Ka/?)
																			  // (Ka/?) ��Ӧ�� KERMAFREEINAIR����ֵ

	float dcctongliang = 0;
	for (int i = 0; i < ergall.size(); i++)  // ��������
	{
		for (int j = 0; j < dirall.size(); j++)  // �������
		{
			std::string singleinstence; singleinstence.clear();
			float tallyfororgan = 0;
			singleinstence = agetmp + "_" + dirall[j] + "_" + ergall[i];
			std::map<std::string, TallyInfoInAPhantom>::iterator tallinaphantom_p;
			if (InfoForAll.tally_count_all.end() != (tallinaphantom_p = InfoForAll.tally_count_all.find(singleinstence))) // �ҵ�����ļ�
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
			// ��������������ٵ�f8�Ķ�����ȷ��
			dcctongliang = tallyfororgan / organmass * 1.602 * 100 * SOURCEAREA[j];
			conversioncoefficients[j + i * dirall.size()] = dcctongliang;  // DCC?
		}
	}

	// DCCk����ȡ
	for (int i = 0; i < ergall.size(); i++)  // ��������
	{
		for (int j = 0; j < dirall.size(); j++)  // �������
		{
			conversioncoefficients[j + i * dirall.size()] = conversioncoefficients[j + i * dirall.size()] / KERMAFREEINAIR[i];
		}
	}

	// �������
	std::cout << "------------------" << std::endl;
	std::cout << "Organ    " << organ << std::endl;
	std::cout << "Erenge(MeV)       AP       PA     LLAT     RLAT      ROT      ISO" << std::endl;
	for (int i = 0; i < ergall.size(); i++)  // ��������
	{
		std::cout << std::setw(11) << std::setprecision(3) << std::left << ergall_val[i];
		for (int j = 0; j < dirall.size(); j++)  // �������
		{
			std::cout << std::setw(9) << std::setprecision(3) << std::right << conversioncoefficients[j + i * dirall.size()];
		}
		std::cout << std::endl;
	}

	// ������ļ�
	std::fstream fileout;
	fileout.open(outputfilepath, std::ios::out);
	if (fileout.fail()) // ���ܴ��ļ�
	{
		return -2;
	}

	fileout << "------------------" << '\n';
	fileout << "Organ    " << organ << '\n';
	fileout<< "Erenge(MeV)       AP       PA     LLAT     RLAT      ROT      ISO" << '\n';
	for (int i = 0; i < ergall.size(); i++)  // ��������
	{
		fileout << std::setw(11) << std::setprecision(3) << std::left << ergall_val[i];
		for (int j = 0; j < dirall.size(); j++)  // �������
		{
			fileout << std::setw(9) << std::setprecision(3) << std::right << conversioncoefficients[j + i * dirall.size()];
		}
		fileout << std::endl;
	}


	return 0;
}

int calDCCKRBM(PhantomAgeDirErg instancename, AllInfo InfoForAll)
{
	// only cell 14 25 27 29 40 42 44 45 46  50  52  54  56
	// ��Ӧtally 14 24 34 44 54 64 74 84 94 101 114 124 134
	const int tallyNUM = 13;
	int tallysum[tallyNUM] = { 14, 24, 34, 44, 54, 64, 74, 84, 94, 104, 114, 124, 134 };
	float tallyvalue[tallyNUM] = {0};
	float sumpercentRBM = 0;
	if (!instancename.ageGet().compare("am") || !instancename.ageGet().compare("af") || !instancename.ageGet().compare("rpiaf") || !instancename.ageGet().compare("rpiam"))
	{
		for (int i = 0; i < FractionRBMForAdult.size(); i++)
		{
			sumpercentRBM += FractionRBMForAdult[i];
		}
	}
	else if(!instancename.ageGet().compare("15m") || !instancename.ageGet().compare("15f"))
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

	// �����������f4�ļ�¼�������tallyvalue��
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