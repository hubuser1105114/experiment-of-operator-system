#include"Experiment_1_header.h"

//���һ�������ھ���������
Status ReadyQueue::AddRdProcess(PCB* p)
{
	if (this->m_ReadyList.size() >= ProcessMaxNum)return finish;
	//�����������еĽ���
	this->m_ReadyList.push_back(p);
	//Ĭ�������ȷ���(FCFS)�㷨
	p->m_state = PCB::State::W;
	//�޸Ľ���״̬Ϊ����̬
	return ok;
}

//����һ���½���,�������ھ���������
Status ProcessList::CreateNewProcess(void)
{
	if (this->m_PCBList.size() >= ProcessMaxNum)return finish;
	//����������Ľ��������ڽ����������������ɱ�־
	PCB* temp_p = nullptr;
	std::string temp_s;
	int temp_int;
	std::cout << "�������½��̵ı�ʶ����" << std::endl;
	std::cin >> temp_s;
	std::cout << "�������½��̵�Ҫ�����ʱ�䣺" << std::endl;
	std::cin >> temp_int;
	temp_p = new PCB({ temp_s,temp_int });
	//����һ�������û��Լ�����ı�ʶ���Ľ���
	if (nullptr == temp_p)return wrong;
	//�ڴ治�㣬����wrong
	this->m_PCBList.push_back(*temp_p);
	//���´����Ľ�������PCB�б���
	PCB* temp_p2 = &(this->m_PCBList[this->m_PCBList.size() - 1]);
	//ȡPCB�б����½��̵ĵ�ַ
	delete temp_p;
	if (ok == this->m_Readyp->AddRdProcess(temp_p2))
	{
		return ok;
	}
	//���´����Ľ��̼������������
	else
	{
		return wrong;
	}
}

//���ض����̼��뵽ִ�ж�����
Status RunQueue::AddRunProcess(PCB* p)
{
	if (this->m_RunList.size() >= ProcessorNum)return finish;
	//�������5����������
	this->m_RunList.push_back(p);
	//�ѽ��̼�����ִ����������
	p->m_state = PCB::State::R;
	//�޸Ľ���״̬��־
	p->m_wtime = 0;
	//�ѽ��̵ĵȴ�ʱ����0
	return ok;
}

//���ض����̴�ִ�ж�������ȥ��SΪ���̵ĺ���״̬
Status RunQueue::DeleteRunProcess(PCB* p, PCB::State S)
{
	std::string id = p->m_Id;
	for (auto i = this->m_RunList.begin(); i != this->m_RunList.end();)
	{
		if (id == (*i)->m_Id)
		{
			p->m_state = S;
			//�޸�״̬
			i = this->m_RunList.erase(i);
			//���б�����ȥ�ض�����
			return ok;
		}
		else
			i++;
	}
	return wrong;
	//δ�ҵ�������wrong
}

//���ִ����������������������Ľ����Ƴ�
Status RunQueue::TurnProcessFinish(void)
{
	for (auto i = this->m_RunList.begin(); i != this->m_RunList.end();)
	{
		if (true == (*i)->IfProcessDone())
			//���������ִ��
		{
			//�޸Ľ���״̬
			(*i)->m_state = PCB::State::F;
			//�Ƴ��˽���
			i = this->m_RunList.erase(i);
		}
		else
			i++;
	}
	return ok;
}

//�ھ��������г���һ������
Status ReadyQueue::DeleteRdProcess(PCB* p)
{
	std::string id = p->m_Id;
	for (auto i = this->m_ReadyList.begin(); i != this->m_ReadyList.end();)
	{
		if (id == (*i)->m_Id)
		{
			i = this->m_ReadyList.erase(i);
			//��ȥ�ض�����
			return ok;
		}
		else
			i++;
	}
	return wrong;
	//δ�ҵ�����
}

//���캯�����壬��Ҫ����ִ��������;����������ڴ�ռ�
ProcessList::ProcessList(void)
{
	this->m_Runp = new RunQueue;
	this->m_Readyp = new ReadyQueue;
	this->m_PCBList.reserve(ProcessMaxNum);
	//ΪPCB�б�Ԥ���㹻�Ŀռ�
}

//������������Ҫ�ͷ��ڴ�ռ�
ProcessList::~ProcessList(void)
{
	delete this->m_Runp;
	delete this->m_Readyp;
}

