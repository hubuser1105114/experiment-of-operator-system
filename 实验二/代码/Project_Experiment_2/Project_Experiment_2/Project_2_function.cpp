#include"Project_2_header.h"
//�жϽ����Ƿ�ﵽ�������
Status PCB::IfFinish(void)const
{
	for (auto i = 0; i < ResourceKinds; i++)
	{
		if (this->m_hadR[i] < this->m_maxneedR[i])
			return false;
	}
	//ֻҪ��һ����Դû�дﵽ������󣬾��㲻���
	return ok;
}

//<<��������� ����ʾ���̱����Ϣ
std::ostream& operator<<(std::ostream& os, ProcessList& P)
{
	using std::cout;
	using std::endl;
	cout << "ϵͳ�е���Դ��������";
	cout << "(A:" << P.m_R.A_num << "  B:" << P.m_R.B_num
		<< "  C:" << P.m_R.C_num << ")" << endl;
	cout << "�������еĽ���(�Ա�ʶ����ʾ)��";
	for (auto i = (P.m_RunL)->begin(); i != (P.m_RunL)->end(); i++)
	{
		cout << (*i)->m_Id << " ";
	}
	cout << endl;
	cout << "���������еĽ���(�Ա�ʶ����ʾ)��";
	for (auto i = (P.m_ReadyL)->begin(); i != (P.m_ReadyL)->end(); i++)
	{
		cout << (*i)->m_Id << " ";
	}
	cout << endl;
	cout << "���������еĽ���(�Ա�ʶ����ʾ)��";
	for (auto i = (P.m_BlockL)->begin(); i != (P.m_BlockL)->end(); i++)
	{
		cout << (*i)->m_Id << " ";
	}
	cout << endl;
	cout << "��ɵĽ���(�Ա�ʶ����ʾ)��";
	for (auto i = P.m_PCBList.begin(); i != P.m_PCBList.end(); i++)
	{
		if (i->m_state == PCB::State::F)
		{
			cout << i->m_Id << " ";
		}
	}
	cout << endl;
	cout << "���н��̵�PCB��Ϣ��" << endl;
	for (auto i = P.m_PCBList.begin(); i != P.m_PCBList.end(); i++)
	{
		cout << "���̱�ʶ����" << i->m_Id << endl;
		cout << "����״̬��";
		if (i->m_state == PCB::State::B)
		{
			cout << "B(����״̬)";
		}
		else if (i->m_state == PCB::State::F)
		{
			cout << "F(���״̬)";
		}
		else if (i->m_state == PCB::State::R)
		{
			cout << "R(����״̬)";
		}
		else if (i->m_state == PCB::State::W)
		{
			cout << "W(����״̬)";
		}
		cout << endl;
		cout << "�����Ҫ����Դ������" << endl;
		cout << "A:" << i->m_maxneedR[Resource::A] << "  B:" << i->m_maxneedR[Resource::B]
			 << "  C:" << i->m_maxneedR[Resource::C] << endl;
		cout << "�Ѿ���õ���Դ������" << endl;
		cout << "A:" << i->m_hadR[Resource::A] << "  B:" << i->m_hadR[Resource::B]
			<< "  C:" << i->m_hadR[Resource::C] << endl << endl;
	}
	return os;
}

//����һ������
Status ProcessList::CreatingProcess(void)
{
	if (this->m_PCBList.size() >= ProcessMaxNum)
		return finish;
	//����Ƿ��Ѵﵽ���������
	if (this->m_BlockL->size() + this->m_ReadyL->size() + this->m_RunL->size() >= ConcurrencyNum)
		return ok;
	//����Ƿ�ﵽ�������
	    std::string temp_s;
		std::cout << "�����봴�����̵ı�ʶ����";
		std::cin >> temp_s;
		PCB temp_p = PCB(temp_s);
		//����һ���µ�PCB����
		this->m_PCBList.push_back(temp_p);
		//���µĽ���װ��PCB�б���
		this->m_ReadyL->push_back(&this->m_PCBList[this->m_PCBList.size() - 1]);
		//���µĽ���װ�����������
	    return ok;
}

