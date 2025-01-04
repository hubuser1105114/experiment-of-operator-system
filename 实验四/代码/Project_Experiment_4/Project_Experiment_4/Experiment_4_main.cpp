#include"Experiment_4_header.h"
int main()
{
	Disk* D = new Disk;
	std::cout << "输入一随机正数："; 
	unsigned int RandomNum = 0;
	std::cin >> RandomNum;
	auto IO = D->CreateIO(RandomNum);
	std::array<int, 10> *IO_1, *IO_2, *IO_3;
	IO_1 = new std::array<int, 10>;
	IO_2 = new std::array<int, 10>;
	IO_3 = new std::array<int, 10>;
	std::cout << "请求IO：" << std::endl;
	for (int i = 0; i < 10; i++)
	{
		std::cout << "磁道号：" << (*IO)[i] << std::endl;
		(*IO_1)[i] = (*IO)[i];
		(*IO_2)[i] = (*IO)[i];
		(*IO_3)[i] = (*IO)[i];
	}
	int SeekLength = 0;
	D->FCFS(SeekLength, IO);
	std::cout << "FCFS平均寻道长度：" << float(SeekLength) / 10 << std::endl;
	SeekLength = 0;
	D->SSTF(SeekLength, IO_1);
	std::cout << "SSTF平均寻道长度：" << float(SeekLength) / 10 << std::endl;
	SeekLength = 0;
	D->SCAN(SeekLength, IO_2);
	std::cout << "SCAN平均寻道长度：" << float(SeekLength) / 10 << std::endl;
	SeekLength = 0;
	D->CSCAN(SeekLength, IO_3);
	std::cout << "CSCAN平均寻道长度：" << float(SeekLength) / 10 << std::endl;
	delete IO;
	delete IO_1;
	delete IO_2;
	delete IO_3;
	delete D;
	system("pause");
}