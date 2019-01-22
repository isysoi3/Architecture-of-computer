#include <mpi.h>
#include <iostream>
#include "Process.h"
#include "Data.h"

class Program
{
public:
	static void Main()
	{
		auto process = MPI::Process();

		if (process.IsMaster())
		{
			SendData();
		}
		else if (process.Rank() == 1)
		{
			ReceiveData();
		}
	}

private:
	static void SendData()
	{
		int dataLength = 90;
		double* data = new double[dataLength];
		Fill(data, dataLength);
		MPI::Send(data, dataLength, 1, 0);
		delete[] data;
	}

	static void Fill(double* data, int length)
	{
		for (auto i = 0; i < length; ++i)
		{
			data[i] = i + 0.25;
		}
	}

	static void ReceiveData()
	{
		MPI_Status status;
		MPI_Probe(           // 1. �������� ���������� � ���������, �� �� ������ ��� �� ������:
			MPI::MasterRank, // ���� ��������-�����������.
			0,               // ���.
			MPI_COMM_WORLD,  // ������� ������.
			&status);        // ���������.

		int dataLength;
		MPI_Get_count(       // 2. �������� ����� ������.
			&status,         // ���������� �� Probe.
			MPI_DOUBLE,      // ��������� ��� ������.
			&dataLength);    // ���������.               

		auto data = MPI::ReceiveDoubleArray(dataLength, MPI::MasterRank, 0); // 3. ���������� ����� � ������� Receive.
		std::cout << "Received double[" << dataLength << "], last element: " << data[dataLength - 1] << std::endl;
		delete[] data;
	}
};

void main()
{
	Program::Main();
}