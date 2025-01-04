#pragma once
#ifndef EXPERIMENT_3_HEADER
#define EXPERIMENT_3_HEADER
#include<iostream>
#include<list>
#include<vector>
#include<string>
#include<utility>
#include<array>
//用户内存空间有1MB=1024KB，这里以KB作为内存基本单位
//作业状态分为W(就绪)、R(执行)、F(完成)
//
const int MemorySize = 1024;
//用户内存空间大小
const int ProcessorNum = 5;
//最多允许同时运行5个作业
const int JobSize = 20;
//每个作业固定为20KB大小
const int JobMaxNum = 20;
//最大作业数，超过这个数就不再创建作业
const int JobRunTime = 6;
//作业运行时间
//
const int ok = 1;
const int wrong = 0;
const int finish = 2;
using Status = int;
//
//动态分区管理
namespace Dynamic_Partition
{
	class JCB;
	using QueueType = std::list<JCB*>;
	using ListType = std::vector<JCB>;
	//队列数据类型
	const int PartitionSize = 32;
	//分区大小(动态分区管理使用)
	//1. 动态分区管理——首次适应算法
	//一个作业创建出来，首先进入作业后备队列中
	//当可以运行的时候，分配空间，如果空间不足，不运行；
	//如果空间足够，分配空间，并运行(规定每个作业运行2个时间片)
	//运行完后，释放内存空间，作业完成

	//动态分区管理(空闲分区表项类)
	struct IdleListElemType
	{
		int m_num;
		//分区号
		int m_size;
		//分区大小
		int m_OgAdr;
		//分区始址
		enum class State
		{
			F = 1,
			//空闲状态
			O
			//占用状态
		};
		IdleListElemType::State m_state;
		//分区状态
	};

	using IdleListType = std::vector<IdleListElemType>;

	//作业调度器类
	class JobScheduler
	{
	private:
		QueueType* m_BackupQueue;
		//后备队列
		QueueType* m_RunQueue;
		//运行队列
		ListType m_JobList;
		//作业数组
		IdleListType* m_ML;
		//动态分区空间分区表
	public:
		JobScheduler(void)
		{
			m_BackupQueue = new QueueType;
			m_RunQueue = new QueueType;
			m_JobList.reserve(JobMaxNum);
			m_ML = new IdleListType;
			m_ML->reserve(MemorySize / PartitionSize);
		}
		//构造函数
		~JobScheduler(void)
		{
			delete m_BackupQueue;
			delete m_RunQueue;
			delete m_ML;
		}
		//析构函数

		Status Init_ML(void);
		//初始化空闲分区表

		Status CreatingJob(void);
		//创建作业函数

		Status AllocateMemory(JCB* &j);
		//分配内存

		Status DeallocateMemory(JCB* &j);
		//回收内存

		void RunTimeIncreasing(void);
		//执行时间增长函数

		Status ReadyToRun(JCB* &j);
		//把就绪作业转入执行状态

		Status RunToReady(JCB* &j);
		//把执行作业转入就绪状态

		Status TurnToFinish(void);
		//检查作业是否完成

		friend void JobScheduling_DM(JobScheduler* &J);
		friend Status Round_Robin_DM(JobScheduler* &J);
		friend std::ostream& operator<<(std::ostream& os, IdleListType* ML);
		friend std::ostream& operator<<(std::ostream& os, QueueType* RunQueue);
		//友元函数
	};

	//JCB作业控制块类
	class JCB
	{
	public:
		enum class State
		{
			W = 1,
			//就绪状态
			F,
			//完成状态
			R
			//运行状态
		};
	private:
		int m_ntime;
		//作业还需运行的时间
		std::string m_Id;
		//作业标识符
		JCB::State m_state;
		//作业状态
		int m_PartGetAdr;
		//获得的分区首址；若没有，那么为-1
	public:
		JCB(void)
		{
			std::cout << "引发异常" << std::endl;
			//不允许默认构造
		}
		JCB(const std::string Id_argu)
		{
			m_ntime = JobRunTime;
			m_Id = std::string("");
			m_Id += Id_argu;
			m_state = JCB::State::W;
			m_PartGetAdr = -1;
			//初始化成员变量
		}
		~JCB(void) {}
		inline Status IfFinish(void)
		{
			return (m_ntime <= 0);
		}
		//检查作业是否已经完成
		friend class JobScheduler;
		//友元类
		friend std::ostream& operator<<(std::ostream& os, QueueType* RunQueue);
	};

