#include"Experiment_3_header.h"
using namespace Dynamic_Partition;
//主页面
void main_menu()
{
	using std::cout;
	using std::endl;
	cout << "********************************" << endl
		<< "**                            **" << endl
		<< "** 1.动态分区-首次适应         **" << endl
		<< "** 2.基本分页-地址顺序(非紧凑)  **" << endl
		<< "** 3.基本分段-最佳适应         **" << endl
		<< "**                            **" << endl
		<< "**                            **" << endl
		<< "**                            **" << endl
		<< "********************************" << endl;
}


//操作符重载(显示空闲内存)
std::ostream& Dynamic_Partition::operator<<(std::ostream& os, IdleListType* ML)
{
	using std::cout;
	using std::endl;
	cout << "空闲内存：" << endl;
	for (auto i = ML->begin(); i != ML->end(); i++)
	{
		cout << "分区号：" << i->m_num << endl;
		cout << "分区大小：" << i->m_size << endl;
		cout << "分区始址：" << i->m_OgAdr << endl;
		if (i->m_state == IdleListElemType::State::F)
		{
			cout << "分区状态：F(空闲状态)" << endl;
		}
		cout << endl;
	}
	return os;
}

//操作符重载
std::ostream& Dynamic_Partition::operator<<(std::ostream& os, QueueType* RunQueue)
{
	using std::cout;
	using std::endl;
	cout << "分区使用情况：" << endl;
	for (auto i = RunQueue->begin(); i != RunQueue->end(); i++)
	{
		cout << "分区始址：" << (*i)->m_PartGetAdr << endl;
		cout << "分区大小：" << JobSize << endl;
		cout << "分区状态：O(占用状态)" << endl << endl;
	}
	return os;
}

//创建作业函数
Status JobScheduler::CreatingJob(void)
{
	if (m_JobList.size() >= JobMaxNum)
	{
		return finish;
	}
	//检查作业总数
	std::string temp_s;
	std::cout << "请输入创建的作业标识符：";
	std::cin >> temp_s;
	JCB temp_j = JCB(temp_s);
	//创建一个作业
	m_JobList.push_back(temp_j);
	//把创建好的作业加入到作业数组中
	m_BackupQueue->push_back(&m_JobList[m_JobList.size() - 1]);
	//把创建好的作业加入后备队列中
	return ok;
}

//执行时间增长函数
void JobScheduler::RunTimeIncreasing(void)
{
	for (auto i = m_RunQueue->begin(); i != m_RunQueue->end(); i++)
	{
		(*i)->m_ntime--;
	}
}

//初始化空闲分区表
Status JobScheduler::Init_ML(void)
{
	m_ML->resize(MemorySize / PartitionSize);
	for (int i = 0; i < MemorySize / PartitionSize; i++)
	{
		(*m_ML)[i].m_num = i + 1;
		//分区号初始化
		(*m_ML)[i].m_size = PartitionSize;
		//分区大小
		if (i == 0)
		{
			(*m_ML)[i].m_OgAdr = 0;
		}
		else
		{
			(*m_ML)[i].m_OgAdr = PartitionSize + (*m_ML)[i - 1].m_OgAdr;
		}
		//分区始址
		(*m_ML)[i].m_state = IdleListElemType::State::F;
		//分区状态
	}
	return ok;
}

//为作业J分配内存
Status JobScheduler::AllocateMemory(JCB* &j)
{
	for (auto i = m_ML->begin(); i != m_ML->end();)
	{
		if (i->m_size > JobSize)
		{
			j->m_PartGetAdr = i->m_OgAdr;
			//分配始址
			i->m_OgAdr += JobSize;
			//修改分区始址
			i->m_size -= JobSize;
			//修改分区大小
			return ok;
		}
		else if (i->m_size == JobSize)
		{
			j->m_PartGetAdr = i->m_OgAdr;
			//分配始址
			for (auto k = i + 1; k != m_ML->end(); k++)
			{
				k->m_num--;
			}
			//修改分区号
			i = m_ML->erase(i);
			//直接删除表项
			return ok;
		}
		else
			i++;
	}
	return wrong;
}

//把就绪作业转入执行状态
Status JobScheduler::ReadyToRun(JCB* &j)
{
	for (auto i = m_BackupQueue->begin(); i != m_BackupQueue->end();)
	{
		if (j->m_Id == (*i)->m_Id)
		{
			(*i)->m_state = JCB::State::R;
			//修改作业状态
			if (wrong == this->AllocateMemory(j))
			{
				return wrong;
			}
			//分配内存空间
			m_RunQueue->push_back(*i);
			//加入到执行队列中
			i = m_BackupQueue->erase(i);
			//从后备队列中移除
			return ok;
		}
		else
			i++;
	}
	return wrong;
}

