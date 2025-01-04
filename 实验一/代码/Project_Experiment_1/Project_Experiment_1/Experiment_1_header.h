#pragma once
#ifndef EXPERIMENT_1_HEADER
#define EXPERIMENT_1_HEADER
#include<iostream>
#include<vector>
#include<string>
#include<list>
#include<random>
#include<Windows.h>
//按索引方式组织PCB
class PCB;
class RunQueue;
class ReadyQueue;
//前向声明
using Status = int;
using RunIndexType = std::list<PCB*>;
//执行索引表数据类型
using RdIndexType = std::vector<PCB*>;
//就绪索引表数据类型
using PCBListType = std::vector<PCB>;
//进程控制块表数据类型
const unsigned int ProcessorNum = 5;
//处理机数目,系统最多允许5个进程同时运行
const unsigned int ProcessMaxNum = 50;
//进程最大数
const int finish = -1;
//当程序发生完成性错误(如创建的进程已达最大数)返回finish
const bool wrong = false;
//当程序发生程序性(或用户输入性)错误(如按用户要求访问一个不存在的进程等)返回wrong
const bool ok = true;
//设立进程调度项目反馈标志

//进程表
class ProcessList
{
private:
	RunQueue* m_Runp;
	//进程执行指针
	ReadyQueue* m_Readyp;
	//进程就绪表指针
	PCBListType m_PCBList;
	//所有PCB进程的列表
public:
	ProcessList(void);
	//构造函数声明
	~ProcessList(void);
	//析构函数，需要释放内存空间
	Status CreateNewProcess(void);
	//创建新进程
	Status RunReadyProcess(PCB* p);
	//将特定的进程从就绪状态转入执行状态
	Status DiscontinueProcess(PCB* p);
	//将特定进程从执行队列中转入就绪队列(即发生了中断)
	friend std::ostream& operator<<(std::ostream& os, const ProcessList& P);
	//进程表显示
	void TurnAroundTimeAdd(void);
	//周转时间增长函数
	float cal_turnaroundtime(void)const;
	//计算平均周转时间
	float cal_w_turnaroundtime(void)const;
	//计算带权平均周转时间
	friend Status Preemptive_Mode(ProcessList* &P, Status(*Pre_ProcessSchedule)(ProcessList* &P));
	friend Status NonPreemptive_Mode(ProcessList* &P, PCB* (*ProcessSchedule)(ReadyQueue* &Rq));
	friend Status Pre_ShortJobFirst(ProcessList* &P);
	friend Status Pre_HRRN(ProcessList* &P);
	friend Status TimePieceRR(ProcessList* &P);
	//友元函数
};

//就绪索引表
class ReadyQueue
{
private:
	RdIndexType m_ReadyList;
public:
	Status AddRdProcess(PCB* p);
	//入队一个进程在就绪队列中
	Status DeleteRdProcess(PCB* p);
	//出队一个进程
	void WaitTimeAdd(void);
	//等待时间增长函数
	friend std::ostream& operator<<(std::ostream& os, const ProcessList& P);
	friend PCB* Non_ShortJobFirst(ReadyQueue* &Rq);
	friend PCB* Non_HRRN(ReadyQueue* &Rq);
	friend Status Pre_ShortJobFirst(ProcessList* &P);
	friend Status Pre_HRRN(ProcessList* &P);
	friend Status TimePieceRR(ProcessList* &P);
	friend Status NonPreemptive_Mode(ProcessList* &P, PCB* (*ProcessSchedule)(ReadyQueue* &Rq));
	friend Status Preemptive_Mode(ProcessList* &P, Status(*Pre_ProcessSchedule)(ProcessList* &P));
	friend void ProcessList::TurnAroundTimeAdd(void);
	//友元函数
};

