// lab1_1_a.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include <mpi.h>
#include "Process.h"
#include <iostream>

class Program
{
public:
	static void Main1()
	{
		auto process = MPI::Process();

		double* vector1 = nullptr;
		double* vector2 = nullptr;
		auto sliceSize = 2;
		auto dataLength = process.ProcessCount() * sliceSize;

		if (process.IsMaster())
		{
			vector1 = new double[dataLength];
			Fill(vector1, dataLength);
			std::cout << "Vector 1: \n";
			Print(vector1, dataLength);

			vector2 = new double[dataLength];
			Fill(vector2, dataLength);
			std::cout << "Vector 2: \n";
			Print(vector2, dataLength);
		}

		auto buffer1 = new double[sliceSize];
		auto buffer2 = new double[sliceSize];

		MPI_Scatter(		   // 1: ��������� ������ �� ������ ����� � ���������� ������� �������� ���� �����:
			vector1, 			   // �������� ������ (��� ������-��������), ��� ��������� ��������� �� ������������.
			sliceSize, 		   // ������ ����� (��� ������-��������), ��� ��������� ��������� �� ������������.
			MPI_DOUBLE, 	   // ��� ������ (��� ������-��������), ��� ��������� ��������� �� ������������.
			buffer1, 		   // ������, ���� ����� ������������ �����.
			sliceSize, 		   // ������ �����.
			MPI_DOUBLE, 	   // ��� ������.
			MPI::MasterRank,   // ���� ���������� ��������.
			MPI_COMM_WORLD);   // ������� ������.

		MPI_Scatter(		   // 1: ��������� ������ �� ������ ����� � ���������� ������� �������� ���� �����:
			vector2, 			   // �������� ������ (��� ������-��������), ��� ��������� ��������� �� ������������.
			sliceSize, 		   // ������ ����� (��� ������-��������), ��� ��������� ��������� �� ������������.
			MPI_DOUBLE, 	   // ��� ������ (��� ������-��������), ��� ��������� ��������� �� ������������.
			buffer2, 		   // ������, ���� ����� ������������ �����.
			sliceSize, 		   // ������ �����.
			MPI_DOUBLE, 	   // ��� ������.
			MPI::MasterRank,   // ���� ���������� ��������.
			MPI_COMM_WORLD);   // ������� ������.


		ChangeData(buffer1, buffer2, sliceSize);

		MPI_Gather(            // 3. �������� ����� � ������� ��������:
			buffer1, 		   // �����.
			sliceSize, 		   // ������ �����.
			MPI_DOUBLE, 	   // ��� ������.
			vector1, 			   // ������, � ������� ����� ����������� ����� (��� ������-��������), ��� ��������� ��������� �� ������������.
			sliceSize, 		   // ������ ����� (��� ������-��������), ��� ��������� ��������� �� ������������.
			MPI_DOUBLE, 	   // ��� ������ (��� ������-��������), ��� ��������� ��������� �� ������������.
			MPI::MasterRank,   // ���� ����������� ��������.
			MPI_COMM_WORLD);   // ������� ������.

		if (process.IsMaster())
		{
			std::cout << "The sum vectors:" << std::endl;
			Print(vector1, dataLength);
			delete[] vector2;
			delete[] vector1;
			delete[] buffer2;
			delete[] buffer1;

		}
	}

	static void Main2() {

		auto process = MPI::Process();

		int dataLength = 15;
		double* vector1 = nullptr;
		double* vector2 = nullptr;
		double* result = nullptr;

		int* sendcount = new int[process.ProcessCount()];
		int* shifts = new int[process.ProcessCount()];


		sendcount[0] = (dataLength / process.ProcessCount()) + ((dataLength%process.ProcessCount()) ? 1 : 0);
		shifts[0] = 0;
		for (int i = 1; i < process.ProcessCount(); ++i)
		{
			sendcount[i] = (dataLength / process.ProcessCount()) + ((dataLength%process.ProcessCount() - i > 0) ? 1 : 0);
			shifts[i] = shifts[i - 1] + sendcount[i - 1];
		}

		int k = sendcount[process.Rank()];

		if (process.IsMaster())
		{
			vector1 = new double[dataLength];
			Fill(vector1, dataLength);
			std::cout << "Vector 1: \n";
			Print(vector1, dataLength);

			vector2 = new double[dataLength];
			Fill(vector2, dataLength);
			std::cout << "Vector 2: \n";
			Print(vector2, dataLength);
			result = new double[dataLength];

		}

		double* getbuff_a = new double[k];
		double* getbuff_b = new double[k];

		MPI_Scatterv(		// ��������� ������ �� �������� (!) ����� � ���������� ������� �������� ���� �����:
			vector1,		// �������� ������ (��� ������-��������), ��� ��������� ��������� �� ������������.
			sendcount,		// ������ ���� �������� (��� ������-��������), ��� ��������� ��������� �� ������������.
			shifts,			// ������ ��������� �������� �������� (��� ������-��������), ��� ��������� ��������� �� ������������.
			MPI_DOUBLE,		// ��� ������ (��� ������-��������), ��� ��������� ��������� �� ������������.
			getbuff_a,		// ������, ���� ����� ������������ �����.
			k,				// ������ �����.
			MPI_DOUBLE,		// ��� ������
			MPI::MasterRank,// ���� ���������� ��������.
			MPI_COMM_WORLD);// ������� ������.
		MPI_Scatterv(vector2, sendcount, shifts, MPI_DOUBLE, getbuff_b, k, MPI_DOUBLE, MPI::MasterRank, MPI_COMM_WORLD);


		double* send_buf = new double[k];
		for (int i = 0; i < k; ++i)
			send_buf[i] = getbuff_a[i] + getbuff_b[i];


		MPI_Gatherv(    //  �������� ����� � ������� ��������:
			send_buf,    // �����.
			k,          // ������ �����. 	 
			MPI_DOUBLE,  // ��� ������.
			result,       // ������, � ������� ����� ����������� ����� (��� ������-��������), ��� ��������� ��������� �� ������������.
			sendcount, // ������ ���� �������� (��� ������-��������), ��� ��������� ��������� �� ������������.
			shifts,     // ������ ��������� �������� �������� (��� ������-��������), ��� ��������� ��������� �� ������������.
			MPI_DOUBLE,   // ��� ������ (��� ������-��������), ��� ��������� ��������� �� ������������.
			MPI::MasterRank,   // ���� ����������� ��������.
			MPI_COMM_WORLD);  // ������� ������.
		
		if (process.IsMaster())
		{
			std::cout << "Result: \n";
			Print(result, dataLength);
			delete[] vector1;
			delete[] vector2;
			delete[] result;
			delete[] getbuff_a;
			delete[] getbuff_b;
			delete[] send_buf;
		}

	}

private:
	static void Fill(double* data, int length)
	{
		for (auto i = 0; i < length; ++i)
		{
			//data[i] = i + 1;
			data[i] = rand() % 10 + 1;
		}
	}

	static void Print(double* array, double length)
	{
		for (int i = 0; i < length; i++)
		{
			std::cout << array[i] << " ";
		}
		std::cout << std::endl;
	}

	static void ChangeData(double* data, double* data1, int length)
	{
		for (auto i = 0; i < length; ++i)
		{
			data[i] += data1[i];
		}
	}
};

void main()
{
	//Program::Main1();
	Program::Main2();

}
