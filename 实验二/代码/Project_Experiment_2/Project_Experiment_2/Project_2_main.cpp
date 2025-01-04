#include"Project_2_header.h"
int main()
{
	ProcessList* PL = new ProcessList;
	PL->ProcessScheduing();
	delete PL;
	system("pause");
}