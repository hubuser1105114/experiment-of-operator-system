#include"Project_2_header.h"
//判断进程是否达到完成条件
Status PCB::IfFinish(void)const
{
	for (auto i = 0; i < ResourceKinds; i++)
	{
		if (this->m_hadR[i] < this->m_maxneedR[i])
			return false;
	}
	//只要有一种资源没有达到最大需求，就算不完成
	return ok;
}

//<<运算符重载 ，显示进程表的信息
std::ostream& operator<<(std::ostream& os, ProcessList& P)
{
	using std::cout;
	using std::endl;
	cout << "系统中的资源可用数：";
	cout << "(A:" << P.m_R.A_num << "  B:" << P.m_R.B_num
		<< "  C:" << P.m_R.C_num << ")" << endl;
	cout << "正在运行的进程(以标识符显示)：";
	for (auto i = (P.m_RunL)->begin(); i != (P.m_RunL)->end(); i++)
	{
		cout << (*i)->m_Id << " ";
	}
	cout << endl;
	cout << "就绪队列中的进程(以标识符显示)：";
	for (auto i = (P.m_ReadyL)->begin(); i != (P.m_ReadyL)->end(); i++)
	{
		cout << (*i)->m_Id << " ";
	}
	cout << endl;
	cout << "阻塞队列中的进程(以标识符显示)：";
	for (auto i = (P.m_BlockL)->begin(); i != (P.m_BlockL)->end(); i++)
	{
		cout << (*i)->m_Id << " ";
	}
	cout << endl;
	cout << "完成的进程(以标识符显示)：";
	for (auto i = P.m_PCBList.begin(); i != P.m_PCBList.end(); i++)
	{
		if (i->m_state == PCB::State::F)
		{
			cout << i->m_Id << " ";
		}
	}
	cout << endl;
	cout << "所有进程的PCB信息：" << endl;
	for (auto i = P.m_PCBList.begin(); i != P.m_PCBList.end(); i++)
	{
		cout << "进程标识符：" << i->m_Id << endl;
		cout << "进程状态：";
		if (i->m_state == PCB::State::B)
		{
			cout << "B(阻塞状态)";
		}
		else if (i->m_state == PCB::State::F)
		{
			cout << "F(完成状态)";
		}
		else if (i->m_state == PCB::State::R)
		{
			cout << "R(运行状态)";
		}
		else if (i->m_state == PCB::State::W)
		{
			cout << "W(就绪状态)";
		}
		cout << endl;
		cout << "最大需要的资源总数：" << endl;
		cout << "A:" << i->m_maxneedR[Resource::A] << "  B:" << i->m_maxneedR[Resource::B]
			 << "  C:" << i->m_maxneedR[Resource::C] << endl;
		cout << "已经获得的资源总数：" << endl;
		cout << "A:" << i->m_hadR[Resource::A] << "  B:" << i->m_hadR[Resource::B]
			<< "  C:" << i->m_hadR[Resource::C] << endl << endl;
	}
	return os;
}

//创建一个进程
Status ProcessList::CreatingProcess(void)
{
	if (this->m_PCBList.size() >= ProcessMaxNum)
		return finish;
	//检查是否已达到进程最大数
	if (this->m_BlockL->size() + this->m_ReadyL->size() + this->m_RunL->size() >= ConcurrencyNum)
		return ok;
	//检查是否达到最大并行数
	    std::string temp_s;
		std::cout << "请输入创建进程的标识符：";
		std::cin >> temp_s;
		PCB temp_p = PCB(temp_s);
		//创建一个新的PCB进程
		this->m_PCBList.push_back(temp_p);
		//把新的进程装入PCB列表中
		this->m_ReadyL->push_back(&this->m_PCBList[this->m_PCBList.size() - 1]);
		//把新的进程装入就绪队列中
	    return ok;
}

//把特定进程从就绪状态转入阻塞状态
Status ProcessList::ReadyToBlock(const PCB* p)
{
	for (auto i = this->m_ReadyL->begin(); i != this->m_ReadyL->end();)
	{
		if ((*i)->m_Id == p->m_Id)
		{
			(*i)->m_state = PCB::State::B;
			//修改进程状态
			this->m_BlockL->push_back(*i);
			//加入p进程到阻塞队列中
			i = this->m_ReadyL->erase(i);
			//在就绪队列中删去p进程
			return ok;
		}
		else
			i++;
	}
	return wrong;
}