//���ض��Ľ��̴Ӿ���״̬ת��ִ��״̬
Status ProcessList::RunReadyProcess(PCB* p)
{
	if (ok != this->m_Readyp->DeleteRdProcess(p))
	{
		return wrong;
	}
	//���ض����̴Ӿ��������г���
	Status flag = this->m_Runp->AddRunProcess(p);
	if(ok != flag and finish != flag)
	{
		return wrong;
	}
	else if (finish == flag)
	{
		return finish;
	}
	//���ض��������ִ�ж�����
	return ok;
}

//���ض����̴�ִ�ж�����ת���������(���������ж�)
Status ProcessList::DiscontinueProcess(PCB* p)
{
	if (ok != this->m_Runp->DeleteRunProcess(p, PCB::State::W))
	{
		return wrong;
	}
	//�ѽ��̴�ִ�ж������Ƴ�
	if (ok != this->m_Readyp->AddRdProcess(p))
	{
		return wrong;
	}
	//�ѽ�����Ӿ�������
	return ok;
}

//���̱���ʾ
std::ostream& operator<<(std::ostream& os, const ProcessList& P)
{
	using std::cout;
	using std::endl;
	cout << "�������еĽ���(�Ա�ʶ����ʾ����)��";
	for (auto i = (P.m_Runp)->m_RunList.begin(); i != (P.m_Runp)->m_RunList.end(); i++)
	{
		cout << (*i)->m_Id << " ";
	}
	cout << endl;
	//��ʾ�������еĽ���
	cout << "���о����Ľ���(�Ա�ʶ����ʾ����)��";
	for (auto i = (P.m_Readyp)->m_ReadyList.begin(); i != (P.m_Readyp)->m_ReadyList.end(); i++)
	{
		cout << (*i)->m_Id << " ";
	}
	cout << endl;
	//��ʾ���о�������
	cout << "������ɵĽ���(�Ա�ʶ����ʾ����)��";
	for (auto i = (P.m_PCBList).begin(); i != (P.m_PCBList).end(); i++)
	{
		if (i->m_state == PCB::State::F)
		{
			cout << i->m_Id << " ";
		}
		//�жϽ��̱�־
	}
	cout << endl;
	//��ʾ��ɵĽ���
	cout << "���н��̵�PCB��Ϣ��" << endl;
	for (auto i = P.m_PCBList.begin(); i != P.m_PCBList.end(); i++)
	{
		cout << "���̱�ʶ����" << i->m_Id << endl;
		if (i->m_state == PCB::State::F)
		{
			cout << "����״̬��F(���״̬)" << endl;
		}
		else if (i->m_state == PCB::State::R)
		{
			cout << "����״̬��R(ִ��״̬)" << endl;
		}
		else if (i->m_state == PCB::State::W)
		{
			cout << "����״̬��W(����״̬)" << endl;
		}
		cout << "����Ҫ�����ʱ�䣺" << i->m_ntime << endl;
		cout << "�����Ѿ���÷���ʱ�䣺" << i->m_rtime << endl;
		cout << "�����ѵȴ�ʱ�䣺" << i->m_wtime << endl;
		cout << "������תʱ�䣺" << i->turnaround_time << endl;
		cout << endl;
	}
	//��ʾ���н��̵�PCB��Ϣ
	return os;
}

//����ռ��ʽ�Ķ̽��������㷨
PCB* Non_ShortJobFirst(ReadyQueue* &Rq)
{
	int min = INT_MAX;
	PCB* min_p = nullptr;
	for (auto i = Rq->m_ReadyList.begin(); i != Rq->m_ReadyList.end(); i++)
	{
		if (min > (*i)->m_ntime)
		{
			min = (*i)->m_ntime;
			min_p = *i;
		}
	}
	return min_p;
	//���ؾ�����������Ҫ�����ʱ����̵��Ǹ����̣���û�У�����nullptr
}

//����ռ��ʽ�ĸ���Ӧ�����ȵ����㷨
PCB* Non_HRRN(ReadyQueue* &Rq)
{
	float max = 0;
	PCB* max_p = nullptr;
	for (auto i = Rq->m_ReadyList.begin(); i != Rq->m_ReadyList.end(); i++)
	{
		float rp = ((*i)->m_wtime + (*i)->m_ntime) / (*i)->m_ntime;
		//�������Ӧ�����ȼ�,rpԽ�����ȼ�Խ��
		if (max < rp)
		{
			max = rp;
			max_p = *i;
		}
	}
	return max_p;
	//���ؾ������������ȼ���ߵĽ��̡���û�У���ô����nullptr

}

