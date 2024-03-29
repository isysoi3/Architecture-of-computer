#include <mpi.h>
#include <iostream>
#include "Process.h"

class Program
{
public:
	static void Main()
	{
		auto process = MPI::Process();

		int dataLength = 100;
		double* data = new double[dataLength];

		if (process.IsMaster())
		{
			Fill(data, dataLength);
		}

		MPI_Bcast(           // 1: ��������� ������ ���� ���������.
			data,            // ��������� �� ������ ������.      
			dataLength,      // ����� ������� ������.
			MPI_DOUBLE,      // ��� ������.
			MPI::MasterRank, // ���� �������� � ��������� ������.
			MPI_COMM_WORLD); // ������� ������ ���������.

		std::cout << "#" << process.Rank() << " received double " << data[process.Rank()] << " at data[" << process.Rank() << "]" << std::endl;
	}

private:
	static void Fill(double* data, int length)
	{
		for (auto i = 0; i < length; ++i)
		{
			data[i] = i + 0.25;
		}
	}
};

void main()
{
	Program::Main();
}