	std::ostream& operator<<(std::ostream& os, IdleListType* ML);
	//操作符重载

	std::ostream& operator<<(std::ostream& os, QueueType* RunQueue);
	//操作符重载

	Status Round_Robin_DM(JobScheduler* &J);
	//时间片轮转调度-动态分区

	void JobScheduling_DM(JobScheduler* &J);
	//动态分区-作业调度函数
}

//基本分页管理
namespace Paging
{
	class JCB;
	class JobScheduler;
	//前向声明
	using QueueType = std::list<JCB*>;
	using ListType = std::vector<JCB>;
	//队列数据类型
	const int PageSize = 2;
	//页面大小
	const int Sectionsize = 2;
	//段大小
	using PageTableType = std::array<int, (JobSize / Sectionsize)>;
	//页表数据类型
	using PTHeaderType = std::pair<PageTableType*, unsigned int>;
	//页表头数据类型,first是页表始址，second是页表大小
	using MemoryList = std::array<bool, (MemorySize / PageSize)>;
	//内存状态数据类型
	//using PTRType = std::list<std::pair<std::string, PTHeaderType>>;
	////页表寄存器PTR类型
	const bool Busy = false;
	const bool Free = true;
	//定义内存状态
	//
//2. 分页式管理-地址顺序
//把用户空间分成固定大小的块，每块2KB大小
//作业的地址空间也分为固定大小的页面，每页2KB大小
//允许作业的各个页离散地存储在块中
//每个作业都有一个页表，页表中含有页号-块号的对应关系
//
//每个作业被创建后，就有对应的页表(如果还没装入，那么对应的块号为-1)
//转入执行状态，把作业的每一页按地址顺序分配给每一块
//回收的时候，就把块号回收
//

//作业调度器类
	class JobScheduler
	{
	private:
		QueueType* m_BackUpQueue;
		//后备队列
		QueueType* m_RunQueue;
		//执行队列
		ListType m_JobList;
		//作业数组
		MemoryList* m_MS;
		//内存状态数组
		//PTRType* m_PTR;
		////页表寄存器PTR
	public:
		//默认构造
		JobScheduler(void)
		{
			m_BackUpQueue = new QueueType;
			m_RunQueue = new QueueType;
			m_JobList.reserve(JobMaxNum);
			m_MS = new MemoryList;
			//m_PTR = new PTRType;
			for (auto i = m_MS->begin(); i != m_MS->end(); i++)
			{
				*i = Free;
			}
		}

		//析构
		~JobScheduler(void)
		{
			delete m_BackUpQueue;
			delete m_RunQueue;
			delete m_MS;
			//delete m_PTR;
		}

		//创建作业，并加入到后备队列
		Status JobCreating(void);

		//回收内存
		Status DeAllocateMemory(JCB* &j);

		//把j作业从执行状态转为就绪状态
		Status RunToReady(JCB* &j);

		//把特定作业从就绪状态转入执行状态
		Status ReadyToRun(JCB* &j);

		Status AllocateMemory(JCB* &j);
		//为作业j分配内存

		void RunTimeIncrease(void);
		//运行时间增长

		void FindHasFinish(void);
		//把完成的作业转入完成状态

		//友元函数
		friend Status RobinRound_Paging(JobScheduler* &J);
		friend Status JobScheduling_Paging(JobScheduler* &J);
		friend std::ostream& operator<<(std::ostream& os, Paging::JobScheduler* &J);
	};

//作业控制块类
	class JCB
	{
	public:
		enum class State
		{
			W = 1,
			//就绪状态
			F,
			//完成状态
			R
			//运行状态
		};
	private:
		std::string m_Id;
		//作业标识符
		int m_ntime;
		//作业还需运行的时间
		JCB::State m_state;
		//作业状态
		PTHeaderType m_pt;
		//抽象页表,first是页表始址，second是页表长度
	public:
		//默认构造
		JCB(void)
		{
			std::cout << "引发异常" << std::endl;
			//不允许默认构造
		}

