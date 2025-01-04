#pragma once
#ifndef EXPERIMENT_3_HEADER
#define EXPERIMENT_3_HEADER
#include<iostream>
#include<list>
#include<vector>
#include<string>
#include<utility>
#include<array>
//�û��ڴ�ռ���1MB=1024KB��������KB��Ϊ�ڴ������λ
//��ҵ״̬��ΪW(����)��R(ִ��)��F(���)
//
const int MemorySize = 1024;
//�û��ڴ�ռ��С
const int ProcessorNum = 5;
//�������ͬʱ����5����ҵ
const int JobSize = 20;
//ÿ����ҵ�̶�Ϊ20KB��С
const int JobMaxNum = 20;
//�����ҵ��������������Ͳ��ٴ�����ҵ
const int JobRunTime = 6;
//��ҵ����ʱ��
//
const int ok = 1;
const int wrong = 0;
const int finish = 2;
using Status = int;
//
//��̬��������
namespace Dynamic_Partition
{
	class JCB;
	using QueueType = std::list<JCB*>;
	using ListType = std::vector<JCB>;
	//������������
	const int PartitionSize = 32;
	//������С(��̬��������ʹ��)
	//1. ��̬�����������״���Ӧ�㷨
	//һ����ҵ�������������Ƚ�����ҵ�󱸶�����
	//���������е�ʱ�򣬷���ռ䣬����ռ䲻�㣬�����У�
	//����ռ��㹻������ռ䣬������(�涨ÿ����ҵ����2��ʱ��Ƭ)
	//��������ͷ��ڴ�ռ䣬��ҵ���

	//��̬��������(���з���������)
	struct IdleListElemType
	{
		int m_num;
		//������
		int m_size;
		//������С
		int m_OgAdr;
		//����ʼַ
		enum class State
		{
			F = 1,
			//����״̬
			O
			//ռ��״̬
		};
		IdleListElemType::State m_state;
		//����״̬
	};

	using IdleListType = std::vector<IdleListElemType>;

	//��ҵ��������
	class JobScheduler
	{
	private:
		QueueType* m_BackupQueue;
		//�󱸶���
		QueueType* m_RunQueue;
		//���ж���
		ListType m_JobList;
		//��ҵ����
		IdleListType* m_ML;
		//��̬�����ռ������
	public:
		JobScheduler(void)
		{
			m_BackupQueue = new QueueType;
			m_RunQueue = new QueueType;
			m_JobList.reserve(JobMaxNum);
			m_ML = new IdleListType;
			m_ML->reserve(MemorySize / PartitionSize);
		}
		//���캯��
		~JobScheduler(void)
		{
			delete m_BackupQueue;
			delete m_RunQueue;
			delete m_ML;
		}
		//��������

		Status Init_ML(void);
		//��ʼ�����з�����

		Status CreatingJob(void);
		//������ҵ����

		Status AllocateMemory(JCB* &j);
		//�����ڴ�

		Status DeallocateMemory(JCB* &j);
		//�����ڴ�

		void RunTimeIncreasing(void);
		//ִ��ʱ����������

		Status ReadyToRun(JCB* &j);
		//�Ѿ�����ҵת��ִ��״̬

		Status RunToReady(JCB* &j);
		//��ִ����ҵת�����״̬

		Status TurnToFinish(void);
		//�����ҵ�Ƿ����

		friend void JobScheduling_DM(JobScheduler* &J);
		friend Status Round_Robin_DM(JobScheduler* &J);
		friend std::ostream& operator<<(std::ostream& os, IdleListType* ML);
		friend std::ostream& operator<<(std::ostream& os, QueueType* RunQueue);
		//��Ԫ����
	};

	//JCB��ҵ���ƿ���
	class JCB
	{
	public:
		enum class State
		{
			W = 1,
			//����״̬
			F,
			//���״̬
			R
			//����״̬
		};
	private:
		int m_ntime;
		//��ҵ�������е�ʱ��
		std::string m_Id;
		//��ҵ��ʶ��
		JCB::State m_state;
		//��ҵ״̬
		int m_PartGetAdr;
		//��õķ�����ַ����û�У���ôΪ-1
	public:
		JCB(void)
		{
			std::cout << "�����쳣" << std::endl;
			//������Ĭ�Ϲ���
		}
		JCB(const std::string Id_argu)
		{
			m_ntime = JobRunTime;
			m_Id = std::string("");
			m_Id += Id_argu;
			m_state = JCB::State::W;
			m_PartGetAdr = -1;
			//��ʼ����Ա����
		}
		~JCB(void) {}
		inline Status IfFinish(void)
		{
			return (m_ntime <= 0);
		}
		//�����ҵ�Ƿ��Ѿ����
		friend class JobScheduler;
		//��Ԫ��
		friend std::ostream& operator<<(std::ostream& os, QueueType* RunQueue);
	};

