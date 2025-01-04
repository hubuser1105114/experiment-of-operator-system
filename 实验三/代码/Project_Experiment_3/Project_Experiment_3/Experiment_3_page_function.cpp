#include"Experiment_3_header.h"
using namespace Paging;

//创建作业，并加入到后备队列
Status JobScheduler::JobCreating(void)
{
	if (m_JobList.size() >= JobMaxNum)
		return finish;
	std::string temp_s;
	std::cout << "请输入创建的作业的标识符：";
	std::cin >> temp_s;
	//创建作业标识符
	JCB temp_j = JCB(temp_s);
	m_JobList.push_back(temp_j);
	//加入到后备队列
	m_BackUpQueue->push_back(&m_JobList[m_JobList.size() - 1]);
	//加入到Job数组中
	return ok;
}

//回收内存
Status JobScheduler::DeAllocateMemory(JCB* &j)
{
	for (auto i = m_RunQueue->begin(); i != m_RunQueue->end();i++)
	{
		if (j->m_Id == (*i)->m_Id)
		{
			for (int k = 0; k < j->m_pt.second; k++)
			{
				int SectionNo = (*(j->m_pt.first))[k];
				//得到段号
				if (-1 != SectionNo)
				{
					(*m_MS)[SectionNo] = Free;
					//修改内存状态
				}
				(*(j->m_pt.first))[k] = -1;
				//归为-1
			}
			return ok;
		}
	}
	return wrong;
}

//把j作业从执行状态转为就绪状态
Status JobScheduler::RunToReady(JCB* &j)
{
	for (auto i = m_RunQueue->begin(); i != m_RunQueue->end();)
	{
		if (j->m_Id == (*i)->m_Id)
		{
			if (wrong == this->DeAllocateMemory(*i))
				return wrong;
			j->m_state = JCB::State::W;
			//状态
			m_BackUpQueue->push_back(*i);
			i = m_RunQueue->erase(i);
			return ok;
		}
		else
			i++;
	}
	return wrong;
}

//为作业j分配内存
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
			//相对的页号得到块号
			(*m_MS)[i] = Busy;
			//修改块号状态
			k++;
		}
	}
	if (i == pagenum and k != (j->m_pt.first)->end())
	{
		return wrong;
	}
	//内存不足
	return ok;
}

//把特定作业从就绪状态转入执行状态
Status JobScheduler::ReadyToRun(JCB* &j)
{
	for (auto i = m_BackUpQueue->begin(); i != m_BackUpQueue->end();)
	{
		if (j->m_Id == (*i)->m_Id)
		{
			if (wrong == AllocateMemory(*i))
				return wrong;
		    //分配内存
			j->m_state = JCB::State::R;
			//作业状态
			m_RunQueue->push_back(*i);
			//入队执行队列
			i = m_BackUpQueue->erase(i);
			//出队后备队列
			return ok;
		}
		else
			i++;
	}
	return wrong;
}

//时间片调度算法-基本分页管理
Status Paging::RobinRound_Paging(JobScheduler* &J)
{
	for (auto i = J->m_RunQueue->begin(); i != J->m_RunQueue->end();i = J->m_RunQueue->begin())
	{
		//把作业从执行状态转入就绪状态
		if (ok != J->RunToReady(*i))
			return wrong;
	}
	int num = 0;
	for (auto i = J->m_BackUpQueue->begin(); i != J->m_BackUpQueue->end() and num < ProcessorNum;
		i = J->m_BackUpQueue->begin(),num++)
	{
		//把前五个作业从就绪状态转入执行状态
		if (ok != J->ReadyToRun(*i))
			return wrong;
	}
	return ok;
}

//运行时间增长
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

//把完成的作业转入完成状态
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

//<<重载
std::ostream& Paging::operator<<(std::ostream& os, Paging::JobScheduler* &J)
{
	using std::cout;
	using std::endl;
	int size = JobSize / PageSize;
	cout << "内存使用情况：" << endl;
	for (auto i = J->m_RunQueue->begin(); i != J->m_RunQueue->end(); i++)
	{
		cout << "作业标识符：" << (*i)->m_Id << endl
			 << "作业页表：" << endl;
		for (int j = 0; j < size; j++)
		{
			cout << "页号" << j << "————" << "段号" << (*((*i)->m_pt.first))[j] << endl;
		}
	}
	cout << "空闲内存：" << endl;
	for (int i = 0;i < J->m_MS->size();i++)
	{
		if ((*(J->m_MS))[i] == Free)
		{
			cout << "段号" << i << endl;
		}
	}
	return os;
}

//作业调度(基本分页管理)
Status Paging::JobScheduling_Paging(JobScheduler* &J)
{
	//定义时间变量
	int time_c = 0;
	//定义标志
	int flag = ok, flag_c;
	while (finish != flag and wrong != flag)
	{
		std::cout << "-----------------" << std::endl
			<< "第" << time_c << "个单位时间" << std::endl;
		//每个单位时间创建一个作业
		flag_c = J->JobCreating();
		//每2个单位时间为一个时间片
		if (time_c % 2 == 0)
		{
			if (ok != RobinRound_Paging(J))
				return wrong;
		}
		//运行时间减少
		J->RunTimeIncrease();
		//显示内存使用情况
		std::cout << J;
		//标准时间增长
		time_c++;
		//检查是否作业完成
		J->FindHasFinish();
		//检查调度是否完成
		if (finish == flag_c and J->m_RunQueue->size() == 0 and J->m_BackUpQueue->size() == 0)
			flag = finish;
	}
}

