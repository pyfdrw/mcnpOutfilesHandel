// 处理mcnp的输出文件(以o为最后的字母)，记录tally卡的数量，类型，数值，统计误差等信息

#include "tallyclass.h"

int main()
{
	std::string dirpath;

	// dirpath = filedirGet();
	// std::cout << dirpath << std::endl;
	dirpath = "E:\\体模变形\\变形New\\MCNP\\Allinput\\ANS\\";
	//std::string filepath = "E:\\体模变形\\变形New\\MCNP\\simulate_answer\\test\\am_ap_0o";
	//std::string filepath = "E:\\体模变形\\变形New\\MCNP\\simulate_answer\\test\\fortest.txt";
	//singlefileHandel(filepath);
	AllInfo InfoForAll;
	allinfoStore(InfoForAll, dirpath);

	//showSinglePhantomInfo("am", InfoForAll);
	PhantomAgeDirErg instancename("rpiam_ap_0");

	showSinglePhantomInfo(instancename, InfoForAll);

	//calDCCK(129, 130, "am", InfoForAll);
	//calDCCK(111, 112, "af", InfoForAll);
	//calDCCK(97, 99, "am", InfoForAll);
	// calDCCK(129, "am", InfoForAll);

	//calDCCK(129, 130, "am", InfoForAll, "E:\\体模变形\\变形New\\MCNP\\simulate_answer\\AM\\am_ans_129.txt");
	//calDCCK(129, "am", InfoForAll, "E:\\体模变形\\变形New\\MCNP\\simulate_answer\\AM\\am_ans_129.txt");
	//calDCCK(14, "am", InfoForAll);

	//calDCCKRBMWithShow(instancename, InfoForAll);

	for (int i = 0; i < ergall.size();  i++)
	{
		instancename.name = "rpiam_ap_" + ergall[i];
		std::cout << "\nEffective dose for rpiAM&rpiAF with ergindex " << ergall_val[i] << " =  " << calEffectiveDose(instancename, InfoForAll) << std::endl;
	}
	
	// dataControlFun(dirpath, InfoForAll);

	return 0;
}