		//带标识符的参数构造
		JCB(const std::string Id_argu)
		{
			m_Id = std::string("");
			m_Id += Id_argu;
			//标识符
			m_ntime = JobRunTime;
			//运行时间
			m_state = State::W;
			//作业状态
			m_pt.first = new PageTableType;
			m_pt.second = (JobSize / Sectionsize);  //页表长度
			//页表头初始化
			for (int i = 0; i < m_pt.second; i++)
			{
				(*(m_pt.first))[i] = -1;
			}
			//初始化获得的块号
		}

		//复制构造函数，因为要深拷贝(在CreatingJob函数的vector的push_back)
		JCB(const JCB& j)
		{
			m_Id = std::string("");
			m_Id += j.m_Id;
			m_ntime = j.m_ntime;
			m_state = j.m_state;
			m_pt.first = new PageTableType;
			m_pt.second = j.m_pt.second;
			for (int i = 0; i < m_pt.second; i++)
			{
				(*(m_pt.first))[i] = (*((j.m_pt.first)))[i];
			}
		}

		//析构
		~JCB(void)
		{
			delete m_pt.first;
			m_pt.first = nullptr;
			//释放空间
		}

		//检查是否作业完成
		inline Status IfFinish(void)const
		{
			return (m_ntime <= 0);
		}

		friend class JobScheduler;
		//友元类

		friend std::ostream& Paging::operator<<(std::ostream& os, Paging::JobScheduler* &J);
	};

	//时间片调度算法-基本分页管理
	Status RobinRound_Paging(JobScheduler* &J);

	//作业调度(基本分页管理)
	Status JobScheduling_Paging(JobScheduler* &J);
	
	//<<重载
	std::ostream& operator<<(std::ostream& os, Paging::JobScheduler* &J);
}

//基本分段管理
namespace Segment
{
	class JCB;
	using QueueType = std::list<JCB*>;
	using ListType = std::vector<JCB>;
	//队列数据类型
	//空闲分区链结点类
	struct Node
	{
		int m_id;
		//分区号
		int m_size;
		//分区大小/KB
		int m_addr;
		//分区始址
		Node* front , *next;
		//前、后向指针
	};

	//作业调度器类
	class JobScheduler
	{
	private:
		QueueType* m_BackUpQueue;
		//后备队列
		QueueType* m_RunQueue;
		//执行队列
		ListType m_JCBList;
		//JCB列表
		Node* m_head;
		//空闲分区链的头结点
	public:
		//默认构造
		JobScheduler(void);

		//析构
		~JobScheduler(void);

		//创建作业，并加入到后备队列
		Status JobCreating(void);

		//回收内存
		Status DeAllocateMemory(JCB* &j);

		//把j作业从执行状态转为就绪状态
		Status RunToReady(JCB* &j);

		//把特定作业从就绪状态转入执行状态
		Status ReadyToRun(JCB* &j);

		Status AllocateMemory(JCB* &j);
		//为作业j分配内存

		void RunTimeIncrease(void);
		//运行时间增长

		void FindHasFinish(void);
		//把完成的作业转入完成状态

		//友元函数
		friend Status RobinRound_Segment(JobScheduler* &J);
		friend Status JobScheduling_Segment(JobScheduler* &J);
		friend std::ostream& operator<<(std::ostream& os, Segment::JobScheduler* &J);
	};

	//作业控制块类
	class JCB
	{
	public:
		enum class State
		{
			W = 1,
			//就绪
			F,
			//完成
			R
			//执行
		};
	private:
		std::string m_Id;
		//作业标识符
		State m_state;
		//作业状态
		Node* m_node;
		//获得的分区链节点值
		int m_ntime;
		//还需运行的时间
	public:
		//默认构造
		JCB(void)
		{
			std::cout << "引发异常" << std::endl;
		}
		//构造
		JCB(std::string Id_argu);
		//析构
		~JCB(void);

		//检查是否作业完成
		inline Status IfFinish(void)const
		{
			return (m_ntime <= 0);
		}

		friend class JobScheduler;
		//友元类

		friend std::ostream& operator<<(std::ostream& os, Segment::JobScheduler* &J);
	};
}

void main_menu();
//主页面(在DynaPart_function定义)

void fake_show();
#endif