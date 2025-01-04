#pragma once
#ifndef EXPERIMENT_1_HEADER
#define EXPERIMENT_1_HEADER
#include<iostream>
#include<vector>
#include<string>
#include<list>
#include<random>
#include<Windows.h>
//��������ʽ��֯PCB
class PCB;
class RunQueue;
class ReadyQueue;
//ǰ������
using Status = int;
using RunIndexType = std::list<PCB*>;
//ִ����������������
using RdIndexType = std::vector<PCB*>;
//������������������
using PCBListType = std::vector<PCB>;
//���̿��ƿ����������
const unsigned int ProcessorNum = 5;
//�������Ŀ,ϵͳ�������5������ͬʱ����
const unsigned int ProcessMaxNum = 50;
//���������
const int finish = -1;
//������������Դ���(�紴���Ľ����Ѵ������)����finish
const bool wrong = false;
//��������������(���û�������)����(�簴�û�Ҫ�����һ�������ڵĽ��̵�)����wrong
const bool ok = true;
//�������̵�����Ŀ������־

//���̱�
class ProcessList
{
private:
	RunQueue* m_Runp;
	//����ִ��ָ��
	ReadyQueue* m_Readyp;
	//���̾�����ָ��
	PCBListType m_PCBList;
	//����PCB���̵��б�
public:
	ProcessList(void);
	//���캯������
	~ProcessList(void);
	//������������Ҫ�ͷ��ڴ�ռ�
	Status CreateNewProcess(void);
	//�����½���
	Status RunReadyProcess(PCB* p);
	//���ض��Ľ��̴Ӿ���״̬ת��ִ��״̬
	Status DiscontinueProcess(PCB* p);
	//���ض����̴�ִ�ж�����ת���������(���������ж�)
	friend std::ostream& operator<<(std::ostream& os, const ProcessList& P);
	//���̱���ʾ
	void TurnAroundTimeAdd(void);
	//��תʱ����������
	float cal_turnaroundtime(void)const;
	//����ƽ����תʱ��
	float cal_w_turnaroundtime(void)const;
	//�����Ȩƽ����תʱ��
	friend Status Preemptive_Mode(ProcessList* &P, Status(*Pre_ProcessSchedule)(ProcessList* &P));
	friend Status NonPreemptive_Mode(ProcessList* &P, PCB* (*ProcessSchedule)(ReadyQueue* &Rq));
	friend Status Pre_ShortJobFirst(ProcessList* &P);
	friend Status Pre_HRRN(ProcessList* &P);
	friend Status TimePieceRR(ProcessList* &P);
	//��Ԫ����
};

//����������
class ReadyQueue
{
private:
	RdIndexType m_ReadyList;
public:
	Status AddRdProcess(PCB* p);
	//���һ�������ھ���������
	Status DeleteRdProcess(PCB* p);
	//����һ������
	void WaitTimeAdd(void);
	//�ȴ�ʱ����������
	friend std::ostream& operator<<(std::ostream& os, const ProcessList& P);
	friend PCB* Non_ShortJobFirst(ReadyQueue* &Rq);
	friend PCB* Non_HRRN(ReadyQueue* &Rq);
	friend Status Pre_ShortJobFirst(ProcessList* &P);
	friend Status Pre_HRRN(ProcessList* &P);
	friend Status TimePieceRR(ProcessList* &P);
	friend Status NonPreemptive_Mode(ProcessList* &P, PCB* (*ProcessSchedule)(ReadyQueue* &Rq));
	friend Status Preemptive_Mode(ProcessList* &P, Status(*Pre_ProcessSchedule)(ProcessList* &P));
	friend void ProcessList::TurnAroundTimeAdd(void);
	//��Ԫ����
};