//进程控制块PCB类
class PCB
{
private:
	std::string m_Id;
	//标识符,用于表示一个唯一的进程
	enum class State { W = 1, R, F };
	//W=Wait(就绪状态),R=Run(运行状态),F=Finish(完成状态)
	State m_state;
	//进程状态，指明进程的状态
	int m_ntime;
	//进程的要求服务时间
	int m_rtime;
	//已占用CPU服务时间
	int m_wtime;
	//进程已等待时间
public:
	int turnaround_time;
	//进程的周转时间
	PCB(void)
	{
		//不允许默认构造
		std::cout << "引发异常" << std::endl;
	}
	PCB(const std::string m_Id_argu , const int m_ntime_argu)
	{
		this->m_Id = std::string("");
		//先定义一个空字符串
		this->m_Id.append(m_Id_argu);
		this->m_state = PCB::State::W;
		this->m_ntime = m_ntime_argu;
		this->m_rtime = 0;
		this->m_wtime = 0;
		this->turnaround_time = 0;
	}
	~PCB(void) {}
	Status IfProcessDone(void)const
	{
		return (this->m_rtime >= this->m_ntime);
	}
	//检测自身进程是否已完成运行
	friend class RunQueue;
	friend class ReadyQueue;
	//友元类
	friend Status ProcessList::RunReadyProcess(PCB* p);
	friend Status ProcessList::DiscontinueProcess(PCB* p);
	//友元成员函数
	friend std::ostream& operator<<(std::ostream& os, const ProcessList& P);
	friend PCB* Non_ShortJobFirst(ReadyQueue* &Rq);
	friend PCB* Non_HRRN(ReadyQueue* &Rq);
	friend Status Pre_ShortJobFirst(ProcessList* &P);
	friend Status Pre_HRRN(ProcessList* &P);
	friend float ProcessList::cal_w_turnaroundtime(void)const;
	//友元函数
};

//执行索引表
class RunQueue
{
private:
	RunIndexType m_RunList;
public:
	Status AddRunProcess(PCB* p);
	//把特定进程加入到执行队列里
	Status DeleteRunProcess(PCB* p,PCB::State S);
	//把特定进程从执行队列中移去，S为进程的后续状态
	void RunTimeAdd(void);
	//执行时间增长函数
	inline Status FullRunQueue(void)const
	{
		return (this->m_RunList.size() == ProcessorNum);
	}
	//判断是否执行队列的进程达到最大数
	Status TurnProcessFinish(void);
	//检查执行索引表，把所有已运行完的进程移除
	friend std::ostream& operator<<(std::ostream& os, const ProcessList& P);
	friend Status Pre_ShortJobFirst(ProcessList* &P);
	friend Status Pre_HRRN(ProcessList* &P);
	friend Status TimePieceRR(ProcessList* &P);
	friend Status NonPreemptive_Mode(ProcessList* &P, PCB* (*ProcessSchedule)(ReadyQueue* &Rq));
	friend Status Preemptive_Mode(ProcessList* &P, Status(*Pre_ProcessSchedule)(ProcessList* &P));
	friend void ProcessList::TurnAroundTimeAdd(void);
	//友元函数
};

Status NonPreemptive_Mode(ProcessList* &P, PCB* (*ProcessSchedule)(ReadyQueue* &Rq));
//非抢占进程调度方式,第一个参数接收进程表，第二个参数接收调度算法

Status Preemptive_Mode(ProcessList* &P,Status (*Pre_ProcessSchedule)(ProcessList* &P));
//抢占式进程调度方式

PCB* Non_ShortJobFirst(ReadyQueue* &Rq);
//非抢占方式的短进程优先算法

PCB* Non_HRRN(ReadyQueue* &Rq);
//非抢占方式的高响应比优先调度算法

Status TimePieceRR(ProcessList* &P);
//时间片轮转算法
//规定分给每一个就绪队列的进程的CPU处理时间为2个单位时间

Status Pre_ShortJobFirst(ProcessList* &P);
//抢占式的短进程优先算法

Status Pre_HRRN(ProcessList* &P);
//抢占式的高响应比优先调度算法

void main_menu(void);
//主页面函数

void np_menu(void);
//非抢占方式调度页面

void p_menu(void);
//抢占方式调度页面

#endif