//���ض����̴Ӿ���״̬ת������״̬
Status ProcessList::ReadyToBlock(const PCB* p)
{
	for (auto i = this->m_ReadyL->begin(); i != this->m_ReadyL->end();)
	{
		if ((*i)->m_Id == p->m_Id)
		{
			(*i)->m_state = PCB::State::B;
			//�޸Ľ���״̬
			this->m_BlockL->push_back(*i);
			//����p���̵�����������
			i = this->m_ReadyL->erase(i);
			//�ھ���������ɾȥp����
			return ok;
		}
		else
			i++;
	}
	return wrong;
}

//���ִ�н����Ƿ�ﵽ��ɱ�־
Status ProcessList::CheckUpFinish(void)
{
	if (0 == this->m_RunL->size() and 0 == this->m_ReadyL->size() and 0 != this->m_BlockL->size())
	{
		this->WakeUpBlock();
	}
	if (0 == this->m_RunL->size())
		//���ִ�ж�����û�н��̣���ô���·���
		return ok;
	if (1 == this->m_RunL->size() and this->m_RunL->front()->IfFinish() == ok)
		//������̴ﵽ�������
	{
		PCB* front = this->m_RunL->front();
		this->m_R.A_num += front->m_hadR[Resource::A];
		this->m_R.B_num += front->m_hadR[Resource::B];
		this->m_R.C_num += front->m_hadR[Resource::C];
		//�黹��Դ
		front->m_hadR[Resource::A] = front->m_hadR[Resource::B]
			= front->m_hadR[Resource::C] = 0;
		//�޸�������Դ��
		front->m_state = PCB::State::F;
		//�޸�״̬
		this->m_RunL->pop_front();
		//ɾ��ִ�ж��еĶ��׽���
		this->CreatingProcess();
		//���һ�����̣��ʹ���һ������
		this->WakeUpBlock();
		//���һ�����̣�������Ӧ����������
	}
	return ok;
}

//���������еĶ��׽���ת�뵽����������
Status ProcessList::WakeUpBlock(void)
{
	if (this->m_BlockL->size() == 0)
		return ok;
	//������Ϊ0����ô���·���
	auto i = this->m_BlockL->begin();
	//ȡ���׽���
	(*i)->m_state = PCB::State::W;
	//�޸�״̬
	this->m_ReadyL->push_back(*i);
	//�����������
	this->m_BlockL->pop_front();
	//ɾ������Ԫ��
	return ok;
}

//��ȫ���㷨
Status ProcessList::Security_algo(PCB* & p,const ResourceType& Available, int** Need, int** Allocation, std::vector<PCB*>& IndexList)
{
	int index = GetListIndex(p, IndexList);
	//�õ���������
	int size = IndexList.size();
	//�õ����̼��ϵĴ�С
	//����2�����������ݽṹ
	//��������Work
	ResourceType Work = Available;
	//�������
	bool* Finish = new bool[size] {false};
	int j = 0;
	for (auto i = IndexList.begin(); i != IndexList.end(); i++,j++)
	{
		if (Finish[j] == false)
		{
			bool sign = true;
			//������־
			for (int k = 0; k < ResourceKinds; k++)
			{
				if (Need[j][k] > Work[k])
				{
					sign = false;
					break;
				}
			}
			//������������Ľ���
			if (sign == true)
			{
				for (int k = 0; k < ResourceKinds; k++)
				{
					Work[k] += Allocation[j][k];
				}
				Finish[j] = true;
				i = IndexList.begin();
				j = 0;
			}
		}
	}
	for (int i = 0; i < size; i++)
	{
		if (Finish[i] != true)
		{
			delete[] Finish;
			return ShouldBlock;
		}
	}
	delete[] Finish;
	return ok;
}

//��������
int GetListIndex(PCB* &p, std::vector<PCB*>& List)
{
	int j = 0;
	for (auto i = List.begin(); i != List.end(); i++, j++)
	{
		if (p->m_Id == (*i)->m_Id)
		{
			return j;
		}
	}
	return -1;
}