//检查执行进程是否达到完成标志
Status ProcessList::CheckUpFinish(void)
{
	if (0 == this->m_RunL->size() and 0 == this->m_ReadyL->size() and 0 != this->m_BlockL->size())
	{
		this->WakeUpBlock();
	}
	if (0 == this->m_RunL->size())
		//如果执行队列中没有进程，那么无事发生
		return ok;
	if (1 == this->m_RunL->size() and this->m_RunL->front()->IfFinish() == ok)
		//如果进程达到完成条件
	{
		PCB* front = this->m_RunL->front();
		this->m_R.A_num += front->m_hadR[Resource::A];
		this->m_R.B_num += front->m_hadR[Resource::B];
		this->m_R.C_num += front->m_hadR[Resource::C];
		//归还资源
		front->m_hadR[Resource::A] = front->m_hadR[Resource::B]
			= front->m_hadR[Resource::C] = 0;
		//修改已有资源数
		front->m_state = PCB::State::F;
		//修改状态
		this->m_RunL->pop_front();
		//删除执行队列的队首进程
		this->CreatingProcess();
		//完成一个进程，就创建一个进程
		this->WakeUpBlock();
		//完成一个进程，唤醒相应的阻塞进程
	}
	return ok;
}

//把阻塞队列的队首进程转入到就绪队列中
Status ProcessList::WakeUpBlock(void)
{
	if (this->m_BlockL->size() == 0)
		return ok;
	//进程数为0，那么无事发生
	auto i = this->m_BlockL->begin();
	//取队首进程
	(*i)->m_state = PCB::State::W;
	//修改状态
	this->m_ReadyL->push_back(*i);
	//加入就绪队列
	this->m_BlockL->pop_front();
	//删除队首元素
	return ok;
}

