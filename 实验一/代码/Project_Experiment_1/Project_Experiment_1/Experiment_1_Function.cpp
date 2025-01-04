#include"Experiment_1_header.h"

//入队一个进程在就绪队列中
Status ReadyQueue::AddRdProcess(PCB* p)
{
	if (this->m_ReadyList.size() >= ProcessMaxNum)return finish;
	//检查就绪队列中的进程
	this->m_ReadyList.push_back(p);
	//默认先来先服务(FCFS)算法
	p->m_state = PCB::State::W;
	//修改进程状态为就绪态
	return ok;
}

//创建一个新进程,并插入在就绪队列里
Status ProcessList::CreateNewProcess(void)
{
	if (this->m_PCBList.size() >= ProcessMaxNum)return finish;
	//如果创建过的进程数多于进程最大数，返回完成标志
	PCB* temp_p = nullptr;
	std::string temp_s;
	int temp_int;
	std::cout << "请输入新进程的标识符：" << std::endl;
	std::cin >> temp_s;
	std::cout << "请输入新进程的要求服务时间：" << std::endl;
	std::cin >> temp_int;
	temp_p = new PCB({ temp_s,temp_int });
	//创建一个带有用户自己输入的标识符的进程
	if (nullptr == temp_p)return wrong;
	//内存不足，返回wrong
	this->m_PCBList.push_back(*temp_p);
	//把新创建的进程塞入PCB列表中
	PCB* temp_p2 = &(this->m_PCBList[this->m_PCBList.size() - 1]);
	//取PCB列表中新进程的地址
	delete temp_p;
	if (ok == this->m_Readyp->AddRdProcess(temp_p2))
	{
		return ok;
	}
	//把新创建的进程加入就绪队列中
	else
	{
		return wrong;
	}
}

//把特定进程加入到执行队列里
Status RunQueue::AddRunProcess(PCB* p)
{
	if (this->m_RunList.size() >= ProcessorNum)return finish;
	//最多允许5个进程运行
	this->m_RunList.push_back(p);
	//把进程加入在执行索引表中
	p->m_state = PCB::State::R;
	//修改进程状态标志
	p->m_wtime = 0;
	//把进程的等待时间置0
	return ok;
}

//把特定进程从执行队列中移去，S为进程的后续状态
Status RunQueue::DeleteRunProcess(PCB* p, PCB::State S)
{
	std::string id = p->m_Id;
	for (auto i = this->m_RunList.begin(); i != this->m_RunList.end();)
	{
		if (id == (*i)->m_Id)
		{
			p->m_state = S;
			//修改状态
			i = this->m_RunList.erase(i);
			//从列表中移去特定进程
			return ok;
		}
		else
			i++;
	}
	return wrong;
	//未找到，返回wrong
}

//检查执行索引表，把所有已运行完的进程移除
Status RunQueue::TurnProcessFinish(void)
{
	for (auto i = this->m_RunList.begin(); i != this->m_RunList.end();)
	{
		if (true == (*i)->IfProcessDone())
			//进程已完成执行
		{
			//修改进程状态
			(*i)->m_state = PCB::State::F;
			//移除此进程
			i = this->m_RunList.erase(i);
		}
		else
			i++;
	}
	return ok;
}

//在就绪队列中出队一个进程
Status ReadyQueue::DeleteRdProcess(PCB* p)
{
	std::string id = p->m_Id;
	for (auto i = this->m_ReadyList.begin(); i != this->m_ReadyList.end();)
	{
		if (id == (*i)->m_Id)
		{
			i = this->m_ReadyList.erase(i);
			//移去特定进程
			return ok;
		}
		else
			i++;
	}
	return wrong;
	//未找到进程
}

//构造函数定义，需要给予执行索引表和就绪索引表内存空间
ProcessList::ProcessList(void)
{
	this->m_Runp = new RunQueue;
	this->m_Readyp = new ReadyQueue;
	this->m_PCBList.reserve(ProcessMaxNum);
	//为PCB列表预留足够的空间
}

