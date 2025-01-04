#include"Experiment_4_header.h"
//创建一组IO任务（10个）
std::array<int, 10>* Disk::CreateIO(const unsigned int RandomNum)
{
	static std::default_random_engine r{RandomNum};
	//随机数
	std::array<int, 10>* IO = new std::array<int, 10>;
	for (int i = 0; i < 10; i++)
	{
		(*IO)[i] = r() % MagneticTrackNum;
	}
	return IO;
}

//先来先服务FCFS算法
Status Disk::FCFS(int & SeekLength, std::array<int, 10>* IO)
{
	m_head.first = 100;
	m_head.second = Right;
	SeekLength = 0;
	//平均寻道长度
	for (int i = 0; i < 10; i++)
	{
		if (0 <= (*IO)[i] < MagneticTrackNum)
		{
			SeekLength += abs(m_head.first - (*IO)[i]);
			//计算平均寻道长度
			m_head.first = (*IO)[i];
			//磁头定位
		}
	}
	return ok;
}

//最短寻道时间优先SSTF算法
Status Disk::SSTF(int & SeekLength, std::array<int, 10>* IO)
{
	m_head.first = 100;
	m_head.second = Right;

	SeekLength = 0;
	for (int i = 0; i < 10; i++)
	{
		int min = INT_MAX;
		//距离最小值
		int* addr_m = nullptr;
		//距离最小值磁道在IO数组的位置
		for (auto j = 0; j < 10; j++)
		{
			if (0 <= (*IO)[j] < MagneticTrackNum)
				//检查磁道号的区间
			{
				if (min > abs(this->m_head.first - (*IO)[j]))
				{
					min = abs(this->m_head.first - (*IO)[j]);
					//得到最小距离
					addr_m = &(*IO)[j];
				}
			}
		}
		SeekLength += min;
		//计算平均寻道长度
		this->m_head.first = *addr_m;
		//改变磁头位置
		*addr_m += MagneticTrackNum;
		//修改最小距离的磁道数
	}
	return ok;
}

//检查是否有IO任务在dirction一侧
Status Disk::IfInThisSize(std::array<int, 10>* IO, const bool direction)
{
	if (m_head.second == Left and direction == Left)
	{
		for (int i = 0; i < 10; i++)
		{
			if (0 <= (*IO)[i] and (*IO)[i] < MagneticTrackNum and  m_head.first - (*IO)[i] >= 0)
				return ok;
		}
		return wrong;
	}
	else if (m_head.second == Right and direction == Right)
	{
		for (int i = 0; i < 10; i++)
		{
			if (0 <= (*IO)[i] and (*IO)[i] < MagneticTrackNum and (*IO)[i] - m_head.first >= 0)
				return ok;
		}
		return wrong;
	}
}

//找到磁头方向距离最小的那个磁道
int Disk::FindMin(std::array<int, 10>* IO)
{
	//没有符合要求的磁道号，返回-1
	if (wrong == IfInThisSize(IO, m_head.second))
		return -1;
	int min = INT_MAX;
	int min_i = -1;
	for (int i = 0; i < 10; i++)
	{
		if (0 <= (*IO)[i] and (*IO)[i] < MagneticTrackNum and m_head.second == Left and m_head.first >= (*IO)[i])
		{
			if (min > abs(m_head.first - (*IO)[i]))
			{
				min = abs(m_head.first - (*IO)[i]);
				min_i = i;
			}
		}
		else if (0 <= (*IO)[i] and (*IO)[i] < MagneticTrackNum and m_head.second == Right and m_head.first <= (*IO)[i])
		{
			if (min > abs(m_head.first - (*IO)[i]))
			{
				min = abs(m_head.first - (*IO)[i]);
				min_i = i;
			}
		}
	}
	return min_i;
}

//查看IO任务集是否都已访问
Status Disk::IfIOFinish(std::array<int, 10>* IO)
{
	for (int i = 0; i < 10; i++)
	{
		if (0 <= (*IO)[i] and (*IO)[i] < MagneticTrackNum)
			return false;
	}
	return true;
}

//扫描SCAN算法
Status Disk::SCAN(int & SeekLength, std::array<int, 10>* IO)
{
	m_head.first = 100;
	m_head.second = Right;
	//平均寻道长度
	SeekLength = 0;
	//定义距离最小值min
	int min_i = INT_MAX;
	while(false == IfIOFinish(IO))
	{
		//磁头方向向左
		if (m_head.second == Left)
		{
			if (true == this->IfInThisSize(IO, Left))
				//还有IO任务在左边
			{
				min_i = FindMin(IO);
				if (-1 != min_i)
				{
					SeekLength += abs(m_head.first - (*IO)[min_i]);
					m_head.first = (*IO)[min_i];
					(*IO)[min_i] += MagneticTrackNum;
				}
			}
			//说明没有IO任务在左侧
			else
			{
				m_head.second = Right;
			}
		}
		//磁头方向向右
		else
		{
			if (true == IfInThisSize(IO, Right))
			{
				min_i = FindMin(IO);
				if (-1 != min_i)
				{
					SeekLength += abs(m_head.first - (*IO)[min_i]);
					m_head.first = (*IO)[min_i];
					(*IO)[min_i] += MagneticTrackNum;
				}
			}
			else
			{
				m_head.second = Left;
			}
		}
	}
	return ok;
}

//循环扫描CSCAN算法
Status Disk::CSCAN(int & SeekLength, std::array<int, 10>* IO)
{
	m_head.first = 100;
	m_head.second = Right;
	SeekLength = 0;
	//只允许磁头自里向外，Right方向
	int* left_mag = &(*IO)[0];
	int min_i = INT_MAX;
	//找到最左边的磁道号
	for (int i = 0; i < 10; i++)
	{
		if (*left_mag > (*IO)[i])
		{
			left_mag = &(*IO)[i];
		}
	}
	while(false == IfIOFinish(IO))
	{
		if (true == IfInThisSize(IO, Right) and m_head.second == Right)
		{
			min_i = FindMin(IO);
			if (-1 != min_i)
			{
				SeekLength += abs(m_head.first - (*IO)[min_i]);
				m_head.first = (*IO)[min_i];
				(*IO)[min_i] += MagneticTrackNum;
			}
		}
		else
		{
			if (0 <= (*left_mag) < MagneticTrackNum and m_head.second == Right)
			{
				SeekLength += abs(m_head.first - *left_mag);
				m_head.first = *left_mag;
				(*left_mag) += MagneticTrackNum;
			}
		}
	}
	return ok;
}