//���̿��ƿ�PCB��
class PCB
{
private:
	std::string m_Id;
	//��ʶ��,���ڱ�ʾһ��Ψһ�Ľ���
	enum class State { W = 1, R, F };
	//W=Wait(����״̬),R=Run(����״̬),F=Finish(���״̬)
	State m_state;
	//����״̬��ָ�����̵�״̬
	int m_ntime;
	//���̵�Ҫ�����ʱ��
	int m_rtime;
	//��ռ��CPU����ʱ��
	int m_wtime;
	//�����ѵȴ�ʱ��
public:
	int turnaround_time;
	//���̵���תʱ��
	PCB(void)
	{
		//������Ĭ�Ϲ���
		std::cout << "�����쳣" << std::endl;
	}
	PCB(const std::string m_Id_argu , const int m_ntime_argu)
	{
		this->m_Id = std::string("");
		//�ȶ���һ�����ַ���
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
	//�����������Ƿ����������
	friend class RunQueue;
	friend class ReadyQueue;
	//��Ԫ��
	friend Status ProcessList::RunReadyProcess(PCB* p);
	friend Status ProcessList::DiscontinueProcess(PCB* p);
	//��Ԫ��Ա����
	friend std::ostream& operator<<(std::ostream& os, const ProcessList& P);
	friend PCB* Non_ShortJobFirst(ReadyQueue* &Rq);
	friend PCB* Non_HRRN(ReadyQueue* &Rq);
	friend Status Pre_ShortJobFirst(ProcessList* &P);
	friend Status Pre_HRRN(ProcessList* &P);
	friend float ProcessList::cal_w_turnaroundtime(void)const;
	//��Ԫ����
};

//ִ��������
class RunQueue
{
private:
	RunIndexType m_RunList;
public:
	Status AddRunProcess(PCB* p);
	//���ض����̼��뵽ִ�ж�����
	Status DeleteRunProcess(PCB* p,PCB::State S);
	//���ض����̴�ִ�ж�������ȥ��SΪ���̵ĺ���״̬
	void RunTimeAdd(void);
	//ִ��ʱ����������
	inline Status FullRunQueue(void)const
	{
		return (this->m_RunList.size() == ProcessorNum);
	}
	//�ж��Ƿ�ִ�ж��еĽ��̴ﵽ�����
	Status TurnProcessFinish(void);
	//���ִ����������������������Ľ����Ƴ�
	friend std::ostream& operator<<(std::ostream& os, const ProcessList& P);
	friend Status Pre_ShortJobFirst(ProcessList* &P);
	friend Status Pre_HRRN(ProcessList* &P);
	friend Status TimePieceRR(ProcessList* &P);
	friend Status NonPreemptive_Mode(ProcessList* &P, PCB* (*ProcessSchedule)(ReadyQueue* &Rq));
	friend Status Preemptive_Mode(ProcessList* &P, Status(*Pre_ProcessSchedule)(ProcessList* &P));
	friend void ProcessList::TurnAroundTimeAdd(void);
	//��Ԫ����
};

Status NonPreemptive_Mode(ProcessList* &P, PCB* (*ProcessSchedule)(ReadyQueue* &Rq));
//����ռ���̵��ȷ�ʽ,��һ���������ս��̱��ڶ����������յ����㷨

Status Preemptive_Mode(ProcessList* &P,Status (*Pre_ProcessSchedule)(ProcessList* &P));
//��ռʽ���̵��ȷ�ʽ

PCB* Non_ShortJobFirst(ReadyQueue* &Rq);
//����ռ��ʽ�Ķ̽��������㷨

PCB* Non_HRRN(ReadyQueue* &Rq);
//����ռ��ʽ�ĸ���Ӧ�����ȵ����㷨

Status TimePieceRR(ProcessList* &P);
//ʱ��Ƭ��ת�㷨
//�涨�ָ�ÿһ���������еĽ��̵�CPU����ʱ��Ϊ2����λʱ��

Status Pre_ShortJobFirst(ProcessList* &P);
//��ռʽ�Ķ̽��������㷨

Status Pre_HRRN(ProcessList* &P);
//��ռʽ�ĸ���Ӧ�����ȵ����㷨

void main_menu(void);
//��ҳ�溯��

void np_menu(void);
//����ռ��ʽ����ҳ��

void p_menu(void);
//��ռ��ʽ����ҳ��

#endif