//析构函数，需要释放内存空间
ProcessList::~ProcessList(void)
{
	delete this->m_Runp;
	delete this->m_Readyp;
}

//将特定的进程从就绪状态转入执行状态
Status ProcessList::RunReadyProcess(PCB* p)
{
	if (ok != this->m_Readyp->DeleteRdProcess(p))
	{
		return wrong;
	}
	//把特定进程从就绪队列中出队
	Status flag = this->m_Runp->AddRunProcess(p);
	if(ok != flag and finish != flag)
	{
		return wrong;
	}
	else if (finish == flag)
	{
		return finish;
	}
	//把特定进程入队执行队列中
	return ok;
}

//将特定进程从执行队列中转入就绪队列(即发生了中断)
Status ProcessList::DiscontinueProcess(PCB* p)
{
	if (ok != this->m_Runp->DeleteRunProcess(p, PCB::State::W))
	{
		return wrong;
	}
	//把进程从执行队列中移除
	if (ok != this->m_Readyp->AddRdProcess(p))
	{
		return wrong;
	}
	//把进程入队就绪队列
	return ok;
}

//进程表显示
std::ostream& operator<<(std::ostream& os, const ProcessList& P)
{
	using std::cout;
	using std::endl;
	cout << "正在运行的进程(以标识符显示进程)：";
	for (auto i = (P.m_Runp)->m_RunList.begin(); i != (P.m_Runp)->m_RunList.end(); i++)
	{
		cout << (*i)->m_Id << " ";
	}
	cout << endl;
	//显示正在运行的进程
	cout << "所有就绪的进程(以标识符显示进程)：";
	for (auto i = (P.m_Readyp)->m_ReadyList.begin(); i != (P.m_Readyp)->m_ReadyList.end(); i++)
	{
		cout << (*i)->m_Id << " ";
	}
	cout << endl;
	//显示所有就绪进程
	cout << "所有完成的进程(以标识符显示进程)：";
	for (auto i = (P.m_PCBList).begin(); i != (P.m_PCBList).end(); i++)
	{
		if (i->m_state == PCB::State::F)
		{
			cout << i->m_Id << " ";
		}
		//判断进程标志
	}
	cout << endl;
	//显示完成的进程
	cout << "所有进程的PCB信息：" << endl;
	for (auto i = P.m_PCBList.begin(); i != P.m_PCBList.end(); i++)
	{
		cout << "进程标识符：" << i->m_Id << endl;
		if (i->m_state == PCB::State::F)
		{
			cout << "进程状态：F(完成状态)" << endl;
		}
		else if (i->m_state == PCB::State::R)
		{
			cout << "进程状态：R(执行状态)" << endl;
		}
		else if (i->m_state == PCB::State::W)
		{
			cout << "进程状态：W(就绪状态)" << endl;
		}
		cout << "进程要求服务时间：" << i->m_ntime << endl;
		cout << "进程已经获得服务时间：" << i->m_rtime << endl;
		cout << "进程已等待时间：" << i->m_wtime << endl;
		cout << "进程周转时间：" << i->turnaround_time << endl;
		cout << endl;
	}
	//显示所有进程的PCB信息
	return os;
}

//非抢占方式的短进程优先算法
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
	//返回就绪索引表中要求服务时间最短的那个进程；若没有，返回nullptr
}

//非抢占方式的高响应比优先调度算法
PCB* Non_HRRN(ReadyQueue* &Rq)
{
	float max = 0;
	PCB* max_p = nullptr;
	for (auto i = Rq->m_ReadyList.begin(); i != Rq->m_ReadyList.end(); i++)
	{
		float rp = ((*i)->m_wtime + (*i)->m_ntime) / (*i)->m_ntime;
		//计算高响应比优先级,rp越大，优先级越高
		if (max < rp)
		{
			max = rp;
			max_p = *i;
		}
	}
	return max_p;
	//返回就绪队列中优先级最高的进程。若没有，那么返回nullptr

}

