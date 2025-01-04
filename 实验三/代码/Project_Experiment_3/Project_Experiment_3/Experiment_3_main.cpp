#include"Experiment_3_header.h"
#include <iostream>

int main()
{
	using namespace Dynamic_Partition;
	//using namespace Paging;
	main_menu();
	int input_1;
	std::cin >> input_1;
	JobScheduler* J = new JobScheduler;
	switch (input_1)
	{
	case 1:
		//创建空闲分区表
		system("cls");
		JobScheduling_DM(J);
		//作业调度
		break;
		//动态分区管理-首次适应
	case 2:
		system("cls");
		//JobScheduling_Paging(J);
		//作业调度
		break;
		//基本分页管理-地址顺序
	}
	delete J;
	system("pause");

}

//int main(int argc, char* argv[])
//{
//	char c;
//	cout << "请输入内存的大小：";
//	cin >> neicun;
//	cout << "*******************************************************************************" << endl;
//	cout << "  1:申请作业(a)   " << endl;
//	cout << "  2:显示作业(s)   " << endl;
//	cout << "  3:调度作业(d)   " << endl;
//	cout << "  4.地址转换(z)   " << endl;
//	cout << "  5:回收内存(r)" << endl;
//	cout << "  6:退  出(e)" << endl;
//	cout << "********************************************************************************" << endl;
//	while (1)
//	{
//		cout << "请选择服务种类(a/s/d/z/r/e):";
//		cin >> c;
//		if (c == 'e')
//		{
//			cout << "程序运行完毕，执行退出程序！" << endl;
//			break;
//		}
//		else if (c == 'a')
//			apply();
//		else if (c == 's')
//			show();
//		else if (c == 'd')
//			diaodu();
//		else if (c == 'z')
//			zhuanhuan();
//		else if (c == 'r')
//			reclaim();
//		else
//			cout << "error!请重新输入!" << endl;
//	}
//	return 0;
//}