//回收内存
Status JobScheduler::DeallocateMemory(JCB* &j)
{
	//回收区与插入点的前一个空闲分区相邻接
	for (auto i = 0; i < m_ML->size(); i++)
	{
		//找到插入点
		if (j->m_PartGetAdr > (*m_ML)[i].m_OgAdr and ((*m_ML)[i].m_OgAdr + (*m_ML)[i].m_size == j->m_PartGetAdr))
		{
			(*m_ML)[i].m_size += JobSize;
			j->m_PartGetAdr = -1;
			//修改作业j的内存信息
			return ok;
		}
	}
	
	//回收区与插入点的后一空闲分区相邻接
	for (auto i = 0; i < m_ML->size(); i++)
	{
		if (j->m_PartGetAdr < (*m_ML)[i].m_OgAdr and (j->m_PartGetAdr + JobSize == (*m_ML)[i].m_OgAdr))
		{
			(*m_ML)[i].m_OgAdr = j->m_PartGetAdr;
			(*m_ML)[i].m_size += JobSize;
			j->m_PartGetAdr = -1;
			//修改作业j的内存信息
			return ok;
		}
	}

	//回收区都不相邻接
	for (auto i = m_ML->begin(); i != m_ML->end() - 1; i++)
	{
		//Bug:没有考虑在队尾的情况
		//考虑队首插入的情况
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
		//考虑在队中插入的情况
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

//把执行作业转入就绪状态
Status JobScheduler::RunToReady(JCB* &j)
{
	for (auto i = m_RunQueue->begin(); i != m_RunQueue->end();)
	{
		if ((*i)->m_Id == j->m_Id)
		{
			(*i)->m_state = JCB::State::W;
			//修改作业状态
			this->DeallocateMemory(j);
			//回收内存
			m_BackupQueue->push_back(*i);
			//加入后备队列
			i = m_RunQueue->erase(i);
			//删除执行进程
			return ok;
		}
		else
			i++;
	}
	return wrong;
}

//检查作业是否完成
Status JobScheduler::TurnToFinish(void)
{
	for (auto i = this->m_RunQueue->begin(); i != this->m_RunQueue->end();)
	{
		if ((*i)->IfFinish())
		{
			if (wrong == this->DeallocateMemory(*i))
				return wrong;
			//回收内存
			(*i)->m_state = JCB::State::F;
			//修改作业状态
			i = this->m_RunQueue->erase(i);
		}
		else
			i++;
	}
	return ok;
}

//时间片轮转调度-动态分区
Status Dynamic_Partition::Round_Robin_DM(JobScheduler* &J)
{
	for (auto i = J->m_RunQueue->begin(); i != J->m_RunQueue->end(); i = J->m_RunQueue->begin())
	{
		J->RunToReady(*i);
		//把执行进程转入就绪状态
		//回收内存
	}
	int num = 0;
	for (auto i = J->m_BackupQueue->begin(); i != J->m_BackupQueue->end() and num < ProcessorNum;
		 i = J->m_BackupQueue->begin(),num++)
	{
		J->ReadyToRun(*i);
		//把就绪进程转入执行状态
		//同时分配内存
	}
	return ok;
}

//作业调度函数
void  Dynamic_Partition::JobScheduling_DM(JobScheduler* &J)
{
	unsigned int time_c = 0;
	//设置标准时间
	int flag = ok , flag_cr;
	J->Init_ML();
	//初始化分区空闲表
	while (finish != flag and wrong != flag)
	{
		std::cout << "--------------" << std::endl;
		std::cout << "第" << time_c << "个标准时间" << std::endl;
		flag_cr = J->CreatingJob();
		//每个单位时间创建一个作业
		if (time_c % 2 == 0)
			//时间片为2个单位时间
		{
			
			Round_Robin_DM(J);
			//时间片轮转算法
			//分配内存空间
		}
		J->RunTimeIncreasing();
		//执行队列的时间增长函数
		J->TurnToFinish();
		//检查作业是否完成；若完成，把作业转入完成状态
		std::cout << J->m_ML;
		std::cout << J->m_RunQueue;
		//显示内存状态图
		time_c++;
		//标准时间增长
		if (finish == flag_cr and J->m_BackupQueue->size() == 0 and J->m_RunQueue->size() == 0)
		{
			flag = finish;
		}
		//检查作业调度是否已经完成
	}
	
}