//抢占式的短进程优先算法
Status Pre_ShortJobFirst(ProcessList* &P)
{
	//如果处理机未全部忙状态，则先调度进程使处理机全部忙
	if (true != P->m_Runp->FullRunQueue())
	{
		for (auto i = P->m_Readyp->m_ReadyList.begin(); i != P->m_Readyp->m_ReadyList.end() 
			 and true != P->m_Runp->FullRunQueue(); i = P->m_Readyp->m_ReadyList.begin())
		{
			if (ok != P->RunReadyProcess(*i))
				return wrong;
		}
	}
	//谁时间少，谁直接进入执行队列中
	for (auto i = P->m_Readyp->m_ReadyList.begin(); i != P->m_Readyp->m_ReadyList.end(); i++)
		//查看就绪队列中的每一个进程，与执行队列中的每个进程相比较
	{
		for (auto j = P->m_Runp->m_RunList.begin(); j != P->m_Runp->m_RunList.end(); j++)
		{
			if ((*i)->m_ntime - (*i)->m_rtime < (*j)->m_ntime - (*j)->m_rtime)
				//如果就绪队列的进程小于执行队列尚需运行的时间
			{
				//修改就绪队列的进程状态
				(*i)->m_state = PCB::State::R;
				//修改执行队列的进程状态
				(*j)->m_state = PCB::State::W;
				//等待时间归0
				(*i)->m_wtime = 0;
				//只需把两个进程的地址调换即可
				PCB* temp = *i;
				*i = *j;
				*j = temp;
			}
		}
	}
	return ok;
}

//抢占式的高响应比优先调度算法
Status Pre_HRRN(ProcessList* &P)
{
	//如果处理机未全部忙状态，则先调度进程使处理机全部忙
	if (true != P->m_Runp->FullRunQueue())
	{
		for (auto i = P->m_Readyp->m_ReadyList.begin(); i != P->m_Readyp->m_ReadyList.end()
			and true != P->m_Runp->FullRunQueue();i = P->m_Readyp->m_ReadyList.begin())
		{
			if (ok != P->RunReadyProcess(*i))
				return wrong;
		}
	}
	//谁优先级高，谁直接进入执行队列中
	for (auto i = P->m_Readyp->m_ReadyList.begin(); i != P->m_Readyp->m_ReadyList.end(); i++)
		//查看就绪队列中的每一个进程，与执行队列中的每个进程相比较
	{
		//计算i进程的优先权
		float super_i = ((*i)->m_wtime + (*i)->m_ntime) / (*i)->m_ntime;
		for (auto j = P->m_Runp->m_RunList.begin(); j != P->m_Runp->m_RunList.end(); j++)
		{
			//计算j进程的优先权
			float super_j = ((*j)->m_wtime + (*j)->m_ntime) / (*j)->m_ntime;
			if (super_i > super_j)
				//如果就绪队列的进程的优先权大于执行队列的进程的优先权
			{
				//修改就绪队列的进程状态
				(*i)->m_state = PCB::State::R;
				//修改执行队列的进程状态
				(*j)->m_state = PCB::State::W;
				//等待时间归0
				(*i)->m_wtime = 0;
				//只需把两个进程的地址调换即可
				PCB* temp = *i;
				*i = *j;
				*j = temp;
			}
		}
	}
	return ok;
}

//时间片轮转算法
Status TimePieceRR(ProcessList* &P)
{
	//规定分给每一个就绪队列的进程的CPU处理时间为2个单位时间
	//根据FCFS策略
	//利用erase和push_back来进行就绪队列的调整
			//把进程中断转入就绪队列中
	for (auto i = P->m_Runp->m_RunList.begin(); i != P->m_Runp->m_RunList.end()
		; i = P->m_Runp->m_RunList.begin())
	{
		//删去执行队列的队首进程
		P->DiscontinueProcess(*i);
	}
    //循环ProcessorMaxNum次
		//把进程按时间片轮转算法调入执行队列中
		for (int num = 0; num < ProcessorNum and
			P->m_Readyp->m_ReadyList.begin() != P->m_Readyp->m_ReadyList.end(); num++)
		{
			//就绪队列的列首进程加入执行队列中
			P->RunReadyProcess(*(P->m_Readyp->m_ReadyList.begin()));
		}
		return ok;
}