	std::ostream& operator<<(std::ostream& os, IdleListType* ML);
	//����������

	std::ostream& operator<<(std::ostream& os, QueueType* RunQueue);
	//����������

	Status Round_Robin_DM(JobScheduler* &J);
	//ʱ��Ƭ��ת����-��̬����

	void JobScheduling_DM(JobScheduler* &J);
	//��̬����-��ҵ���Ⱥ���
}

//������ҳ����
namespace Paging
{
	class JCB;
	class JobScheduler;
	//ǰ������
	using QueueType = std::list<JCB*>;
	using ListType = std::vector<JCB>;
	//������������
	const int PageSize = 2;
	//ҳ���С
	const int Sectionsize = 2;
	//�δ�С
	using PageTableType = std::array<int, (JobSize / Sectionsize)>;
	//ҳ����������
	using PTHeaderType = std::pair<PageTableType*, unsigned int>;
	//ҳ��ͷ��������,first��ҳ��ʼַ��second��ҳ���С
	using MemoryList = std::array<bool, (MemorySize / PageSize)>;
	//�ڴ�״̬��������
	//using PTRType = std::list<std::pair<std::string, PTHeaderType>>;
	////ҳ��Ĵ���PTR����
	const bool Busy = false;
	const bool Free = true;
	//�����ڴ�״̬
	//
//2. ��ҳʽ����-��ַ˳��
//���û��ռ�ֳɹ̶���С�Ŀ飬ÿ��2KB��С
//��ҵ�ĵ�ַ�ռ�Ҳ��Ϊ�̶���С��ҳ�棬ÿҳ2KB��С
//������ҵ�ĸ���ҳ��ɢ�ش洢�ڿ���
//ÿ����ҵ����һ��ҳ��ҳ���к���ҳ��-��ŵĶ�Ӧ��ϵ
//
//ÿ����ҵ�������󣬾��ж�Ӧ��ҳ��(�����ûװ�룬��ô��Ӧ�Ŀ��Ϊ-1)
//ת��ִ��״̬������ҵ��ÿһҳ����ַ˳������ÿһ��
//���յ�ʱ�򣬾Ͱѿ�Ż���
//

//��ҵ��������
	class JobScheduler
	{
	private:
		QueueType* m_BackUpQueue;
		//�󱸶���
		QueueType* m_RunQueue;
		//ִ�ж���
		ListType m_JobList;
		//��ҵ����
		MemoryList* m_MS;
		//�ڴ�״̬����
		//PTRType* m_PTR;
		////ҳ��Ĵ���PTR
	public:
		//Ĭ�Ϲ���
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

		//����
		~JobScheduler(void)
		{
			delete m_BackUpQueue;
			delete m_RunQueue;
			delete m_MS;
			//delete m_PTR;
		}

		//������ҵ�������뵽�󱸶���
		Status JobCreating(void);

		//�����ڴ�
		Status DeAllocateMemory(JCB* &j);

		//��j��ҵ��ִ��״̬תΪ����״̬
		Status RunToReady(JCB* &j);

		//���ض���ҵ�Ӿ���״̬ת��ִ��״̬
		Status ReadyToRun(JCB* &j);

		Status AllocateMemory(JCB* &j);
		//Ϊ��ҵj�����ڴ�

		void RunTimeIncrease(void);
		//����ʱ������

		void FindHasFinish(void);
		//����ɵ���ҵת�����״̬

		//��Ԫ����
		friend Status RobinRound_Paging(JobScheduler* &J);
		friend Status JobScheduling_Paging(JobScheduler* &J);
		friend std::ostream& operator<<(std::ostream& os, Paging::JobScheduler* &J);
	};

//��ҵ���ƿ���
	class JCB
	{
	public:
		enum class State
		{
			W = 1,
			//����״̬
			F,
			//���״̬
			R
			//����״̬
		};
	private:
		std::string m_Id;
		//��ҵ��ʶ��
		int m_ntime;
		//��ҵ�������е�ʱ��
		JCB::State m_state;
		//��ҵ״̬
		PTHeaderType m_pt;
		//����ҳ��,first��ҳ��ʼַ��second��ҳ����
	public:
		//Ĭ�Ϲ���
		JCB(void)
		{
			std::cout << "�����쳣" << std::endl;
			//������Ĭ�Ϲ���
		}

