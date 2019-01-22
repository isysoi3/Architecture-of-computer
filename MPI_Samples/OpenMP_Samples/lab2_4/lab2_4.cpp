// lab2_4.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include <omp.h>
#include <cstdlib>
#include <iostream>

using namespace std;
class Program
{
public:

	static void Main()
	{
		int n = 5;
		int m = 5;
		float **matrix = new float *[n];
		for (int i = 0; i < n; i++)
			matrix[i] = new float[m];
		// �������������� ������� ������
		// n - ����� ���������
		// m - ����� ����������
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				matrix[i][j] = rand() % 10;

		// ������� �� �����
		cout << "Matrix: " << endl;
		for (int i = 0; i < n; i++, cout << endl)
		{
			for (int j = 0; j < m; j++)
				cout << matrix[i][j] << " ";
		}
		cout << endl;

		float  tmp, xx[10000];
		int i, j, k;
		double dt, timein, timeout;
		timein = omp_get_wtime();
		int THREADS = 2;
		omp_set_num_threads(THREADS); // ����� ������� ������� � define, �� 4
		for (i = 0; i < n; i++) {
			tmp = matrix[i][i];
			for (j = n; j >= i; j--)
				matrix[i][j] /= tmp;
			   
#pragma omp parallel for private (j, k, tmp)
			for (j = i + 1; j < n; j++) {
				tmp = matrix[j][i];
				for (k = n; k >= i; k--)
					matrix[j][k] -= tmp * matrix[i][k];
			}
		}

		//�������� ���
		xx[n - 1] = matrix[n - 1][n];
		for (i = n - 2; i >= 0; i--) {
			xx[i] = matrix[i][n];
#pragma omp for private (j)
			for (j = i + 1; j < n; j++)
				xx[i] -= matrix[i][j] * xx[j];
		}


		timeout = omp_get_wtime();
		dt = timeout - timein;
		cout << "Time: " << dt << " sec" << endl;

		for (int a = 0; a < n; a++) {
			cout << floor(xx[a] * 1000) / 1000. << " ";
			cout << endl;
		}

		for (int a = 0; a < n; a++)
			delete[] matrix[a];

	}

private:

};

int main()
{
	Program::Main();
}