//等待时间增长函数
void ReadyQueue::WaitTimeAdd(void)
{
	for (auto i = this->m_ReadyList.begin(); i != this->m_ReadyList.end(); i++)
	{
		(*i)->m_wtime++;
	}
}

//执行时间增长函数
void RunQueue::RunTimeAdd(void)
{
	for (auto i = this->m_RunList.begin(); i != this->m_RunList.end(); i++)
	{
		(*i)->m_rtime++;
	}
}

//周转时间增长函数
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

//计算平均周转时间
float ProcessList::cal_turnaroundtime(void)const
{
	float sum = 0;
	for (auto i = this->m_PCBList.begin(); i != this->m_PCBList.end(); i++)
	{
		sum += i->turnaround_time;
	}
	return sum / this->m_PCBList.size();
}

//计算带权平均周转时间
float ProcessList::cal_w_turnaroundtime(void)const
{
	float sum = 0;
	for (auto i = this->m_PCBList.begin(); i != this->m_PCBList.end(); i++)
	{
		sum += (float(i->turnaround_time) / float(i->m_ntime));
	}
	return sum / ProcessMaxNum;
}

//页面函数
void main_menu(void)
{
	using std::cout;
	using std::endl;
	cout << "**************************************" << endl;
	cout << "**************************************" << endl;
	cout << "**              进程调度             **" << endl;
	cout << "**                                   **" << endl;
	cout << "**                                   **" << endl;
	cout << "**  1.非抢占方式         2.抢占方式   **" << endl;
	cout << "**                                   **" << endl;
	cout << "**                                   **" << endl;
	cout << "**                                   **" << endl;
	cout << "**                                   **" << endl;
	cout << "**************************************" << endl;
	cout << "**************************************" << endl;
}

//非抢占方式调度页面
void np_menu(void)
{
	using std::cout;
	using std::endl;
	cout << "**************************************" << endl;
	cout << "**************************************" << endl;
	cout << "**        非抢占方式-进程调度         **" << endl;
	cout << "**                                   **" << endl;
	cout << "**                                   **" << endl;
	cout << "**                                   **" << endl;
	cout << "**        1.短进程优先调度            **" << endl;
	cout << "**        2.高响应比优先调度           **" << endl;
	cout << "**                                   **" << endl;
	cout << "**                                   **" << endl;
	cout << "**                                   **" << endl;
	cout << "**************************************" << endl;
	cout << "**************************************" << endl;


}

//抢占方式调度页面
void p_menu(void)
{
	using std::cout;
	using std::endl;
	cout << "**************************************" << endl;
	cout << "**************************************" << endl;
	cout << "**        抢占方式-进程调度          **" << endl;
	cout << "**                                   **" << endl;
	cout << "**                                   **" << endl;
	cout << "**                                   **" << endl;
	cout << "**        1.短进程优先调度            **" << endl;
	cout << "**        2.高响应比优先调度           **" << endl;
	cout << "**        3.时间片轮转                **" << endl;
	cout << "**                                   **" << endl;
	cout << "**                                   **" << endl;
	cout << "**************************************" << endl;
	cout << "**************************************" << endl;
}

