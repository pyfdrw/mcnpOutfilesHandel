// 处理mcnp的输出文件(以o为最后的字母)，记录tally卡的数量，类型，数值，统计误差等信息

#include "tallyclass.h"

int main()
{
	std::string dirpath;
	// dirpath = filedirGet();
	// std::cout << dirpath << std::endl;
	
	//std::string filepath = "E:\\体模变形\\变形New\\MCNP\\simulate_answer\\test\\am_ap_0o";
	std::string filepath = "E:\\体模变形\\变形New\\MCNP\\simulate_answer\\test\\fortest.txt";
	singlefileHandel(filepath);



	return 0;
}