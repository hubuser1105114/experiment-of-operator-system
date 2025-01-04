#include"Experiment_1_header.h"
int main()
{
	using std::cout;
	using std::cin;
	ProcessList* PL = new ProcessList;
	//创建一个进程表
	main_menu();
	int input_1;
	cin >> input_1;
	//主页面测试
	switch (input_1)
	{
	case 1:
		system("cls");
		np_menu();
		int input_2;
		cin >> input_2;
		switch (input_2)
		{
		case 1:
			system("cls");
			NonPreemptive_Mode(PL,Non_ShortJobFirst);
			break;
			//短进程优先调度,非抢占式
		case 2:
			system("cls");
			NonPreemptive_Mode(PL,Non_HRRN);
			break;
			//高响应比优先调度，非抢占式
		}
		break;
		//非抢占方式
	case 2:
		system("cls");
		p_menu();
		int input_3;
		cin >> input_3;
		system("cls");
		switch (input_3)
		{
		case 1:
		{
			Preemptive_Mode(PL, Pre_ShortJobFirst);
			break;
		}
			//短进程优先调度
		case 2:
		{
			Preemptive_Mode(PL, Pre_HRRN);
			break;
		}
			//高响应比优先调度
		case 3:
		{
			Preemptive_Mode(PL, TimePieceRR);
			break;
		}
			//时间片轮转调度
		}
		break;
		//抢占方式
	}
	delete PL;
	//销毁进程表
	system("pause");
}