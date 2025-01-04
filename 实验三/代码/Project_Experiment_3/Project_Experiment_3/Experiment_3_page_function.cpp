#include"Experiment_3_header.h"
using namespace Paging;

//������ҵ�������뵽�󱸶���
Status JobScheduler::JobCreating(void)
{
	if (m_JobList.size() >= JobMaxNum)
		return finish;
	std::string temp_s;
	std::cout << "�����봴������ҵ�ı�ʶ����";
	std::cin >> temp_s;
	//������ҵ��ʶ��
	JCB temp_j = JCB(temp_s);
	m_JobList.push_back(temp_j);
	//���뵽�󱸶���
	m_BackUpQueue->push_back(&m_JobList[m_JobList.size() - 1]);
	//���뵽Job������
	return ok;
}

//�����ڴ�
Status JobScheduler::DeAllocateMemory(JCB* &j)
{
	for (auto i = m_RunQueue->begin(); i != m_RunQueue->end();i++)
	{
		if (j->m_Id == (*i)->m_Id)
		{
			for (int k = 0; k < j->m_pt.second; k++)
			{
				int SectionNo = (*(j->m_pt.first))[k];
				//�õ��κ�
				if (-1 != SectionNo)
				{
					(*m_MS)[SectionNo] = Free;
					//�޸��ڴ�״̬
				}
				(*(j->m_pt.first))[k] = -1;
				//��Ϊ-1
			}
			return ok;
		}
	}
	return wrong;
}

//��j��ҵ��ִ��״̬תΪ����״̬
Status JobScheduler::RunToReady(JCB* &j)
{
	for (auto i = m_RunQueue->begin(); i != m_RunQueue->end();)
	{
		if (j->m_Id == (*i)->m_Id)
		{
			if (wrong == this->DeAllocateMemory(*i))
				return wrong;
			j->m_state = JCB::State::W;
			//״̬
			m_BackUpQueue->push_back(*i);
			i = m_RunQueue->erase(i);
			return ok;
		}
		else
			i++;
	}
	return wrong;
}

//Ϊ��ҵj�����ڴ�
Status JobScheduler::AllocateMemory(JCB* &j)
{
	int pagenum = MemorySize / PageSize;
	PageTableType::iterator k = (j->m_pt.first)->begin();
	int i = 0;
	for (;i < pagenum and k != (j->m_pt.first)->end();i++)
	{
		if ((*m_MS)[i] == Free)
		{
			*k = i;
			//��Ե�ҳ�ŵõ����
			(*m_MS)[i] = Busy;
			//�޸Ŀ��״̬
			k++;
		}
	}
	if (i == pagenum and k != (j->m_pt.first)->end())
	{
		return wrong;
	}
	//�ڴ治��
	return ok;
}

//���ض���ҵ�Ӿ���״̬ת��ִ��״̬
Status JobScheduler::ReadyToRun(JCB* &j)
{
	for (auto i = m_BackUpQueue->begin(); i != m_BackUpQueue->end();)
	{
		if (j->m_Id == (*i)->m_Id)
		{
			if (wrong == AllocateMemory(*i))
				return wrong;
		    //�����ڴ�
			j->m_state = JCB::State::R;
			//��ҵ״̬
			m_RunQueue->push_back(*i);
			//���ִ�ж���
			i = m_BackUpQueue->erase(i);
			//���Ӻ󱸶���
			return ok;
		}
		else
			i++;
	}
	return wrong;
}

//ʱ��Ƭ�����㷨-������ҳ����
Status Paging::RobinRound_Paging(JobScheduler* &J)
{
	for (auto i = J->m_RunQueue->begin(); i != J->m_RunQueue->end();i = J->m_RunQueue->begin())
	{
		//����ҵ��ִ��״̬ת�����״̬
		if (ok != J->RunToReady(*i))
			return wrong;
	}
	int num = 0;
	for (auto i = J->m_BackUpQueue->begin(); i != J->m_BackUpQueue->end() and num < ProcessorNum;
		i = J->m_BackUpQueue->begin(),num++)
	{
		//��ǰ�����ҵ�Ӿ���״̬ת��ִ��״̬
		if (ok != J->ReadyToRun(*i))
			return wrong;
	}
	return ok;
}

//����ʱ������
void JobScheduler::RunTimeIncrease(void)
{
	for (auto i = m_RunQueue->begin(); i != m_RunQueue->end(); i++)
	{
		if ((*i)->m_ntime > 0)
		{
			(*i)->m_ntime--;
		}
	}
}

//����ɵ���ҵת�����״̬
void Paging::JobScheduler::FindHasFinish(void)
{
	for (auto i = m_RunQueue->begin(); i != m_RunQueue->end();)
	{
		if ((*i)->IfFinish() == ok)
		{
			(*i)->m_state = JCB::State::F;
			DeAllocateMemory(*i);
			i = m_RunQueue->erase(i);
		}
		else
			i++;
	}
}

//<<����
std::ostream& Paging::operator<<(std::ostream& os, Paging::JobScheduler* &J)
{
	using std::cout;
	using std::endl;
	int size = JobSize / PageSize;
	cout << "�ڴ�ʹ�������" << endl;
	for (auto i = J->m_RunQueue->begin(); i != J->m_RunQueue->end(); i++)
	{
		cout << "��ҵ��ʶ����" << (*i)->m_Id << endl
			 << "��ҵҳ��" << endl;
		for (int j = 0; j < size; j++)
		{
			cout << "ҳ��" << j << "��������" << "�κ�" << (*((*i)->m_pt.first))[j] << endl;
		}
	}
	cout << "�����ڴ棺" << endl;
	for (int i = 0;i < J->m_MS->size();i++)
	{
		if ((*(J->m_MS))[i] == Free)
		{
			cout << "�κ�" << i << endl;
		}
	}
	return os;
}

//��ҵ����(������ҳ����)
Status Paging::JobScheduling_Paging(JobScheduler* &J)
{
	//����ʱ�����
	int time_c = 0;
	//�����־
	int flag = ok, flag_c;
	while (finish != flag and wrong != flag)
	{
		std::cout << "-----------------" << std::endl
			<< "��" << time_c << "����λʱ��" << std::endl;
		//ÿ����λʱ�䴴��һ����ҵ
		flag_c = J->JobCreating();
		//ÿ2����λʱ��Ϊһ��ʱ��Ƭ
		if (time_c % 2 == 0)
		{
			if (ok != RobinRound_Paging(J))
				return wrong;
		}
		//����ʱ�����
		J->RunTimeIncrease();
		//��ʾ�ڴ�ʹ�����
		std::cout << J;
		//��׼ʱ������
		time_c++;
		//����Ƿ���ҵ���
		J->FindHasFinish();
		//�������Ƿ����
		if (finish == flag_c and J->m_RunQueue->size() == 0 and J->m_BackUpQueue->size() == 0)
			flag = finish;
	}
}

