#include"Experiment_1_header.h"
int main()
{
	using std::cout;
	using std::cin;
	ProcessList* PL = new ProcessList;
	//����һ�����̱�
	main_menu();
	int input_1;
	cin >> input_1;
	//��ҳ�����
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
			//�̽������ȵ���,����ռʽ
		case 2:
			system("cls");
			NonPreemptive_Mode(PL,Non_HRRN);
			break;
			//����Ӧ�����ȵ��ȣ�����ռʽ
		}
		break;
		//����ռ��ʽ
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
			//�̽������ȵ���
		case 2:
		{
			Preemptive_Mode(PL, Pre_HRRN);
			break;
		}
			//����Ӧ�����ȵ���
		case 3:
		{
			Preemptive_Mode(PL, TimePieceRR);
			break;
		}
			//ʱ��Ƭ��ת����
		}
		break;
		//��ռ��ʽ
	}
	delete PL;
	//���ٽ��̱�
	system("pause");
}