// ����mcnp������ļ�(��oΪ������ĸ)����¼tally�������������ͣ���ֵ��ͳ��������Ϣ

#include "tallyclass.h"

int main()
{
	std::string dirpath;

	// dirpath = filedirGet();
	// std::cout << dirpath << std::endl;
	dirpath = "E:\\��ģ����\\����New\\MCNP\\Allinput\\test\\";
	//std::string filepath = "E:\\��ģ����\\����New\\MCNP\\simulate_answer\\test\\am_ap_0o";
	//std::string filepath = "E:\\��ģ����\\����New\\MCNP\\simulate_answer\\test\\fortest.txt";
	//singlefileHandel(filepath);
	AllInfo InfoForAll;
	allinfoStore(InfoForAll, dirpath);

	showSinglePhantomInfo("am", InfoForAll);
	PhantomAgeDirErg instancename("am_ap_6");

	showSinglePhantomInfo(instancename, InfoForAll);

	calDCCK(129, 130, "am", InfoForAll);
	calDCCK(111, 112, "af", InfoForAll);

	// calDCCK(129, "am", InfoForAll);

	//calDCCK(129, 130, "am", InfoForAll, "E:\\��ģ����\\����New\\MCNP\\simulate_answer\\AM\\am_ans_129.txt");
	//calDCCK(129, "am", InfoForAll, "E:\\��ģ����\\����New\\MCNP\\simulate_answer\\AM\\am_ans_129.txt");
	//calDCCK(14, "am", InfoForAll);

	calDCCKRBMWithShow(instancename, InfoForAll);

	std::cout << "\nEffective dose for AM&AF with ergindex 6 = " << calEffectiveDose(instancename, InfoForAll) << std::endl;

	return 0;
}