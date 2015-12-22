#include "tallyclass.h"

int dataControlFun(std::string dirpath, AllInfo& InfoForAll)
{/*
	std::string systemcommand;
	systemcommand = "mkdir " + dirpath + "datasum";
	system(systemcommand.c_str());*/
	std::string workpath; workpath = ("E:\\datasum\\");

	int organID; // for single organ
	int organID1; int organID2;// for organs in pair
	std::string compare1; std::string compare2;
	float *ratiodata = 0;

	// 比较数据和输出单个体模的信息
	{
		// AM / rpiAM 137 bladder
		organID = 137;
		compare1 = "am"; compare2 = "rpiam";

		ratiodata = organCompare(organID, compare1, compare2, InfoForAll);
		// 输出 文件命名 比较 + 被比较 + 器官ID
		outControl(ratiodata, organID, compare1, compare2, workpath, InfoForAll);

		// AM / rpiAM 95 liver
		organID = 95;
		compare1 = "am"; compare2 = "rpiam";

		ratiodata = organCompare(organID, compare1, compare2, InfoForAll);
		// 输出 文件命名 比较 + 被比较 + 器官ID
		outControl(ratiodata, organID, compare1, compare2, workpath, InfoForAll);

		// AM / rpiAM 75 small intense
		organID = 75;
		compare1 = "am"; compare2 = "rpiam";

		ratiodata = organCompare(organID, compare1, compare2, InfoForAll);
		// 输出 文件命名 比较 + 被比较 + 器官ID
		outControl(ratiodata, organID, compare1, compare2, workpath, InfoForAll);

		// AM / rpiAM 129 130
		organID1 = 129; organID2 = 130;
		compare1 = "am"; compare2 = "rpiam";

		ratiodata = organCompare(organID1, organID2, compare1, compare2, InfoForAll);
		// 输出 文件命名 比较 + 被比较 + 器官ID
		outControl(ratiodata, organID1, organID2, compare1, compare2, workpath, InfoForAll);

		// AM / rpiAM 63 65
		organID1 = 63; organID2 = 65;
		compare1 = "am"; compare2 = "rpiam";

		ratiodata = organCompare(organID1, organID2, compare1, compare2, InfoForAll);
		// 输出 文件命名 比较 + 被比较 + 器官ID
		outControl(ratiodata, organID1, organID2, compare1, compare2, workpath, InfoForAll);

		// AM / rpiAM 97 99
		organID1 = 97; organID2 = 99;
		compare1 = "am"; compare2 = "rpiam";

		ratiodata = organCompare(organID1, organID2, compare1, compare2, InfoForAll);
		// 输出 文件命名 比较 + 被比较 + 器官ID
		outControl(ratiodata, organID1, organID2, compare1, compare2, workpath, InfoForAll);

		// AF / rpiAF 111 112
		organID1 = 111; organID2 = 112;
		compare1 = "af"; compare2 = "rpiaf";

		ratiodata = organCompare(organID1, organID2, compare1, compare2, InfoForAll);
		// 输出 文件命名 比较 + 被比较 + 器官ID
		outControl(ratiodata, organID1, organID2, compare1, compare2, workpath, InfoForAll);

		// AF / rpiAF 111 112
		organID1 = 63; organID2 = 65;
		compare1 = "af"; compare2 = "rpiaf";

		ratiodata = organCompare(organID1, organID2, compare1, compare2, InfoForAll);
		// 输出 文件命名 比较 + 被比较 + 器官ID
		outControl(ratiodata, organID1, organID2, compare1, compare2, workpath, InfoForAll);

		// Af / rpiAf 75 small intense
		organID = 75;
		compare1 = "af"; compare2 = "rpiaf";

		ratiodata = organCompare(organID, compare1, compare2, InfoForAll);
		// 输出 文件命名 比较 + 被比较 + 器官ID
		outControl(ratiodata, organID, compare1, compare2, workpath, InfoForAll);

		// AF / rpiAF 97 99
		organID1 = 97; organID2 = 99;
		compare1 = "af"; compare2 = "rpiaf";

		ratiodata = organCompare(organID1, organID2, compare1, compare2, InfoForAll);
		// 输出 文件命名 比较 + 被比较 + 器官ID
		outControl(ratiodata, organID1, organID2, compare1, compare2, workpath, InfoForAll);
	}
	
	// 有效剂量的对比，有效剂量为性别的平均值



	// 输出有效剂量到文件中，文件名 effectiveDose+age，例effectiveDoserpiadult effectiveDoseadult ...
	ratiodata = new float[ergall.size() * dirall.size()]; // 暂存有效剂量的数值
	PhantomAgeDirErg instancetmp; instancetmp.name.clear();
	std::vector<std::string> agetmp = { "rpiam", "am", "15m", "10m", "5m" }; // 有效剂量是性别的平均值，计算时已经考虑这些，指定"am"和指定"af"是一个效果
	std::vector < std::string> filenametmp = { "effectiveDoserpiadult.txt", "effectiveDoseadult.txt" , "effectiveDose15.txt" , "effectiveDose10.txt", "effectiveDose5.txt" };
	for (size_t i = 0; i < agetmp.size(); i++) // 体模种类遍历
	{
		for (int j = 0; j < ergall.size(); j++) // 能量做纵坐标
		{
			for (int k = 0; k < dirall.size(); k++) // 方向做横坐标
			{
				instancetmp.name = agetmp[i] + "_" + dirall[k] + "_" + ergall[j];
				ratiodata[k + j * dirall.size()] = calEffectiveDose(instancetmp, InfoForAll);
			}
		}
		organCompareOut(ratiodata, filenametmp[i]);
	}
	delete[]ratiodata;


	return 0;
}