//��ռʽ�Ķ̽��������㷨
Status Pre_ShortJobFirst(ProcessList* &P)
{
	//��������δȫ��æ״̬�����ȵ��Ƚ���ʹ�����ȫ��æ
	if (true != P->m_Runp->FullRunQueue())
	{
		for (auto i = P->m_Readyp->m_ReadyList.begin(); i != P->m_Readyp->m_ReadyList.end() 
			 and true != P->m_Runp->FullRunQueue(); i = P->m_Readyp->m_ReadyList.begin())
		{
			if (ok != P->RunReadyProcess(*i))
				return wrong;
		}
	}
	//˭ʱ���٣�˭ֱ�ӽ���ִ�ж�����
	for (auto i = P->m_Readyp->m_ReadyList.begin(); i != P->m_Readyp->m_ReadyList.end(); i++)
		//�鿴���������е�ÿһ�����̣���ִ�ж����е�ÿ��������Ƚ�
	{
		for (auto j = P->m_Runp->m_RunList.begin(); j != P->m_Runp->m_RunList.end(); j++)
		{
			if ((*i)->m_ntime - (*i)->m_rtime < (*j)->m_ntime - (*j)->m_rtime)
				//����������еĽ���С��ִ�ж����������е�ʱ��
			{
				//�޸ľ������еĽ���״̬
				(*i)->m_state = PCB::State::R;
				//�޸�ִ�ж��еĽ���״̬
				(*j)->m_state = PCB::State::W;
				//�ȴ�ʱ���0
				(*i)->m_wtime = 0;
				//ֻ����������̵ĵ�ַ��������
				PCB* temp = *i;
				*i = *j;
				*j = temp;
			}
		}
	}
	return ok;
}

//��ռʽ�ĸ���Ӧ�����ȵ����㷨
Status Pre_HRRN(ProcessList* &P)
{
	//��������δȫ��æ״̬�����ȵ��Ƚ���ʹ�����ȫ��æ
	if (true != P->m_Runp->FullRunQueue())
	{
		for (auto i = P->m_Readyp->m_ReadyList.begin(); i != P->m_Readyp->m_ReadyList.end()
			and true != P->m_Runp->FullRunQueue();i = P->m_Readyp->m_ReadyList.begin())
		{
			if (ok != P->RunReadyProcess(*i))
				return wrong;
		}
	}
	//˭���ȼ��ߣ�˭ֱ�ӽ���ִ�ж�����
	for (auto i = P->m_Readyp->m_ReadyList.begin(); i != P->m_Readyp->m_ReadyList.end(); i++)
		//�鿴���������е�ÿһ�����̣���ִ�ж����е�ÿ��������Ƚ�
	{
		//����i���̵�����Ȩ
		float super_i = ((*i)->m_wtime + (*i)->m_ntime) / (*i)->m_ntime;
		for (auto j = P->m_Runp->m_RunList.begin(); j != P->m_Runp->m_RunList.end(); j++)
		{
			//����j���̵�����Ȩ
			float super_j = ((*j)->m_wtime + (*j)->m_ntime) / (*j)->m_ntime;
			if (super_i > super_j)
				//����������еĽ��̵�����Ȩ����ִ�ж��еĽ��̵�����Ȩ
			{
				//�޸ľ������еĽ���״̬
				(*i)->m_state = PCB::State::R;
				//�޸�ִ�ж��еĽ���״̬
				(*j)->m_state = PCB::State::W;
				//�ȴ�ʱ���0
				(*i)->m_wtime = 0;
				//ֻ����������̵ĵ�ַ��������
				PCB* temp = *i;
				*i = *j;
				*j = temp;
			}
		}
	}
	return ok;
}

//ʱ��Ƭ��ת�㷨
Status TimePieceRR(ProcessList* &P)
{
	//�涨�ָ�ÿһ���������еĽ��̵�CPU����ʱ��Ϊ2����λʱ��
	//����FCFS����
	//����erase��push_back�����о������еĵ���
			//�ѽ����ж�ת�����������
	for (auto i = P->m_Runp->m_RunList.begin(); i != P->m_Runp->m_RunList.end()
		; i = P->m_Runp->m_RunList.begin())
	{
		//ɾȥִ�ж��еĶ��׽���
		P->DiscontinueProcess(*i);
	}
    //ѭ��ProcessorMaxNum��
		//�ѽ��̰�ʱ��Ƭ��ת�㷨����ִ�ж�����
		for (int num = 0; num < ProcessorNum and
			P->m_Readyp->m_ReadyList.begin() != P->m_Readyp->m_ReadyList.end(); num++)
		{
			//�������е����׽��̼���ִ�ж�����
			P->RunReadyProcess(*(P->m_Readyp->m_ReadyList.begin()));
		}
		return ok;
}

