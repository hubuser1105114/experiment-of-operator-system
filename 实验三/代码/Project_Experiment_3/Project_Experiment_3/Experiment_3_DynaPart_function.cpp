#include"Experiment_3_header.h"
using namespace Dynamic_Partition;
//��ҳ��
void main_menu()
{
	using std::cout;
	using std::endl;
	cout << "********************************" << endl
		<< "**                            **" << endl
		<< "** 1.��̬����-�״���Ӧ         **" << endl
		<< "** 2.������ҳ-��ַ˳��(�ǽ���)  **" << endl
		<< "** 3.�����ֶ�-�����Ӧ         **" << endl
		<< "**                            **" << endl
		<< "**                            **" << endl
		<< "**                            **" << endl
		<< "********************************" << endl;
}


//����������(��ʾ�����ڴ�)
std::ostream& Dynamic_Partition::operator<<(std::ostream& os, IdleListType* ML)
{
	using std::cout;
	using std::endl;
	cout << "�����ڴ棺" << endl;
	for (auto i = ML->begin(); i != ML->end(); i++)
	{
		cout << "�����ţ�" << i->m_num << endl;
		cout << "������С��" << i->m_size << endl;
		cout << "����ʼַ��" << i->m_OgAdr << endl;
		if (i->m_state == IdleListElemType::State::F)
		{
			cout << "����״̬��F(����״̬)" << endl;
		}
		cout << endl;
	}
	return os;
}

//����������
std::ostream& Dynamic_Partition::operator<<(std::ostream& os, QueueType* RunQueue)
{
	using std::cout;
	using std::endl;
	cout << "����ʹ�������" << endl;
	for (auto i = RunQueue->begin(); i != RunQueue->end(); i++)
	{
		cout << "����ʼַ��" << (*i)->m_PartGetAdr << endl;
		cout << "������С��" << JobSize << endl;
		cout << "����״̬��O(ռ��״̬)" << endl << endl;
	}
	return os;
}

//������ҵ����
Status JobScheduler::CreatingJob(void)
{
	if (m_JobList.size() >= JobMaxNum)
	{
		return finish;
	}
	//�����ҵ����
	std::string temp_s;
	std::cout << "�����봴������ҵ��ʶ����";
	std::cin >> temp_s;
	JCB temp_j = JCB(temp_s);
	//����һ����ҵ
	m_JobList.push_back(temp_j);
	//�Ѵ����õ���ҵ���뵽��ҵ������
	m_BackupQueue->push_back(&m_JobList[m_JobList.size() - 1]);
	//�Ѵ����õ���ҵ����󱸶�����
	return ok;
}

//ִ��ʱ����������
void JobScheduler::RunTimeIncreasing(void)
{
	for (auto i = m_RunQueue->begin(); i != m_RunQueue->end(); i++)
	{
		(*i)->m_ntime--;
	}
}

//��ʼ�����з�����
Status JobScheduler::Init_ML(void)
{
	m_ML->resize(MemorySize / PartitionSize);
	for (int i = 0; i < MemorySize / PartitionSize; i++)
	{
		(*m_ML)[i].m_num = i + 1;
		//�����ų�ʼ��
		(*m_ML)[i].m_size = PartitionSize;
		//������С
		if (i == 0)
		{
			(*m_ML)[i].m_OgAdr = 0;
		}
		else
		{
			(*m_ML)[i].m_OgAdr = PartitionSize + (*m_ML)[i - 1].m_OgAdr;
		}
		//����ʼַ
		(*m_ML)[i].m_state = IdleListElemType::State::F;
		//����״̬
	}
	return ok;
}

//Ϊ��ҵJ�����ڴ�
Status JobScheduler::AllocateMemory(JCB* &j)
{
	for (auto i = m_ML->begin(); i != m_ML->end();)
	{
		if (i->m_size > JobSize)
		{
			j->m_PartGetAdr = i->m_OgAdr;
			//����ʼַ
			i->m_OgAdr += JobSize;
			//�޸ķ���ʼַ
			i->m_size -= JobSize;
			//�޸ķ�����С
			return ok;
		}
		else if (i->m_size == JobSize)
		{
			j->m_PartGetAdr = i->m_OgAdr;
			//����ʼַ
			for (auto k = i + 1; k != m_ML->end(); k++)
			{
				k->m_num--;
			}
			//�޸ķ�����
			i = m_ML->erase(i);
			//ֱ��ɾ������
			return ok;
		}
		else
			i++;
	}
	return wrong;
}

//�Ѿ�����ҵת��ִ��״̬
Status JobScheduler::ReadyToRun(JCB* &j)
{
	for (auto i = m_BackupQueue->begin(); i != m_BackupQueue->end();)
	{
		if (j->m_Id == (*i)->m_Id)
		{
			(*i)->m_state = JCB::State::R;
			//�޸���ҵ״̬
			if (wrong == this->AllocateMemory(j))
			{
				return wrong;
			}
			//�����ڴ�ռ�
			m_RunQueue->push_back(*i);
			//���뵽ִ�ж�����
			i = m_BackupQueue->erase(i);
			//�Ӻ󱸶������Ƴ�
			return ok;
		}
		else
			i++;
	}
	return wrong;
}