//非抢占进程调度方式,第一个参数接收进程表，第二个参数接收调度算法
Status NonPreemptive_Mode(ProcessList* &P, PCB* (*ProcessSchedule)(ReadyQueue* &Rq))
{
	//并发运行
	Status flag = ok;
	Status flag1;
	unsigned int time_c = 0;
	//随机数引擎声明变量
	std::default_random_engine rand_num;
	PCB* super_p = nullptr;
	while (finish != flag and wrong != flag)
		//循环直到进程数达到最大数或者出现程序错误就结束
		//一次循环表示经过一个单位时间
	{
		std::cout << "-----------------" << std::endl;
		std::cout << "第" << time_c << "个标准单位时间" << std::endl;
		int ra = rand_num();
		//随机时间生成一个新进程
		if (time_c == 0 or ra % 4 != 0)
			//当随机数可以不被4整除时创建新进程(虚假的随机)，有3/4的概率
		{
			//创建进程，接收flag
			flag1 = P->CreateNewProcess();
		}
			//使用调度算法选出优先级最高的
			super_p = ProcessSchedule(P->m_Readyp);
			//判断是否为nullptr;
			if (nullptr != super_p)
				//是，进入下次循环(因为就绪队列没有进程了);
			{
				//不是，先查看5个处理机忙闲情况；
				if (true != P->m_Runp->FullRunQueue())
					//没有空闲位置，那么继续等待
				{
					//有空闲位置，插入执行队列中
					if (ok != P->RunReadyProcess(super_p))
					{
						flag = wrong;
					}
				}
			}
			//缺点：一个单位时间只能完成对一个进程的调度
		//就绪队列时间增长
		P->m_Readyp->WaitTimeAdd();
		//执行队列时间增长
		P->m_Runp->RunTimeAdd();
		//增长周转时间
		P->TurnAroundTimeAdd();
		//检查执行索引表：进程执行完后，需要将它从执行索引表移除
		if (ok != P->m_Runp->TurnProcessFinish())
			return wrong;
		//显示标准时间
		time_c++;
		//显示调度信息
		std::cout << *P;
		std::cout << "-----------------" << std::endl;
		if (flag1 == finish and P->m_Readyp->m_ReadyList.size() == 0 and P->m_Runp->m_RunList.size() == 0)
		{
			flag = finish;
		}
		//Sleep(1000);
	}
	std::cout << "平均周转时间：" << P->cal_turnaroundtime() << std::endl;
	std::cout << "平均带权周转时间：" << P->cal_w_turnaroundtime() << std::endl;
	return ok;
}

//抢占式进程调度方式
Status Preemptive_Mode(ProcessList* &P, Status (*Pre_ProcessSchedule)(ProcessList* &P))
{
	Status flag = ok;
	Status flag1;
	unsigned int time_c = 0;
	//随机数引擎声明变量
	std::default_random_engine rand_num;
	PCB* super_p = nullptr;
	while (finish != flag and wrong != flag)
		//循环直到进程数达到最大数或者出现程序错误就结束
		//一次循环表示经过一个单位时间
	{
		std::cout << "-----------------" << std::endl;
		std::cout << "第" << time_c << "个标准单位时间" << std::endl;
		int ra = rand_num();
		//随机时间生成一个新进程
		if (time_c == 0 or ra % 4 != 0)
			//当随机数可以不被4整除时创建新进程(虚假的随机)，有3/4的概率
		{
			//创建进程，接收flag1
			flag1 = P->CreateNewProcess();
		}
		//时间片轮转要做2个单位时间隔处理
		if (Pre_ProcessSchedule == TimePieceRR)
		{
			if (time_c % 2 == 0)
			{
				if (ok != Pre_ProcessSchedule(P))
					return wrong;
			}
		}
		//抢占式的进程调度算法
		else if (ok != Pre_ProcessSchedule(P))
		{
			return wrong;
		}
		//就绪队列时间增长
		P->m_Readyp->WaitTimeAdd();
		//执行队列时间增长
		P->m_Runp->RunTimeAdd();
		//增长周转时间
		P->TurnAroundTimeAdd();
		//检查执行索引表：进程执行完后，需要将它从执行索引表移除
		if (ok != P->m_Runp->TurnProcessFinish())
			return wrong;
		//显示标准时间
		time_c++;
		//显示调度信息
		std::cout << *P;
		std::cout << "-----------------" << std::endl;
		if (flag1 == finish and P->m_Readyp->m_ReadyList.size() == 0 and P->m_Runp->m_RunList.size() == 0)
		{
			flag = finish;
		}
		Sleep(1000);
	}
	std::cout << "平均周转时间：" << P->cal_turnaroundtime() << std::endl;
	std::cout << "平均带权周转时间：" << P->cal_w_turnaroundtime() << std::endl;
	return ok;
}