//���м��㷨,����������Դ
Status ProcessList::Banker(PCB*  &p, ResourceType& Request)
{
	std::vector<PCB*> IndexList;
	//��������
	for (auto i = this->m_ReadyL->begin(); i != this->m_ReadyL->end(); i++)
	{
		IndexList.push_back(*i);
	}
	for (auto i = this->m_BlockL->begin(); i != this->m_BlockL->end(); i++)
	{
		IndexList.push_back(*i);
	}
	//��ʼ����������
	//�ȴ���4�����ݽṹ
	//��������Դ����Available
	ResourceType Available;
	//��ʼ��Available
	Available[Resource::A] = this->m_R.A_num;
	Available[Resource::B] = this->m_R.B_num;
	Available[Resource::C] = this->m_R.C_num;
	int size = IndexList.size();
	//����������Max
	int** Max = new int*[size];
	//��ʼ��Max
	int i = 0;
	for (auto j = IndexList.begin();j != IndexList.end();j++,i++)
	{
		Max[i] = new int[ResourceKinds];
		Max[i][Resource::A] = (*j)->m_maxneedR[Resource::A];
		Max[i][Resource::B] = (*j)->m_maxneedR[Resource::B];
		Max[i][Resource::C] = (*j)->m_maxneedR[Resource::C];
	}
	//�������Allocation
	int** Allocation = new int*[size];
	//��ʼ��Allocation
	i = 0;
	for (auto j = IndexList.begin();j != IndexList.end();j++,i++)
	{
		Allocation[i] = new int[ResourceKinds];
		Allocation[i][Resource::A] = (*j)->m_hadR[Resource::A];
		Allocation[i][Resource::B] = (*j)->m_hadR[Resource::B];
		Allocation[i][Resource::C] = (*j)->m_hadR[Resource::C];
	}
	//�������Need
	int** Need = new int*[size];
	//��ʼ��Need����
	for (i = 0; i < size; i++)
	{
		Need[i] = new int[ResourceKinds];
		for (int j = 0; j < ResourceKinds; j++)
		{
			Need[i][j] = Max[i][j] - Allocation[i][j];
		}
	}
	int index = GetListIndex(p, IndexList);
	//��������
	for (int i = 0; i < ResourceKinds; i++)
	{
		if (Request[i] > Need[index][i])
		{
			for (int i = 0; i < size; i++)
			{
				delete[] Max[i];
				delete[] Allocation[i];
				delete[] Need[i];
			}
			delete[] Max;
			delete[] Allocation;
			delete[] Need;
			return wrong;
		}
		if (Request[i] > Available[i])
		{
			for (int i = 0; i < size; i++)
			{
				delete[] Max[i];
				delete[] Allocation[i];
				delete[] Need[i];
			}
			delete[] Max;
			delete[] Allocation;
			delete[] Need;
			return ShouldBlock;
		}
	}
	//�����Դ
	for (int i = 0; i < ResourceKinds; i++)
	{
		Available[i] -= Request[i];
		Allocation[index][i] += Request[i];
		Need[index][i] -= Request[i];
	}
	//���γ��Է�����Դ
	int flag = this->Security_algo(p, Available, Need, Allocation,IndexList);
	//��ȫ���㷨
	if (ShouldBlock == flag)
	{
		for (int i = 0; i < ResourceKinds; i++)
		{
			Available[i] += Request[i];
			Allocation[index][i] -= Request[i];
			Need[index][i] += Request[i];
		}
		//�Ѹոշ������Դ����(ʵ�����ƺ�����Ҫ��һ��)
	}
	for (int i = 0; i < size; i++)
	{
		delete[] Max[i];
		delete[] Allocation[i];
		delete[] Need[i];
	}
	delete[] Max;
	delete[] Allocation;
	delete[] Need;
	//�ͷſռ�
	return flag;
}

//���ض����̴�ִ�ж���ת�����������
Status ProcessList::RunToReady(PCB* &p)
{
	for (auto i = this->m_RunL->begin(); i != this->m_RunL->end();)
	{
		if ((*i)->m_Id == p->m_Id)
		{
			this->m_ReadyL->push_back(p);
			//�ѽ��̼��뵽����������
			(*i)->m_state = PCB::State::W;
			//�޸Ľ���״̬
			this->m_RunL->erase(i);
			//�ѽ��̴�ִ�ж����г�ȥ
			return ok;
		}
		else i++;
	}
	return wrong;
}