float* organCompare(int organID1, int organID2, std::string compare1, std::string compare2, AllInfo& InfoForAll)
{
	float* ustcdosedata; float* rpidosedata;
	float* ratiodata = 0;
	ratiodata = new float[dirall.size() * ergall.size()];

	ustcdosedata = calDCCKWithOutShow(organID1, organID2, compare1, InfoForAll);
	rpidosedata = calDCCKWithOutShow(organID1, organID2, compare2, InfoForAll);
	for (int i = 0; i < dirall.size() * ergall.size(); i++)
	{
		ratiodata[i] = ustcdosedata[i] / rpidosedata[i];
	}
	
	return ratiodata;
}

float* organCompare(int organID, std::string compare1, std::string compare2, AllInfo& InfoForAll)
{
	float* ustcdosedata; float* rpidosedata;
	float* ratiodata = 0;
	ratiodata = new float[dirall.size() * ergall.size()];

	ratiodata = new float[dirall.size() * ergall.size()]; // 能量作为列，方向作为行
	ustcdosedata = calDCCKWithOutShow(organID, compare1, InfoForAll);
	rpidosedata = calDCCKWithOutShow(organID, compare2, InfoForAll);
	for (int i = 0; i < dirall.size() * ergall.size(); i++)
	{
		ratiodata[i] = ustcdosedata[i] / rpidosedata[i];
	}

	return ratiodata;
}

int outControl(float* ratiodata, int organID, std::string compare1, std::string compare2, std::string workpath, AllInfo& InfoForAll)
{
	std::string filename;
	filename.clear();
	std::stringstream sstmp;
	sstmp.clear(); sstmp << organID;
	sstmp >> filename;
	filename = workpath + compare1 + compare2 + filename + ".txt";
	organCompareOut(ratiodata, filename);

	filename.clear();
	sstmp.clear(); sstmp << organID;
	sstmp >> filename;
	filename = workpath + "real" + compare1 + filename + ".txt";
	calDCCK(organID, compare1, InfoForAll, filename);

	return 0;
}

int outControl(float* ratiodata, int organID1, int organID2, std::string compare1, std::string compare2, std::string workpath, AllInfo& InfoForAll)
{
	std::string filename;
	filename.clear();
	std::stringstream sstmp;
	sstmp.clear(); sstmp << organID1; sstmp << "and"; sstmp << organID2;
	sstmp >> filename;
	filename = workpath + compare1 + compare2 + filename + ".txt";
	organCompareOut(ratiodata, filename);

	filename.clear();
	sstmp.clear(); sstmp << organID1; sstmp << "and"; sstmp << organID2;
	sstmp >> filename;
	filename = workpath + "real" + compare1 + filename + ".txt";
	calDCCK(organID1, organID2, compare1, InfoForAll, filename);

	return 0;
}

int organDoseCompareAmongAges()
{

}

int organCompareOut(float *ratiodata, std::string filename)
{
	std::ofstream fileout;
	fileout.open(filename);
	if (fileout.fail())
	{
		return 0;
	}
	else
	{
		 for (int i = 0; i < ergall.size(); i++)  // 能量遍历
		 {
		 	for (int j = 0; j < dirall.size(); j++)  // 方向遍历
		 	{
				fileout << std::setw(9) << std::setprecision(3) << std::right << ratiodata[j + i * dirall.size()];
		 	}
			fileout << std::endl;
		 }
	}

	fileout.close();
	return 0;
}
