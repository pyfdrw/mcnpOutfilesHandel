#include "tallyclass.h"

int dataControlFun(std::string dirpath, AllInfo InfoForAll)
{/*
	std::string systemcommand;
	systemcommand = "mkdir " + dirpath + "datasum";
	system(systemcommand.c_str());*/
	std::string workpath; workpath = ("E:\\datasum\\");
	std::stringstream sstmp; sstmp.clear();

	int organID; // for single organ
	int organID1; int organID2;// for organs in pair
	std::string compare1; std::string compare2;
	std::string filename;
	float *rpidosedata; float *ustcdosedata;
	float *ratiodata;

	// AM / rpiAM 137 bladder
	organID = 137;
	compare1 = "am"; compare2 = "rpiam";

	ratiodata =new float[dirall.size() * ergall.size()]; // 能量作为列，方向作为行
	ustcdosedata = calDCCKWithOutShow(organID, compare1, InfoForAll);
	rpidosedata = calDCCKWithOutShow(organID, compare2, InfoForAll);
	for (int i = 0; i < dirall.size() * ergall.size(); i++)
	{
		ratiodata[i] = ustcdosedata[i] / rpidosedata[i];
	}
	// 输出 文件命名 比较 + 被比较 + 器官ID
	filename.clear();
	sstmp.clear(); sstmp << organID;
	sstmp >> filename;
	filename = workpath + compare1 + compare2 + filename + ".txt";
	organCompareOut(ratiodata, filename);


	// AM / rpiAM 95 liver
	organID = 95;
	compare1 = "am"; compare2 = "rpiam";

	ratiodata = new float[dirall.size() * ergall.size()]; // 能量作为列，方向作为行
	ustcdosedata = calDCCKWithOutShow(organID, compare1, InfoForAll);
	rpidosedata = calDCCKWithOutShow(organID, compare2, InfoForAll);
	for (int i = 0; i < dirall.size() * ergall.size(); i++)
	{
		ratiodata[i] = ustcdosedata[i] / rpidosedata[i];
	}
	// 输出 文件命名 比较 + 被比较 + 器官ID
	filename.clear();
	sstmp.clear(); sstmp << organID;
	sstmp >> filename;
	filename = workpath + compare1 + compare2 + filename + ".txt";
	organCompareOut(ratiodata, filename);


	// AM / rpiAM 129 130
	organID1 = 129; organID2 = 130;
	compare1 = "am"; compare2 = "rpiam";

	ustcdosedata = calDCCKWithOutShow(organID1, organID2, compare1, InfoForAll);
	rpidosedata = calDCCKWithOutShow(organID1, organID2, compare2, InfoForAll);
	for (int i = 0; i < dirall.size() * ergall.size(); i++)
	{
		ratiodata[i] = ustcdosedata[i] / rpidosedata[i];
	}
	// 输出 文件命名 比较 + 被比较 + 器官ID1 + "and" + 器官ID2
	filename.clear();
	sstmp.clear(); sstmp << organID1; sstmp << "and"; sstmp << organID2;
	sstmp >> filename;
	filename = workpath + compare1 + compare2 + filename + ".txt";
	organCompareOut(ratiodata, filename);

	// AM / rpiAM 63 65
	organID1 = 63; organID2 = 65;
	compare1 = "am"; compare2 = "rpiam";

	ustcdosedata = calDCCKWithOutShow(organID1, organID2, compare1, InfoForAll);
	rpidosedata = calDCCKWithOutShow(organID1, organID2, compare2, InfoForAll);
	for (int i = 0; i < dirall.size() * ergall.size(); i++)
	{
		ratiodata[i] = ustcdosedata[i] / rpidosedata[i];
	}
	// 输出 文件命名 比较 + 被比较 + 器官ID1 + "and" + 器官ID2
	filename.clear();
	sstmp.clear(); sstmp << organID1; sstmp << "and"; sstmp << organID2;
	sstmp >> filename;
	filename = workpath + compare1 + compare2 + filename + ".txt";
	organCompareOut(ratiodata, filename);

	// AM / rpiAM 97 99
	organID1 = 97; organID2 = 99;
	compare1 = "am"; compare2 = "rpiam";

	ustcdosedata = calDCCKWithOutShow(organID1, organID2, compare1, InfoForAll);
	rpidosedata = calDCCKWithOutShow(organID1, organID2, compare2, InfoForAll);
	for (int i = 0; i < dirall.size() * ergall.size(); i++)
	{
		ratiodata[i] = ustcdosedata[i] / rpidosedata[i];
	}
	// 输出 文件命名 比较 + 被比较 + 器官ID1 + "and" + 器官ID2
	filename.clear();
	sstmp.clear(); sstmp << organID1; sstmp << "and"; sstmp << organID2;
	sstmp >> filename;
	filename = workpath + compare1 + compare2 + filename + ".txt";
	organCompareOut(ratiodata, filename);

	// AF / rpiAF 111 112
	organID1 = 111; organID2 = 112;
	compare1 = "af"; compare2 = "rpiaf";

	ustcdosedata = calDCCKWithOutShow(organID1, organID2, compare1, InfoForAll);
	rpidosedata = calDCCKWithOutShow(organID1, organID2, compare2, InfoForAll);
	for (int i = 0; i < dirall.size() * ergall.size(); i++)
	{
		ratiodata[i] = ustcdosedata[i] / rpidosedata[i];
	}
	// 输出 文件命名 比较 + 被比较 + 器官ID1 + "and" + 器官ID2
	filename.clear();
	sstmp.clear(); sstmp << organID1; sstmp << "and"; sstmp << organID2;
	sstmp >> filename;
	filename = workpath + compare1 + compare2 + filename + ".txt";
	organCompareOut(ratiodata, filename);

	// AF / rpiAF 111 112
	organID1 = 63; organID2 = 65;
	compare1 = "af"; compare2 = "rpiaf";

	ustcdosedata = calDCCKWithOutShow(organID1, organID2, compare1, InfoForAll);
	rpidosedata = calDCCKWithOutShow(organID1, organID2, compare2, InfoForAll);
	for (int i = 0; i < dirall.size() * ergall.size(); i++)
	{
		ratiodata[i] = ustcdosedata[i] / rpidosedata[i];
	}
	// 输出 文件命名 比较 + 被比较 + 器官ID1 + "and" + 器官ID2
	filename.clear();
	sstmp.clear(); sstmp << organID1; sstmp << "and"; sstmp << organID2;
	sstmp >> filename;
	filename = workpath + compare1 + compare2 + filename + ".txt";
	organCompareOut(ratiodata, filename);


	return 0;
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
}