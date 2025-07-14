#define _CRT_SECURE_NO_WARNINGS
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <windows.h>
#define size1 58
#define size2 145
#define size3 217
double** CreateMatrix(int m, int n)
{
	double** a = (double**)malloc(m * sizeof(double*));
	for (int i = 0; i < m; i++)
		a[i] = (double*)malloc(n * sizeof(double));
	return a;
}
void DeleteMatrix(double** a, int m, int n)
{
	for (int i = 0; i < m; i++)
		free(a[i]);
	free(a);
}
void FillMatrix(double** a, int m, int n)
{
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
		{
			//a[i][j] = rand() % 9900 + 100;
			//a[i][j] = 3;
			a[i][j] = ((double)rand() / RAND_MAX) * 9900.0 + 100.0;
		}
}
void FillMatrixOne(double** a, int m, int n)
{
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
		{
			a[i][j] = 1;
		}
}
void PrintMatrixToFile(FILE* f, char* title, double** a, int m, int n)
{
	fprintf(f, title);
	fprintf(f, "\n");
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
			fprintf(f, "%.3f\t", a[i][j]);
		fprintf(f, "\n");
	}
	fprintf(f, "\n");
}
// c = a * b, a - [m][n], b - [n][k], c - [m][k]
double** MultMatrix(double** c, double** a, double** b, int m, int n, int k)
{
	for (int i = 0; i < m; i++)
		for (int j = 0; j < k; j++)
		{
			c[i][j] = 0;
			for (int l = 0; l < n; l++)
				c[i][j] = c[i][j] + a[i][l] * b[l][j];
		}
	return c;
}
int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	MPI_Init(NULL, NULL);
	LARGE_INTEGER frequency, start, end;
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (size != 9)
	{
		printf("This program requires 9 processes !\n");
		fflush(stdout);
		MPI_Abort(MPI_COMM_WORLD, 1);
	}
	int i, j, k;
	int shift;
	int out;
	char SubMatrix[32];
	char fileName[32];
	if (rank == 0)
	{
		double** A = CreateMatrix(size1, size2);
		double** B = CreateMatrix(size2, size3);
		double** C = CreateMatrix(size1, size3);
		sprintf(fileName, "Results_0.txt");
		FILE* file = fopen(fileName, "w");
		if (file == NULL)
		{
			printf("\nError opening file. Check the path and permissions !\n");
			return 1;
		}
		printf("Output the results to the screen and to the file? 1 - yes, else - no : ");
		fflush(stdout);
		scanf("%d", &out);
		for (i = 1; i <= 8; i++)
			MPI_Send(&out, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		// random input matrix A
		FillMatrix(A, size1, size2);
		PrintMatrixToFile(file, (char*)"Matrix A = ", A, size1, size2);
		// random input matrix B
		FillMatrix(B, size2, size3);
		PrintMatrixToFile(file, (char*)"Matrix B = ", B, size2, size3);
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&start);
		// розсилання підматриць А і В
		int A_rows;
		j = 0;
		int B_columns;
		int B_submatrix;
		shift = 0;
		for (i = 1; i <= 8; i++)
		{
			if (i <= 2)
				A_rows = 8;
			else
				A_rows = 7;
			if (out == 1)
			{
				printf("I am 0 process. Send SubMatrix A%d in size %d x 248 to % d process\n", i, A_rows, i);
				fflush(stdout);
			}
			MPI_Send(&A_rows, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			for (k = 0; k < A_rows; k++, j++)
				MPI_Send(A[j], size2, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
			if (i <= 1)
				B_columns = 28;
			else
				B_columns = 27;
			B_submatrix = i;
			if (out == 1)
			{
				printf("I am 0 process. Send SubMatrix B%d in size 248 x %d to % d process\n", B_submatrix, B_columns, i);
				fflush(stdout);
			}
			MPI_Send(&B_submatrix, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&B_columns, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			for (k = 0; k < size2; k++)
				MPI_Ssend((B[k] + shift), B_columns, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
			shift += B_columns;
		}
		// отримання частини матриці С після першого кроку
		j = 0;
		shift = 0;
		for (i = 1; i <= 8; i++)
		{
			MPI_Recv(&A_rows, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&B_columns, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			for (k = 0; k < A_rows; k++, j++)
				MPI_Recv((C[j] + shift), B_columns, MPI_DOUBLE, i, 0,
					MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			shift += B_columns;
			if (out == 1)
			{
				printf("I am 0 process. Received SubMatrix C%d%d from %d process\n", i, i, i);
				fflush(stdout);
			}
		}
		// буфер для отримання смуги матриці В від 1 процесу
		double** b_temp = CreateMatrix(size2, 28);
		for (i = 2; i <= 8; i++)
		{
			// отримання смуги матриці В від 3-го процесу
			MPI_Recv(&B_submatrix, 1, MPI_INT, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&B_columns, 1, MPI_INT, 3, 0, MPI_COMM_WORLD,
				MPI_STATUS_IGNORE);
			for (j = 0; j < size2; j++)
				MPI_Recv(b_temp[j], B_columns, MPI_DOUBLE, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if (out == 1)
			{
				printf("I am 0 process. Received SubMatrix B%d from %d process\n", B_submatrix, 1);
				fflush(stdout);
				printf("I am 0 process. Send SubMatrix B%d to %d process\n", B_submatrix, 2);
				fflush(stdout);
			}
			// пересилання смуги матриці В 5-му процесу
			MPI_Send(&B_submatrix, 1, MPI_INT, 5, 0, MPI_COMM_WORLD);
			MPI_Send(&B_columns, 1, MPI_INT, 5, 0, MPI_COMM_WORLD);
			for (j = 0; j < size2; j++)
				MPI_Send(b_temp[j], B_columns, MPI_DOUBLE, 5, 0,
					MPI_COMM_WORLD);
			// отримання частин матриці С після і-го кроку
			j = 0;
			for (int ii = 1; ii <= 8; ii++)
			{
				MPI_Recv(&A_rows, 1, MPI_INT, ii, 0, MPI_COMM_WORLD,
					MPI_STATUS_IGNORE);
				MPI_Recv(&B_columns, 1, MPI_INT, ii, 0, MPI_COMM_WORLD,
					MPI_STATUS_IGNORE);
				MPI_Recv(&B_submatrix, 1, MPI_INT, ii, 0, MPI_COMM_WORLD,
					MPI_STATUS_IGNORE);
				if (B_submatrix <= 3)
					shift = (B_submatrix - 1) * B_columns;
				else
					shift = 54 + (B_submatrix - 4) * B_columns;
				for (k = 0; k < A_rows; k++, j++)
					MPI_Recv((C[j] + shift), B_columns, MPI_DOUBLE, ii, 0,
						MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				if (out == 1)
				{
					printf("I am 0 process. Received SubMatrix C%d%d from % d process\n", ii, B_submatrix, ii);
					fflush(stdout);
				}
			}
		}
		QueryPerformanceCounter(&end);
		double elapsedTime = static_cast<double>(end.QuadPart - start.QuadPart) /
			frequency.QuadPart * 1000;
		printf("\nElapsed Time: %f milliseconds\n\n", elapsedTime);
		PrintMatrixToFile(file, (char*)"Matrix C = ", C, size1, size3);
		DeleteMatrix(A, size1, size2);
		DeleteMatrix(B, size2, size3);
		DeleteMatrix(C, size1, size3);
		DeleteMatrix(b_temp, size2, 28);
		fclose(file);
	}
	else
	{
		sprintf(fileName, "Results_%d.txt", rank);
		FILE* file = fopen(fileName, "w");
		if (file == NULL)
		{
			printf("Error opening file. Check the path and permissions !\n");
			return 1;
		}
		MPI_Recv(&out, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		// отримання поточним процесом смуги матриці А
		int A_rows;
		MPI_Recv(&A_rows, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		double** a = CreateMatrix(A_rows, size2);
		for (i = 0; i < A_rows; i++)
			MPI_Recv(a[i], size2, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,
				MPI_STATUS_IGNORE);
		if (out == 1)
		{
			printf("I am %d process. Received SubMatrix A%d in size %d x 248 from % d process\n", rank, rank, A_rows, 0);
			fflush(stdout);
			sprintf(SubMatrix, "SubMatrix A%d = ", rank);
			PrintMatrixToFile(file, SubMatrix, a, A_rows, size2);
		}
		// отримання поточним процесом смуги матриці B
		double** b = CreateMatrix(size2, 28);
		int B_columns, B_submatrix;
		MPI_Recv(&B_submatrix, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
			MPI_STATUS_IGNORE);
		MPI_Recv(&B_columns, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
			MPI_STATUS_IGNORE);
		for (i = 0; i < size2; i++)
			MPI_Recv(b[i], B_columns, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,
				MPI_STATUS_IGNORE);
		if (out == 1)
		{
			printf("I am %d process. Received SubMatrix B%d in size 248 x %d from % d process\n", rank, B_submatrix, B_columns, 0);
			fflush(stdout);
			sprintf(SubMatrix, "SubMatrix B%d = ", B_submatrix);
			PrintMatrixToFile(file, SubMatrix, b, size2, B_columns);
		}
		double** c = CreateMatrix(A_rows, 28);
		// множення смуги матриці А на смугу матриці В
		MultMatrix(c, a, b, A_rows, size2, B_columns);
		if (out == 1)
		{
			printf("I am %d process. Multiplied SubMatrix A%d by B%d = SubMatrix C % d % d \n", rank, rank, B_submatrix, rank, B_submatrix);
			fflush(stdout);
			sprintf(SubMatrix, "SubMatrix C%d%d = A%d x B%d ", rank,
				B_submatrix, rank, B_submatrix);
			PrintMatrixToFile(file, SubMatrix, c, A_rows, B_columns);
			printf("I am %d process. Send SubMatrix C%d%d to %d process\n",
				rank, rank, B_submatrix, 0);
			fflush(stdout);
		}
		// надсилання частини матриці С після першого кроку
		MPI_Send(&A_rows, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Send(&B_columns, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		for (i = 0; i < A_rows; i++)
			MPI_Ssend(c[i], B_columns, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
		int from, to;
		switch (rank)
		{
		case 1: from = 2; to = 7; break;
		case 2: from = 6; to = 1; break;
		case 3: from = 4; to = 0; break;
		case 4: from = 8; to = 3; break;
		case 5: from = 0; to = 6; break;
		case 6: from = 5; to = 2; break;
		case 7: from = 1; to = 8; break;
		case 8: from = 7; to = 4; break;
		}
		for (i = 2; i <= 8; i++)
		{
			// пересилання смуги матриці В від поточного процесу до процесу з рангом to
			if (out == 1)
			{
				printf("I am %d process. Send SubMatrix B%d to %d process\n",
					rank, B_submatrix, to);
				fflush(stdout);
			}
			MPI_Send(&B_submatrix, 1, MPI_INT, to, 0, MPI_COMM_WORLD);
			MPI_Send(&B_columns, 1, MPI_INT, to, 0, MPI_COMM_WORLD);
			for (j = 0; j < size2; j++)
				MPI_Send(b[j], B_columns, MPI_DOUBLE, to, 0, MPI_COMM_WORLD);
			// отримання смуги матриці В від процесу з рангом from поточним процесом
			MPI_Recv(&B_submatrix, 1, MPI_INT, from, 0, MPI_COMM_WORLD,
				MPI_STATUS_IGNORE);
			MPI_Recv(&B_columns, 1, MPI_INT, from, 0, MPI_COMM_WORLD,
				MPI_STATUS_IGNORE);
			for (j = 0; j < size2; j++)
				MPI_Recv(b[j], B_columns, MPI_DOUBLE, from, 0,
					MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if (out == 1)
			{
				printf("I am %d process. Received SubMatrix B%d from %d process\n", rank, B_submatrix, from);
				fflush(stdout);
				sprintf(SubMatrix, "SubMatrix B%d = ", B_submatrix);
				PrintMatrixToFile(file, SubMatrix, b, size2, B_columns);
			}
			// множення смуги матриці А на смугу матриці В
			MultMatrix(c, a, b, A_rows, size2, B_columns);
			if (out == 1)
			{
				printf("I am %d process. Multiplied SubMatrix A%d by B%d = SubMatrix C % d % d \n", rank, rank, B_submatrix, rank, B_submatrix);
				fflush(stdout);
				sprintf(SubMatrix, "SubMatrix C%d%d = A%d x B%d ", rank,
					B_submatrix, rank, B_submatrix);
				PrintMatrixToFile(file, SubMatrix, c, A_rows, B_columns);
			}
			// надсилання частини матриці С після і-го кроку
			MPI_Send(&A_rows, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			MPI_Send(&B_columns, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			MPI_Send(&B_submatrix, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			for (j = 0; j < A_rows; j++)
				MPI_Send(c[j], B_columns, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
			if (out == 1)
			{
				printf("I am %d process. Sent SubMatrix C%d%d to %d process\n", rank, rank, B_submatrix, 0);
				fflush(stdout);
			}
		}
		DeleteMatrix(a, A_rows, size2);
		DeleteMatrix(b, size2, 28);
		DeleteMatrix(c, A_rows, 28);
		fclose(file);
	}
	MPI_Finalize();
	return 0;
}