		//����ʶ���Ĳ�������
		JCB(const std::string Id_argu)
		{
			m_Id = std::string("");
			m_Id += Id_argu;
			//��ʶ��
			m_ntime = JobRunTime;
			//����ʱ��
			m_state = State::W;
			//��ҵ״̬
			m_pt.first = new PageTableType;
			m_pt.second = (JobSize / Sectionsize);  //ҳ����
			//ҳ��ͷ��ʼ��
			for (int i = 0; i < m_pt.second; i++)
			{
				(*(m_pt.first))[i] = -1;
			}
			//��ʼ����õĿ��
		}

		//���ƹ��캯������ΪҪ���(��CreatingJob������vector��push_back)
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

		//����
		~JCB(void)
		{
			delete m_pt.first;
			m_pt.first = nullptr;
			//�ͷſռ�
		}

		//����Ƿ���ҵ���
		inline Status IfFinish(void)const
		{
			return (m_ntime <= 0);
		}

		friend class JobScheduler;
		//��Ԫ��

		friend std::ostream& Paging::operator<<(std::ostream& os, Paging::JobScheduler* &J);
	};

	//ʱ��Ƭ�����㷨-������ҳ����
	Status RobinRound_Paging(JobScheduler* &J);

	//��ҵ����(������ҳ����)
	Status JobScheduling_Paging(JobScheduler* &J);
	
	//<<����
	std::ostream& operator<<(std::ostream& os, Paging::JobScheduler* &J);
}

//�����ֶι���
namespace Segment
{
	class JCB;
	using QueueType = std::list<JCB*>;
	using ListType = std::vector<JCB>;
	//������������
	//���з����������
	struct Node
	{
		int m_id;
		//������
		int m_size;
		//������С/KB
		int m_addr;
		//����ʼַ
		Node* front , *next;
		//ǰ������ָ��
	};

	//��ҵ��������
	class JobScheduler
	{
	private:
		QueueType* m_BackUpQueue;
		//�󱸶���
		QueueType* m_RunQueue;
		//ִ�ж���
		ListType m_JCBList;
		//JCB�б�
		Node* m_head;
		//���з�������ͷ���
	public:
		//Ĭ�Ϲ���
		JobScheduler(void);

		//����
		~JobScheduler(void);

		//������ҵ�������뵽�󱸶���
		Status JobCreating(void);

		//�����ڴ�
		Status DeAllocateMemory(JCB* &j);

		//��j��ҵ��ִ��״̬תΪ����״̬
		Status RunToReady(JCB* &j);

		//���ض���ҵ�Ӿ���״̬ת��ִ��״̬
		Status ReadyToRun(JCB* &j);

		Status AllocateMemory(JCB* &j);
		//Ϊ��ҵj�����ڴ�

		void RunTimeIncrease(void);
		//����ʱ������

		void FindHasFinish(void);
		//����ɵ���ҵת�����״̬

		//��Ԫ����
		friend Status RobinRound_Segment(JobScheduler* &J);
		friend Status JobScheduling_Segment(JobScheduler* &J);
		friend std::ostream& operator<<(std::ostream& os, Segment::JobScheduler* &J);
	};

	//��ҵ���ƿ���
	class JCB
	{
	public:
		enum class State
		{
			W = 1,
			//����
			F,
			//���
			R
			//ִ��
		};
	private:
		std::string m_Id;
		//��ҵ��ʶ��
		State m_state;
		//��ҵ״̬
		Node* m_node;
		//��õķ������ڵ�ֵ
		int m_ntime;
		//�������е�ʱ��
	public:
		//Ĭ�Ϲ���
		JCB(void)
		{
			std::cout << "�����쳣" << std::endl;
		}
		//����
		JCB(std::string Id_argu);
		//����
		~JCB(void);

		//����Ƿ���ҵ���
		inline Status IfFinish(void)const
		{
			return (m_ntime <= 0);
		}

		friend class JobScheduler;
		//��Ԫ��

		friend std::ostream& operator<<(std::ostream& os, Segment::JobScheduler* &J);
	};
}

void main_menu();
//��ҳ��(��DynaPart_function����)

void fake_show();
#endif