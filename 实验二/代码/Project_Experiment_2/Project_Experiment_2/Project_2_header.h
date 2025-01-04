#pragma once
#ifndef PROJECT_2_HEADER
#define PROJECT_2_HEADER
#include<iostream>
#include<array>
#include<vector>
#include<random>
#include<string>
#include<list>
//�����5�����̲������У�ʵ����ֻ��һ����������������еĽ������Ϊ5��
//ʱ��Ƭ��ת�����㷨��ÿ��ʱ��Ƭ ���Ⱦ������еĶ��׽�������
//���м��㷨����ÿ��Ҫִ�еĽ��̽������м��㷨�����̼���Ϊ5�����еĽ���
//���һ������û�г��������Ҵ��ڰ�ȫ���У���ô����Դ����������У�һ��ʱ��Ƭ�������������
//һ������ֻ�е�������Դ�ﵽ��������ʱ��Ż��������
//һ��������ɺ󣬻��������һ���½��̣�������һ����������
//���һ�����̳�������򲻴��ڰ�ȫ���У���ô������������ʣ�¾������еĽ��̽�������
//��ɵĽ���Ҫ������Դ�������Ľ��̲��û�����Դ�������Ľ���Ҳ���û�����Դ
using Status = int;
const unsigned int ConcurrencyNum = 5;
//�������5�����̲�������
const unsigned int ProcessMaxNum = 14;
//�����������ﵽ�������ٴ����½���
const int ResourceKinds = 3;
//������Դ������
class PCB;
//ǰ������
using ResourceType = std::array<unsigned int, ResourceKinds>;
//��Դ���������������
using ProcessListType = std::vector<PCB>;
//���̱���������
using ReadyListType = std::list<PCB*>;
//����������������
using RunListType = ReadyListType;
//ִ�ж�����������
using BlockListType = ReadyListType;
//����������������
const int ok = 1;
const int wrong = 0;
const int finish = 2;
const int ShouldBlock = 3;
//����һЩ��ʶ��

//ϵͳ��Դ
class Resource
{
public:
	int A_num;
	//A����Դ�Ŀ�������
	int B_num;
	//B����Դ�Ŀ�������
	int C_num;
	//C����Դ�Ŀ�������
	static const unsigned int A = 0;
	static const unsigned int B = 1;
	static const unsigned int C = 2;
	//ָ��A��B��C����Դ�������е�����
	static const unsigned int A_maxnum = 10;
	static const unsigned int B_maxnum = 15;
	static const unsigned int C_maxnum = 8;
	//����A��B��C����Դ����������
	Resource():A_num(A_maxnum),B_num(B_maxnum),C_num(C_maxnum)
	{}
	//��ʼ����Դ��Ŀ
	~Resource(){}
};

//���̱���
class ProcessList
{
private:
	ProcessListType m_PCBList;
	//�������Ա�,����������ʽ��֯PCB
	ReadyListType* m_ReadyL;
	//��������
	RunListType* m_RunL;
	//ִ�ж��У�ʵ����ֻ��1�������
	BlockListType* m_BlockL;
	//��������
	Resource m_R;
	//������Դ��
public:
	//���캯��
	ProcessList(void)
	{
		m_PCBList.reserve(ProcessMaxNum);
		//Ԥ���㹻��λ��
		m_ReadyL = new ReadyListType;
		m_RunL = new RunListType;
		m_BlockL = new BlockListType;
		//�����ڴ�
	}
	//��������
	~ProcessList() 
	{
		delete m_ReadyL;
		delete m_RunL;
		delete m_BlockL;
		//�ͷ��ڴ�
	}
	Status CreatingProcess(void);
	//����һ������
	Status RunToReady(PCB* &p);
	//���ض����̴�ִ�ж���ת�����������
	Status ReadyToRun(PCB* &p, const ResourceType Request);
	//���ض����̴Ӿ�������ת��ִ�ж�����
	Status ReadyToBlock(const PCB* p);
	//���ض����̴Ӿ���״̬ת������״̬
	Status CheckUpFinish(void);
	//���ִ�н����Ƿ�ﵽ��ɱ�־
	Status WakeUpBlock(void);
	//���������еĶ��׽���ת�뵽����������
	Status RoundRobin(void);
	//ʱ��Ƭ��ת�����㷨
	Status Banker(PCB* &p, ResourceType& Request);
	//���м��㷨
	Status Security_algo(PCB* & p,const ResourceType& Available, int** Need, int** Allocation, std::vector<PCB*>& IndexList);
	//��ȫ���㷨
	Status ProcessScheduing(void);
    //���̵��Ⱥ���
	friend std::ostream& operator<<(std::ostream& os, ProcessList& P);
	//��Ԫ����
};

//���̿��ƿ�PCB��
class PCB
{
public:
	enum class State
	{
		W = 1,
		//Wait,����״̬
		R,
		//Run������״̬
		B,
		//Block������״̬
		F
		//Finish�����״̬
	};
private:
	State m_state;
	//����״̬
	std::string m_Id; 
	//���̱�ʶ��(������)
	ResourceType m_maxneedR;
	//��Ҫ����Դ����
	ResourceType m_hadR;
	//�ѷ������Դ��
public:
	PCB(void)
	{
		std::cout << "�����쳣" << std::endl;
		//������Ĭ�Ϲ���
	}
	PCB(const std::string Id_argu)
	{
		m_Id = std::string("");
		m_Id.append(Id_argu);
		//��ʼ�����̱�ʶ��
		m_state = State::W;
		//��״̬����Ϊ����״̬
		m_hadR[Resource::A] = m_hadR[Resource::B]
		= m_hadR[Resource::C] = 0;
		//��ʼ���ѷ�����Դ
		static std::default_random_engine e;
		//���������
		m_maxneedR[Resource::A] = e() % (Resource::A_maxnum + 1);
		m_maxneedR[Resource::B] = e() % (Resource::B_maxnum + 1);
		m_maxneedR[Resource::C] = e() % (Resource::C_maxnum + 1);
		//���������Դ
	}
	~PCB(void){}
	Status IfFinish(void)const;
	//�жϽ����Ƿ�ﵽ�������
	friend class ProcessList;
	//��Ԫ��
	friend std::ostream& operator<<(std::ostream& os, ProcessList& P);
	friend int GetListIndex(PCB* &p, std::vector<PCB*>& List);
	//��Ԫ����
};

std::ostream& operator<<(std::ostream& os, ProcessList& P);
//<<��������� ����ʾ���̱����Ϣ

int GetListIndex(PCB* &p,std::vector<PCB*>& List);
//��������
#endif
