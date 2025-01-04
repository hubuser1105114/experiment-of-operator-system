#pragma once
#ifndef PROJECT_2_HEADER
#define PROJECT_2_HEADER
#include<iostream>
#include<array>
#include<vector>
#include<random>
#include<string>
#include<list>
//最多有5个进程并发运行：实际上只有一个处理机，就绪队列的进程最多为5个
//时间片轮转调度算法：每个时间片 调度就绪队列的队首进程运行
//银行家算法：对每个要执行的进程进行银行家算法，进程集合为5个并行的进程
//如果一个进程没有出现问题且存在安全序列，那么把资源分配给它运行，一个时间片后调度其他进程
//一个进程只有当已有资源达到最大需求的时候才会完成运行
//一个进程完成后，会随机创建一个新进程，并唤醒一个阻塞进程
//如果一个进程出现问题或不存在安全序列，那么把它阻塞，对剩下就绪队列的进程进行运行
//完成的进程要回收资源，就绪的进程不用回收资源，阻塞的进程也不用回收资源
using Status = int;
const unsigned int ConcurrencyNum = 5;
//最多允许5个进程并发进行
const unsigned int ProcessMaxNum = 14;
//最大进程数，达到此数后不再创建新进程
const int ResourceKinds = 3;
//定义资源种类数
class PCB;
//前向声明
using ResourceType = std::array<unsigned int, ResourceKinds>;
//资源种类变量数据类型
using ProcessListType = std::vector<PCB>;
//进程表数据类型
using ReadyListType = std::list<PCB*>;
//就绪队列数据类型
using RunListType = ReadyListType;
//执行队列数据类型
using BlockListType = ReadyListType;
//阻塞队列数据类型
const int ok = 1;
const int wrong = 0;
const int finish = 2;
const int ShouldBlock = 3;
//定义一些标识符

//系统资源
class Resource
{
public:
	int A_num;
	//A类资源的可用数量
	int B_num;
	//B类资源的可用数量
	int C_num;
	//C类资源的可用数量
	static const unsigned int A = 0;
	static const unsigned int B = 1;
	static const unsigned int C = 2;
	//指定A、B、C类资源在数组中的索引
	static const unsigned int A_maxnum = 10;
	static const unsigned int B_maxnum = 15;
	static const unsigned int C_maxnum = 8;
	//定义A、B、C类资源的最大可用数
	Resource():A_num(A_maxnum),B_num(B_maxnum),C_num(C_maxnum)
	{}
	//初始化资源数目
	~Resource(){}
};

//进程表类
class ProcessList
{
private:
	ProcessListType m_PCBList;
	//进程线性表,采用索引方式组织PCB
	ReadyListType* m_ReadyL;
	//就绪队列
	RunListType* m_RunL;
	//执行队列，实际上只有1个处理机
	BlockListType* m_BlockL;
	//阻塞队列
	Resource m_R;
	//进程资源池
public:
	//构造函数
	ProcessList(void)
	{
		m_PCBList.reserve(ProcessMaxNum);
		//预留足够的位置
		m_ReadyL = new ReadyListType;
		m_RunL = new RunListType;
		m_BlockL = new BlockListType;
		//分配内存
	}
	//析构函数
	~ProcessList() 
	{
		delete m_ReadyL;
		delete m_RunL;
		delete m_BlockL;
		//释放内存
	}
	Status CreatingProcess(void);
	//创建一个进程
	Status RunToReady(PCB* &p);
	//把特定进程从执行队列转入就绪队列中
	Status ReadyToRun(PCB* &p, const ResourceType Request);
	//把特定进程从就绪队列转入执行队列中
	Status ReadyToBlock(const PCB* p);
	//把特定进程从就绪状态转入阻塞状态
	Status CheckUpFinish(void);
	//检查执行进程是否达到完成标志
	Status WakeUpBlock(void);
	//把阻塞队列的队首进程转入到就绪队列中
	Status RoundRobin(void);
	//时间片轮转调度算法
	Status Banker(PCB* &p, ResourceType& Request);
	//银行家算法
	Status Security_algo(PCB* & p,const ResourceType& Available, int** Need, int** Allocation, std::vector<PCB*>& IndexList);
	//安全性算法
	Status ProcessScheduing(void);
    //进程调度函数
	friend std::ostream& operator<<(std::ostream& os, ProcessList& P);
	//友元函数
};

//进程控制块PCB类
class PCB
{
public:
	enum class State
	{
		W = 1,
		//Wait,就绪状态
		R,
		//Run，运行状态
		B,
		//Block，阻塞状态
		F
		//Finish，完成状态
	};
private:
	State m_state;
	//进程状态
	std::string m_Id; 
	//进程标识符(进程名)
	ResourceType m_maxneedR;
	//需要的资源总数
	ResourceType m_hadR;
	//已分配的资源数
public:
	PCB(void)
	{
		std::cout << "引发异常" << std::endl;
		//不允许默认构造
	}
	PCB(const std::string Id_argu)
	{
		m_Id = std::string("");
		m_Id.append(Id_argu);
		//初始化进程标识符
		m_state = State::W;
		//将状态设置为就绪状态
		m_hadR[Resource::A] = m_hadR[Resource::B]
		= m_hadR[Resource::C] = 0;
		//初始化已分配资源
		static std::default_random_engine e;
		//随机数引擎
		m_maxneedR[Resource::A] = e() % (Resource::A_maxnum + 1);
		m_maxneedR[Resource::B] = e() % (Resource::B_maxnum + 1);
		m_maxneedR[Resource::C] = e() % (Resource::C_maxnum + 1);
		//随机申请资源
	}
	~PCB(void){}
	Status IfFinish(void)const;
	//判断进程是否达到完成条件
	friend class ProcessList;
	//友元类
	friend std::ostream& operator<<(std::ostream& os, ProcessList& P);
	friend int GetListIndex(PCB* &p, std::vector<PCB*>& List);
	//友元函数
};

std::ostream& operator<<(std::ostream& os, ProcessList& P);
//<<运算符重载 ，显示进程表的信息

int GetListIndex(PCB* &p,std::vector<PCB*>& List);
//查找索引
#endif