//���ض����̴Ӿ�������ת��ִ�ж�����
Status ProcessList::ReadyToRun(PCB* &p, const ResourceType Request)
{
	if (this->m_RunL->size() != 0)
		return ok;
	for (int i = 0; i < ResourceKinds; i++)
	{
		p->m_hadR[i] += Request[i];
		//���̻����Դ
	}
	this->m_R.A_num -= Request[Resource::A];
	this->m_R.B_num -= Request[Resource::B];
	this->m_R.C_num -= Request[Resource::C];
	//�޸���Դ�ص���Դ��Ŀ
	p->m_state = PCB::State::R;
	//�޸Ľ���״̬
	this->m_RunL->push_back(p);
	//����ִ�ж�����
	for (auto i = this->m_ReadyL->begin(); i != this->m_ReadyL->end();)
	{
		if (p->m_Id == (*i)->m_Id)
		{
			i = this->m_ReadyL->erase(i);
			return ok;
		}
		else
		{
			i++;
		}
	}
	//ɾ���ھ������еĽ���
	return wrong;
}

//ʱ��Ƭ��ת�����㷨
Status ProcessList::RoundRobin(void)
{
	if (this->m_RunL->size() != 0)
	{
		auto i = this->m_RunL->begin();
		this->RunToReady(*i);
	}
	//�Ȱ�ִ�ж��е����н���ת�����������
	if (this->m_ReadyL->size() == 0)
		return ok;
	//�����������е�����
	static std::default_random_engine r;
	//���������
	ResourceType Request;
	//������Դ����
	for (int i = 0; i < ResourceKinds; i++)
	{
		Request[i] = r() %
			(this->m_ReadyL->front()->m_maxneedR[i] - this->m_ReadyL->front()->m_hadR[i] + 1);
	}
	//��ʼ����������
	std::cout << this->m_ReadyL->front()->m_Id << "����������Դ��" ;
	std::cout << "(A:" << Request[Resource::A] << "  B:" << Request[Resource::B]
		<< "  C:" << Request[Resource::C] << ")" << std::endl;
	//
	Status flag = this->Banker(this->m_ReadyL->front(),Request);
	//std::cout << "��־1";
    //���������еĶ��׽��� �������м��㷨
	//��ɣ�1.������Դ�������  2.��Դ����
	if (ok == flag)
	{
		this->ReadyToRun(this->m_ReadyL->front(), Request);
		return ok;
	}
	//���ڰ�ȫ���У���ô��������
	else if (ShouldBlock == flag)
	{
		this->ReadyToBlock(this->m_ReadyL->front());
		return ok;
	}
	//�����ڰ�ȫ���У���������
	else if (wrong == flag)
	{
		return wrong;
	}
}

//���̵��Ⱥ���
Status ProcessList::ProcessScheduing(void)
{
	using std::cout;
	using std::endl;
	//������ʶ��
	Status flag = ok , flag1;
	//ʱ�����
	unsigned int time_c = 0;
	//�������
	while (finish != flag and wrong != flag)
	{
		cout << "-----------------" << endl;
		cout << "��" << time_c << "����׼ʱ��" << endl;
		flag1 = this->CreatingProcess();
		//ÿ1����λʱ�䶼����һ������,����һ�������ɹ�
		//ʱ��ƬΪ2����λʱ��
		if (time_c % 2 == 0)
		{
			this->RoundRobin();
			//ʹ��ʱ��Ƭ��ת�㷨���Ƚ�������
		}
		this->CheckUpFinish();
		//ÿ����λʱ�䣬���ִ�н����Ƿ��Ѿ����
		time_c++;
        //��׼ʱ������
		cout << *this;
		//��ʾ������Ϣ
		if (this->m_BlockL->size() == 0 and this->m_ReadyL->size() == 0
			and this->m_RunL->size() == 0 and finish == flag1)
			//������������Ҵ������̷�����ɱ�־��Ϊ�գ�˵�����н��̶��������
		{
			flag = finish;
		}
	}
	return ok;
}