//安全性算法
Status ProcessList::Security_algo(PCB* & p,const ResourceType& Available, int** Need, int** Allocation, std::vector<PCB*>& IndexList)
{
	int index = GetListIndex(p, IndexList);
	//得到进程索引
	int size = IndexList.size();
	//得到进程集合的大小
	//创建2个辅助的数据结构
	//工作向量Work
	ResourceType Work = Available;
	//完成向量
	bool* Finish = new bool[size] {false};
	int j = 0;
	for (auto i = IndexList.begin(); i != IndexList.end(); i++,j++)
	{
		if (Finish[j] == false)
		{
			bool sign = true;
			//设立标志
			for (int k = 0; k < ResourceKinds; k++)
			{
				if (Need[j][k] > Work[k])
				{
					sign = false;
					break;
				}
			}
			//检查满足条件的进程
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

//查找索引
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

//银行家算法,进程申请资源
Status ProcessList::Banker(PCB*  &p, ResourceType& Request)
{
	std::vector<PCB*> IndexList;
	//索引数组
	for (auto i = this->m_ReadyL->begin(); i != this->m_ReadyL->end(); i++)
	{
		IndexList.push_back(*i);
	}
	for (auto i = this->m_BlockL->begin(); i != this->m_BlockL->end(); i++)
	{
		IndexList.push_back(*i);
	}
	//初始化索引数组
	//先创建4个数据结构
	//可利用资源向量Available
	ResourceType Available;
	//初始化Available
	Available[Resource::A] = this->m_R.A_num;
	Available[Resource::B] = this->m_R.B_num;
	Available[Resource::C] = this->m_R.C_num;
	int size = IndexList.size();
	//最大需求矩阵Max
	int** Max = new int*[size];
	//初始化Max
	int i = 0;
	for (auto j = IndexList.begin();j != IndexList.end();j++,i++)
	{
		Max[i] = new int[ResourceKinds];
		Max[i][Resource::A] = (*j)->m_maxneedR[Resource::A];
		Max[i][Resource::B] = (*j)->m_maxneedR[Resource::B];
		Max[i][Resource::C] = (*j)->m_maxneedR[Resource::C];
	}
	//分配矩阵Allocation
	int** Allocation = new int*[size];
	//初始化Allocation
	i = 0;
	for (auto j = IndexList.begin();j != IndexList.end();j++,i++)
	{
		Allocation[i] = new int[ResourceKinds];
		Allocation[i][Resource::A] = (*j)->m_hadR[Resource::A];
		Allocation[i][Resource::B] = (*j)->m_hadR[Resource::B];
		Allocation[i][Resource::C] = (*j)->m_hadR[Resource::C];
	}
	//需求矩阵Need
	int** Need = new int*[size];
	//初始化Need矩阵
	for (i = 0; i < size; i++)
	{
		Need[i] = new int[ResourceKinds];
		for (int j = 0; j < ResourceKinds; j++)
		{
			Need[i][j] = Max[i][j] - Allocation[i][j];
		}
	}
	int index = GetListIndex(p, IndexList);
	//查找索引
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
	//检查资源
	for (int i = 0; i < ResourceKinds; i++)
	{
		Available[i] -= Request[i];
		Allocation[index][i] += Request[i];
		Need[index][i] -= Request[i];
	}
	//初次尝试分配资源
	int flag = this->Security_algo(p, Available, Need, Allocation,IndexList);
	//安全性算法
	if (ShouldBlock == flag)
	{
		for (int i = 0; i < ResourceKinds; i++)
		{
			Available[i] += Request[i];
			Allocation[index][i] -= Request[i];
			Need[index][i] += Request[i];
		}
		//把刚刚分配的资源作废(实际上似乎不需要这一步)
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
	//释放空间
	return flag;
}

//把特定进程从执行队列转入就绪队列中
Status ProcessList::RunToReady(PCB* &p)
{
	for (auto i = this->m_RunL->begin(); i != this->m_RunL->end();)
	{
		if ((*i)->m_Id == p->m_Id)
		{
			this->m_ReadyL->push_back(p);
			//把进程加入到就绪队列中
			(*i)->m_state = PCB::State::W;
			//修改进程状态
			this->m_RunL->erase(i);
			//把进程从执行队列中除去
			return ok;
		}
		else i++;
	}
	return wrong;
}

//把特定进程从就绪队列转入执行队列中
Status ProcessList::ReadyToRun(PCB* &p, const ResourceType Request)
{
	if (this->m_RunL->size() != 0)
		return ok;
	for (int i = 0; i < ResourceKinds; i++)
	{
		p->m_hadR[i] += Request[i];
		//进程获得资源
	}
	this->m_R.A_num -= Request[Resource::A];
	this->m_R.B_num -= Request[Resource::B];
	this->m_R.C_num -= Request[Resource::C];
	//修改资源池的资源数目
	p->m_state = PCB::State::R;
	//修改进程状态
	this->m_RunL->push_back(p);
	//加入执行队列中
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
	//删除在就绪队列的进程
	return wrong;
}

//时间片轮转调度算法
Status ProcessList::RoundRobin(void)
{
	if (this->m_RunL->size() != 0)
	{
		auto i = this->m_RunL->begin();
		this->RunToReady(*i);
	}
	//先把执行队列的所有进程转入就绪队列中
	if (this->m_ReadyL->size() == 0)
		return ok;
	//检查就绪队列中的数量
	static std::default_random_engine r;
	//随机数引擎
	ResourceType Request;
	//请求资源向量
	for (int i = 0; i < ResourceKinds; i++)
	{
		Request[i] = r() %
			(this->m_ReadyL->front()->m_maxneedR[i] - this->m_ReadyL->front()->m_hadR[i] + 1);
	}
	//初始化请求向量
	std::cout << this->m_ReadyL->front()->m_Id << "进程请求资源：" ;
	std::cout << "(A:" << Request[Resource::A] << "  B:" << Request[Resource::B]
		<< "  C:" << Request[Resource::C] << ")" << std::endl;
	//
	Status flag = this->Banker(this->m_ReadyL->front(),Request);
	//std::cout << "标志1";
    //将就绪队列的队首进程 进行银行家算法
	//完成：1.进程资源随机申请  2.资源分配
	if (ok == flag)
	{
		this->ReadyToRun(this->m_ReadyL->front(), Request);
		return ok;
	}
	//存在安全序列，那么让它运行
	else if (ShouldBlock == flag)
	{
		this->ReadyToBlock(this->m_ReadyL->front());
		return ok;
	}
	//不存在安全序列，把它阻塞
	else if (wrong == flag)
	{
		return wrong;
	}
}

//进程调度函数
Status ProcessList::ProcessScheduing(void)
{
	using std::cout;
	using std::endl;
	//设立标识符
	Status flag = ok , flag1;
	//时间变量
	unsigned int time_c = 0;
	//处理进程
	while (finish != flag and wrong != flag)
	{
		cout << "-----------------" << endl;
		cout << "第" << time_c << "个标准时间" << endl;
		flag1 = this->CreatingProcess();
		//每1个单位时间都创建一个进程,但不一定创建成功
		//时间片为2个单位时间
		if (time_c % 2 == 0)
		{
			this->RoundRobin();
			//使用时间片轮转算法调度进程运行
		}
		this->CheckUpFinish();
		//每个单位时间，检查执行进程是否已经完成
		time_c++;
        //标准时间增长
		cout << *this;
		//显示进程信息
		if (this->m_BlockL->size() == 0 and this->m_ReadyL->size() == 0
			and this->m_RunL->size() == 0 and finish == flag1)
			//如果三个队列且创建进程返回完成标志都为空，说明所有进程都处理完毕
		{
			flag = finish;
		}
	}
	return ok;
}

