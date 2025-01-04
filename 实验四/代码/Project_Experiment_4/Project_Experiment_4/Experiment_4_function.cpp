#include"Experiment_4_header.h"
//����һ��IO����10����
std::array<int, 10>* Disk::CreateIO(const unsigned int RandomNum)
{
	static std::default_random_engine r{RandomNum};
	//�����
	std::array<int, 10>* IO = new std::array<int, 10>;
	for (int i = 0; i < 10; i++)
	{
		(*IO)[i] = r() % MagneticTrackNum;
	}
	return IO;
}

//�����ȷ���FCFS�㷨
Status Disk::FCFS(int & SeekLength, std::array<int, 10>* IO)
{
	m_head.first = 100;
	m_head.second = Right;
	SeekLength = 0;
	//ƽ��Ѱ������
	for (int i = 0; i < 10; i++)
	{
		if (0 <= (*IO)[i] < MagneticTrackNum)
		{
			SeekLength += abs(m_head.first - (*IO)[i]);
			//����ƽ��Ѱ������
			m_head.first = (*IO)[i];
			//��ͷ��λ
		}
	}
	return ok;
}

//���Ѱ��ʱ������SSTF�㷨
Status Disk::SSTF(int & SeekLength, std::array<int, 10>* IO)
{
	m_head.first = 100;
	m_head.second = Right;

	SeekLength = 0;
	for (int i = 0; i < 10; i++)
	{
		int min = INT_MAX;
		//������Сֵ
		int* addr_m = nullptr;
		//������Сֵ�ŵ���IO�����λ��
		for (auto j = 0; j < 10; j++)
		{
			if (0 <= (*IO)[j] < MagneticTrackNum)
				//���ŵ��ŵ�����
			{
				if (min > abs(this->m_head.first - (*IO)[j]))
				{
					min = abs(this->m_head.first - (*IO)[j]);
					//�õ���С����
					addr_m = &(*IO)[j];
				}
			}
		}
		SeekLength += min;
		//����ƽ��Ѱ������
		this->m_head.first = *addr_m;
		//�ı��ͷλ��
		*addr_m += MagneticTrackNum;
		//�޸���С����Ĵŵ���
	}
	return ok;
}

//����Ƿ���IO������dirctionһ��
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

//�ҵ���ͷ���������С���Ǹ��ŵ�
int Disk::FindMin(std::array<int, 10>* IO)
{
	//û�з���Ҫ��Ĵŵ��ţ�����-1
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

//�鿴IO�����Ƿ��ѷ���
Status Disk::IfIOFinish(std::array<int, 10>* IO)
{
	for (int i = 0; i < 10; i++)
	{
		if (0 <= (*IO)[i] and (*IO)[i] < MagneticTrackNum)
			return false;
	}
	return true;
}

//ɨ��SCAN�㷨
Status Disk::SCAN(int & SeekLength, std::array<int, 10>* IO)
{
	m_head.first = 100;
	m_head.second = Right;
	//ƽ��Ѱ������
	SeekLength = 0;
	//���������Сֵmin
	int min_i = INT_MAX;
	while(false == IfIOFinish(IO))
	{
		//��ͷ��������
		if (m_head.second == Left)
		{
			if (true == this->IfInThisSize(IO, Left))
				//����IO���������
			{
				min_i = FindMin(IO);
				if (-1 != min_i)
				{
					SeekLength += abs(m_head.first - (*IO)[min_i]);
					m_head.first = (*IO)[min_i];
					(*IO)[min_i] += MagneticTrackNum;
				}
			}
			//˵��û��IO���������
			else
			{
				m_head.second = Right;
			}
		}
		//��ͷ��������
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

//ѭ��ɨ��CSCAN�㷨
Status Disk::CSCAN(int & SeekLength, std::array<int, 10>* IO)
{
	m_head.first = 100;
	m_head.second = Right;
	SeekLength = 0;
	//ֻ�����ͷ�������⣬Right����
	int* left_mag = &(*IO)[0];
	int min_i = INT_MAX;
	//�ҵ�����ߵĴŵ���
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