//�ȴ�ʱ����������
void ReadyQueue::WaitTimeAdd(void)
{
	for (auto i = this->m_ReadyList.begin(); i != this->m_ReadyList.end(); i++)
	{
		(*i)->m_wtime++;
	}
}

//ִ��ʱ����������
void RunQueue::RunTimeAdd(void)
{
	for (auto i = this->m_RunList.begin(); i != this->m_RunList.end(); i++)
	{
		(*i)->m_rtime++;
	}
}

//��תʱ����������
void ProcessList::TurnAroundTimeAdd(void)
{
	for (auto i = this->m_Readyp->m_ReadyList.begin(); i != this->m_Readyp->m_ReadyList.end(); i++)
	{
		(*i)->turnaround_time++;
	}
	for (auto i = this->m_Runp->m_RunList.begin(); i != this->m_Runp->m_RunList.end(); i++)
	{
		(*i)->turnaround_time++;
	}
}

//����ƽ����תʱ��
float ProcessList::cal_turnaroundtime(void)const
{
	float sum = 0;
	for (auto i = this->m_PCBList.begin(); i != this->m_PCBList.end(); i++)
	{
		sum += i->turnaround_time;
	}
	return sum / this->m_PCBList.size();
}

//�����Ȩƽ����תʱ��
float ProcessList::cal_w_turnaroundtime(void)const
{
	float sum = 0;
	for (auto i = this->m_PCBList.begin(); i != this->m_PCBList.end(); i++)
	{
		sum += (float(i->turnaround_time) / float(i->m_ntime));
	}
	return sum / ProcessMaxNum;
}

//ҳ�溯��
void main_menu(void)
{
	using std::cout;
	using std::endl;
	cout << "**************************************" << endl;
	cout << "**************************************" << endl;
	cout << "**              ���̵���             **" << endl;
	cout << "**                                   **" << endl;
	cout << "**                                   **" << endl;
	cout << "**  1.����ռ��ʽ         2.��ռ��ʽ   **" << endl;
	cout << "**                                   **" << endl;
	cout << "**                                   **" << endl;
	cout << "**                                   **" << endl;
	cout << "**                                   **" << endl;
	cout << "**************************************" << endl;
	cout << "**************************************" << endl;
}

//����ռ��ʽ����ҳ��
void np_menu(void)
{
	using std::cout;
	using std::endl;
	cout << "**************************************" << endl;
	cout << "**************************************" << endl;
	cout << "**        ����ռ��ʽ-���̵���         **" << endl;
	cout << "**                                   **" << endl;
	cout << "**                                   **" << endl;
	cout << "**                                   **" << endl;
	cout << "**        1.�̽������ȵ���            **" << endl;
	cout << "**        2.����Ӧ�����ȵ���           **" << endl;
	cout << "**                                   **" << endl;
	cout << "**                                   **" << endl;
	cout << "**                                   **" << endl;
	cout << "**************************************" << endl;
	cout << "**************************************" << endl;


}

//��ռ��ʽ����ҳ��
void p_menu(void)
{
	using std::cout;
	using std::endl;
	cout << "**************************************" << endl;
	cout << "**************************************" << endl;
	cout << "**        ��ռ��ʽ-���̵���          **" << endl;
	cout << "**                                   **" << endl;
	cout << "**                                   **" << endl;
	cout << "**                                   **" << endl;
	cout << "**        1.�̽������ȵ���            **" << endl;
	cout << "**        2.����Ӧ�����ȵ���           **" << endl;
	cout << "**        3.ʱ��Ƭ��ת                **" << endl;
	cout << "**                                   **" << endl;
	cout << "**                                   **" << endl;
	cout << "**************************************" << endl;
	cout << "**************************************" << endl;
}

