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
		//�������з�����
		system("cls");
		JobScheduling_DM(J);
		//��ҵ����
		break;
		//��̬��������-�״���Ӧ
	case 2:
		system("cls");
		//JobScheduling_Paging(J);
		//��ҵ����
		break;
		//������ҳ����-��ַ˳��
	}
	delete J;
	system("pause");

}

//int main(int argc, char* argv[])
//{
//	char c;
//	cout << "�������ڴ�Ĵ�С��";
//	cin >> neicun;
//	cout << "*******************************************************************************" << endl;
//	cout << "  1:������ҵ(a)   " << endl;
//	cout << "  2:��ʾ��ҵ(s)   " << endl;
//	cout << "  3:������ҵ(d)   " << endl;
//	cout << "  4.��ַת��(z)   " << endl;
//	cout << "  5:�����ڴ�(r)" << endl;
//	cout << "  6:��  ��(e)" << endl;
//	cout << "********************************************************************************" << endl;
//	while (1)
//	{
//		cout << "��ѡ���������(a/s/d/z/r/e):";
//		cin >> c;
//		if (c == 'e')
//		{
//			cout << "����������ϣ�ִ���˳�����" << endl;
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
//			cout << "error!����������!" << endl;
//	}
//	return 0;
//}
