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
//磁道数
using TrackType = std::array<bool, MagneticTrackNum>;
//磁盘数据类型
using HeadType = std::pair<int , bool>;
//磁头数据类型,first是指针指向磁道(代表磁道)，second是磁头移动方向
using Status = int;
const int ok = 1;
const int wrong = 0;
const bool Left = true;
const bool Right = false;
//
//磁盘调度
//用一个array定义磁盘,下标就是磁盘号
//磁头用一个指针来代替

//磁盘类
class Disk
{
private:
	TrackType* m_track;
	//磁道数组
	HeadType m_head;
	//磁头类型
public:
	//构造
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

	//析构
	~Disk(void)
	{
		delete m_track;
	}

	//创建一组IO任务（10个）
	std::array<int, 10>* CreateIO(const unsigned int RandomNum);

	//先来先服务FCFS算法
	Status FCFS(int& SeekLength,std::array<int, 10>* IO);

	//最短寻道时间优先SSTF算法
	Status SSTF(int& SeekLength, std::array<int, 10>* IO);
	
	//检查是否有IO任务在dirction一侧
	Status IfInThisSize(std::array<int, 10>* IO , const bool direction);

	//找到磁头方向距离最小的那个磁道
	int FindMin(std::array<int, 10>* IO);

	//查看IO任务集是否都已访问
	Status IfIOFinish(std::array<int, 10>* IO);

	//扫描SCAN算法
	Status SCAN(int& SeekLength, std::array<int, 10>* IO);

	//循环扫描CSCAN算法
	Status CSCAN(int& SeekLength, std::array<int, 10>* IO);
};


#endif