//����ռ���̵��ȷ�ʽ,��һ���������ս��̱��ڶ����������յ����㷨
Status NonPreemptive_Mode(ProcessList* &P, PCB* (*ProcessSchedule)(ReadyQueue* &Rq))
{
	//��������
	Status flag = ok;
	Status flag1;
	unsigned int time_c = 0;
	//�����������������
	std::default_random_engine rand_num;
	PCB* super_p = nullptr;
	while (finish != flag and wrong != flag)
		//ѭ��ֱ���������ﵽ��������߳��ֳ������ͽ���
		//һ��ѭ����ʾ����һ����λʱ��
	{
		std::cout << "-----------------" << std::endl;
		std::cout << "��" << time_c << "����׼��λʱ��" << std::endl;
		int ra = rand_num();
		//���ʱ������һ���½���
		if (time_c == 0 or ra % 4 != 0)
			//����������Բ���4����ʱ�����½���(��ٵ����)����3/4�ĸ���
		{
			//�������̣�����flag
			flag1 = P->CreateNewProcess();
		}
			//ʹ�õ����㷨ѡ�����ȼ���ߵ�
			super_p = ProcessSchedule(P->m_Readyp);
			//�ж��Ƿ�Ϊnullptr;
			if (nullptr != super_p)
				//�ǣ������´�ѭ��(��Ϊ��������û�н�����);
			{
				//���ǣ��Ȳ鿴5�������æ�������
				if (true != P->m_Runp->FullRunQueue())
					//û�п���λ�ã���ô�����ȴ�
				{
					//�п���λ�ã�����ִ�ж�����
					if (ok != P->RunReadyProcess(super_p))
					{
						flag = wrong;
					}
				}
			}
			//ȱ�㣺һ����λʱ��ֻ����ɶ�һ�����̵ĵ���
		//��������ʱ������
		P->m_Readyp->WaitTimeAdd();
		//ִ�ж���ʱ������
		P->m_Runp->RunTimeAdd();
		//������תʱ��
		P->TurnAroundTimeAdd();
		//���ִ������������ִ�������Ҫ������ִ���������Ƴ�
		if (ok != P->m_Runp->TurnProcessFinish())
			return wrong;
		//��ʾ��׼ʱ��
		time_c++;
		//��ʾ������Ϣ
		std::cout << *P;
		std::cout << "-----------------" << std::endl;
		if (flag1 == finish and P->m_Readyp->m_ReadyList.size() == 0 and P->m_Runp->m_RunList.size() == 0)
		{
			flag = finish;
		}
		//Sleep(1000);
	}
	std::cout << "ƽ����תʱ�䣺" << P->cal_turnaroundtime() << std::endl;
	std::cout << "ƽ����Ȩ��תʱ�䣺" << P->cal_w_turnaroundtime() << std::endl;
	return ok;
}

//��ռʽ���̵��ȷ�ʽ
Status Preemptive_Mode(ProcessList* &P, Status (*Pre_ProcessSchedule)(ProcessList* &P))
{
	Status flag = ok;
	Status flag1;
	unsigned int time_c = 0;
	//�����������������
	std::default_random_engine rand_num;
	PCB* super_p = nullptr;
	while (finish != flag and wrong != flag)
		//ѭ��ֱ���������ﵽ��������߳��ֳ������ͽ���
		//һ��ѭ����ʾ����һ����λʱ��
	{
		std::cout << "-----------------" << std::endl;
		std::cout << "��" << time_c << "����׼��λʱ��" << std::endl;
		int ra = rand_num();
		//���ʱ������һ���½���
		if (time_c == 0 or ra % 4 != 0)
			//����������Բ���4����ʱ�����½���(��ٵ����)����3/4�ĸ���
		{
			//�������̣�����flag1
			flag1 = P->CreateNewProcess();
		}
		//ʱ��Ƭ��תҪ��2����λʱ�������
		if (Pre_ProcessSchedule == TimePieceRR)
		{
			if (time_c % 2 == 0)
			{
				if (ok != Pre_ProcessSchedule(P))
					return wrong;
			}
		}
		//��ռʽ�Ľ��̵����㷨
		else if (ok != Pre_ProcessSchedule(P))
		{
			return wrong;
		}
		//��������ʱ������
		P->m_Readyp->WaitTimeAdd();
		//ִ�ж���ʱ������
		P->m_Runp->RunTimeAdd();
		//������תʱ��
		P->TurnAroundTimeAdd();
		//���ִ������������ִ�������Ҫ������ִ���������Ƴ�
		if (ok != P->m_Runp->TurnProcessFinish())
			return wrong;
		//��ʾ��׼ʱ��
		time_c++;
		//��ʾ������Ϣ
		std::cout << *P;
		std::cout << "-----------------" << std::endl;
		if (flag1 == finish and P->m_Readyp->m_ReadyList.size() == 0 and P->m_Runp->m_RunList.size() == 0)
		{
			flag = finish;
		}
		Sleep(1000);
	}
	std::cout << "ƽ����תʱ�䣺" << P->cal_turnaroundtime() << std::endl;
	std::cout << "ƽ����Ȩ��תʱ�䣺" << P->cal_w_turnaroundtime() << std::endl;
	return ok;
}








