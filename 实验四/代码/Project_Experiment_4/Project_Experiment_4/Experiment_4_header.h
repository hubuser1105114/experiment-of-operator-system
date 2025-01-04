#pragma once
#ifndef EXPERIMENT_4_HEADER
#define EXPERIMENT_4_HEADER
#include<iostream>
#include<array>
#include<random>
#include<cmath>
#include<utility>
#include<vector>
const int MagneticTrackNum = 256;
//�ŵ���
using TrackType = std::array<bool, MagneticTrackNum>;
//������������
using HeadType = std::pair<int , bool>;
//��ͷ��������,first��ָ��ָ��ŵ�(����ŵ�)��second�Ǵ�ͷ�ƶ�����
using Status = int;
const int ok = 1;
const int wrong = 0;
const bool Left = true;
const bool Right = false;
//
//���̵���
//��һ��array�������,�±���Ǵ��̺�
//��ͷ��һ��ָ��������

//������
class Disk
{
private:
	TrackType* m_track;
	//�ŵ�����
	HeadType m_head;
	//��ͷ����
public:
	//����
	Disk(void)
	{
		m_track = new TrackType;
		m_head.first = 100;
		m_head.second = Right;
		for (int i = 0; i < MagneticTrackNum; i++)
		{
			(*m_track)[i] = true;
		}
	}

	//����
	~Disk(void)
	{
		delete m_track;
	}

	//����һ��IO����10����
	std::array<int, 10>* CreateIO(const unsigned int RandomNum);

	//�����ȷ���FCFS�㷨
	Status FCFS(int& SeekLength,std::array<int, 10>* IO);

	//���Ѱ��ʱ������SSTF�㷨
	Status SSTF(int& SeekLength, std::array<int, 10>* IO);
	
	//����Ƿ���IO������dirctionһ��
	Status IfInThisSize(std::array<int, 10>* IO , const bool direction);

	//�ҵ���ͷ���������С���Ǹ��ŵ�
	int FindMin(std::array<int, 10>* IO);

	//�鿴IO�����Ƿ��ѷ���
	Status IfIOFinish(std::array<int, 10>* IO);

	//ɨ��SCAN�㷨
	Status SCAN(int& SeekLength, std::array<int, 10>* IO);

	//ѭ��ɨ��CSCAN�㷨
	Status CSCAN(int& SeekLength, std::array<int, 10>* IO);
};


#endif
