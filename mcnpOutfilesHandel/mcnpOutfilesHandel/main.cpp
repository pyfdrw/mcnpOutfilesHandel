// ����mcnp������ļ�(��oΪ������ĸ)����¼tally�������������ͣ���ֵ��ͳ��������Ϣ

#include "tallyclass.h"

int main()
{
	std::string dirpath;

	// dirpath = filedirGet();
	// std::cout << dirpath << std::endl;
	dirpath = "E:\\��ģ����\\����New\\MCNP\\Allinput\\SimANS\\RightAns\\";
	//std::string filepath = "E:\\��ģ����\\����New\\MCNP\\simulate_answer\\test\\am_ap_0o";
	//std::string filepath = "E:\\��ģ����\\����New\\MCNP\\simulate_answer\\test\\fortest.txt";
	//singlefileHandel(filepath);
	AllInfo InfoForAll;
	allinfoStore(InfoForAll, dirpath);

	// showSinglePhantomInfo("rpiam", InfoForAll);
	PhantomAgeDirErg instancename("rpiam_ap_2");
	// 
	// showSinglePhantomInfo(instancename, InfoForAll);

	//calDCCK(129, 130, "am", InfoForAll);
	//calDCCK(111, 112, "af", InfoForAll);
	//calDCCK(97, 99, "am", InfoForAll);
	// calDCCK(129, "am", InfoForAll);

	//calDCCK(129, 130, "am", InfoForAll, "E:\\��ģ����\\����New\\MCNP\\simulate_answer\\AM\\am_ans_129.txt");
	//calDCCK(129, "am", InfoForAll, "E:\\��ģ����\\����New\\MCNP\\simulate_answer\\AM\\am_ans_129.txt");
	//calDCCK(14, "am", InfoForAll);

	// calDCCKRBMWithShow(instancename, InfoForAll);
	
	//for (int i = 0; i < ergall.size();  i++)
	//{
		//instancename.name = "rpiaf_rot_" + ergall[i];
		//std::cout << "erg  == " << ergall_val[i] << std::endl;
		// calDCCKRBMWithShow(instancename, InfoForAll);
		// std::cout << "\nEffective dose for rpiAM&rpiAF with ergindex " << ergall_val[i] << " =  " << calEffectiveDose(instancename, InfoForAll) << std::endl;
	//}
	 
	// calDCCKWithShow(63, 65, "rpiaf", InfoForAll);

	dataControlFun(dirpath, InfoForAll);

	return 0;
}