//�����ڴ�
Status JobScheduler::DeallocateMemory(JCB* &j)
{
	//�������������ǰһ�����з������ڽ�
	for (auto i = 0; i < m_ML->size(); i++)
	{
		//�ҵ������
		if (j->m_PartGetAdr > (*m_ML)[i].m_OgAdr and ((*m_ML)[i].m_OgAdr + (*m_ML)[i].m_size == j->m_PartGetAdr))
		{
			(*m_ML)[i].m_size += JobSize;
			j->m_PartGetAdr = -1;
			//�޸���ҵj���ڴ���Ϣ
			return ok;
		}
	}
	
	//������������ĺ�һ���з������ڽ�
	for (auto i = 0; i < m_ML->size(); i++)
	{
		if (j->m_PartGetAdr < (*m_ML)[i].m_OgAdr and (j->m_PartGetAdr + JobSize == (*m_ML)[i].m_OgAdr))
		{
			(*m_ML)[i].m_OgAdr = j->m_PartGetAdr;
			(*m_ML)[i].m_size += JobSize;
			j->m_PartGetAdr = -1;
			//�޸���ҵj���ڴ���Ϣ
			return ok;
		}
	}

	//�������������ڽ�
	for (auto i = m_ML->begin(); i != m_ML->end() - 1; i++)
	{
		//Bug:û�п����ڶ�β�����
		//���Ƕ��ײ�������
		if (j->m_PartGetAdr < m_ML->begin()->m_OgAdr)
		{
			IdleListElemType temp;
			temp.m_num = 1;
			temp.m_OgAdr = j->m_PartGetAdr;
			temp.m_size = JobSize;
			temp.m_state = IdleListElemType::State::F;
			m_ML->insert(m_ML->begin(), temp);
			for (auto k = m_ML->begin() + 1; k != m_ML->end(); k++)
			{
				k->m_num++;
			}
			j->m_PartGetAdr = -1;
			return ok;
		}
		//�����ڶ��в�������
		else if (j->m_PartGetAdr > i->m_OgAdr)
		{
			IdleListElemType temp;
			temp.m_num = i->m_num + 1;
			temp.m_OgAdr = j->m_PartGetAdr;
			temp.m_size = JobSize;
			temp.m_state = IdleListElemType::State::F;
			for (auto k = i + 1; k != m_ML->end(); k++)
			{
				k->m_num++;
			}
			m_ML->insert(i + 1, temp);
			j->m_PartGetAdr = -1;
			return ok;
		}
	}
	return wrong;
}

//��ִ����ҵת�����״̬
Status JobScheduler::RunToReady(JCB* &j)
{
	for (auto i = m_RunQueue->begin(); i != m_RunQueue->end();)
	{
		if ((*i)->m_Id == j->m_Id)
		{
			(*i)->m_state = JCB::State::W;
			//�޸���ҵ״̬
			this->DeallocateMemory(j);
			//�����ڴ�
			m_BackupQueue->push_back(*i);
			//����󱸶���
			i = m_RunQueue->erase(i);
			//ɾ��ִ�н���
			return ok;
		}
		else
			i++;
	}
	return wrong;
}

//�����ҵ�Ƿ����
Status JobScheduler::TurnToFinish(void)
{
	for (auto i = this->m_RunQueue->begin(); i != this->m_RunQueue->end();)
	{
		if ((*i)->IfFinish())
		{
			if (wrong == this->DeallocateMemory(*i))
				return wrong;
			//�����ڴ�
			(*i)->m_state = JCB::State::F;
			//�޸���ҵ״̬
			i = this->m_RunQueue->erase(i);
		}
		else
			i++;
	}
	return ok;
}

//ʱ��Ƭ��ת����-��̬����
Status Dynamic_Partition::Round_Robin_DM(JobScheduler* &J)
{
	for (auto i = J->m_RunQueue->begin(); i != J->m_RunQueue->end(); i = J->m_RunQueue->begin())
	{
		J->RunToReady(*i);
		//��ִ�н���ת�����״̬
		//�����ڴ�
	}
	int num = 0;
	for (auto i = J->m_BackupQueue->begin(); i != J->m_BackupQueue->end() and num < ProcessorNum;
		 i = J->m_BackupQueue->begin(),num++)
	{
		J->ReadyToRun(*i);
		//�Ѿ�������ת��ִ��״̬
		//ͬʱ�����ڴ�
	}
	return ok;
}

//��ҵ���Ⱥ���
void  Dynamic_Partition::JobScheduling_DM(JobScheduler* &J)
{
	unsigned int time_c = 0;
	//���ñ�׼ʱ��
	int flag = ok , flag_cr;
	J->Init_ML();
	//��ʼ���������б�
	while (finish != flag and wrong != flag)
	{
		std::cout << "--------------" << std::endl;
		std::cout << "��" << time_c << "����׼ʱ��" << std::endl;
		flag_cr = J->CreatingJob();
		//ÿ����λʱ�䴴��һ����ҵ
		if (time_c % 2 == 0)
			//ʱ��ƬΪ2����λʱ��
		{
			
			Round_Robin_DM(J);
			//ʱ��Ƭ��ת�㷨
			//�����ڴ�ռ�
		}
		J->RunTimeIncreasing();
		//ִ�ж��е�ʱ����������
		J->TurnToFinish();
		//�����ҵ�Ƿ���ɣ�����ɣ�����ҵת�����״̬
		std::cout << J->m_ML;
		std::cout << J->m_RunQueue;
		//��ʾ�ڴ�״̬ͼ
		time_c++;
		//��׼ʱ������
		if (finish == flag_cr and J->m_BackupQueue->size() == 0 and J->m_RunQueue->size() == 0)
		{
			flag = finish;
		}
		//�����ҵ�����Ƿ��Ѿ����
	}
	
}
