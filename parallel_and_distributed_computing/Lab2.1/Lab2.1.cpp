#define _CRT_SECURE_NO_WARNINGS
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <windows.h>
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
			a[i][j] = rand() % 10;
			//a[i][j] = 1;
		}
}
void InputMatrix(double** a, int m, int n)
{
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			printf("Enter [%d][%d] element = ", i, j);
			fflush(stdout);
			scanf_s("%lf", &(a[i][j]));
		}
		printf("\n");
	}
}
void PrintMatrix(double** a, int m, int n)
{
	int i, j;
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
			printf("%8.2f\t", a[i][j]);
		printf("\n");
	}
	printf("\n");
}
void PrintMatrixToFile(FILE* f, char* title, double** a, int m, int n)
{
	fprintf(f, title);
	fprintf(f, "\n");
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
			fprintf(f, "%.2f\t", a[i][j]);
		fprintf(f, "\n");
	}
	fprintf(f, "\n");
}
// c = a * b, a - mxn, b - nxk, c - mxk
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
// b = k * a
double** MultMatrixNum(double** b, double k, double** a, int m, int n)
{
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			b[i][j] = a[i][j] * k;
	return b;
}
// c = a + b
double** SumMatrix(double** c, double** a, double** b, int m, int n)
{
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
		{
			c[i][j] = a[i][j] + b[i][j];
		}
	return c;
}
// vector transposition (column-to-row conversion)
void TVector(double** b, double** a, int n)
{
	for (int i = 0; i < n; i++)
		b[0][i] = a[i][0];
}
int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	MPI_Init(NULL, NULL);
	LARGE_INTEGER frequency, start, end;
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (size < 2)
	{
		printf("This program requires at least two processes !\n");
		MPI_Abort(MPI_COMM_WORLD, 1);
	}
	int n;
	int in;
	int out;
	if (rank == 0)
	{
		printf("Enter the matrix size n : ");
		fflush(stdout);
		scanf_s("%d", &n);
		// Процес з рангом 0 відправляє n процесу з рангом 1
		MPI_Send(&n, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		const char* fileName = "Results_0.txt";
		FILE* file = fopen(fileName, "w");
		if (file == NULL)
		{
			printf("\nError opening file. Check the path and permissions !\n");
			return 1;
		}
		printf("\nSelect an input option : 1 - manual input, else - random input: ");
			fflush(stdout);
		scanf_s("%d", &in);
		printf("\n");
		MPI_Send(&in, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		double** A = CreateMatrix(n, n);
		double** b = CreateMatrix(n, 1);
		double** A1 = CreateMatrix(n, n);
		double** b1 = CreateMatrix(n, 1);
		double** c1 = CreateMatrix(n, 1);
		if (in == 1)
		{
			// manual input matrix A
			printf("Enter the elements of matrix A :\n");
			InputMatrix(A, n, n);
			PrintMatrixToFile(file, (char*)"Matrix A = ", A, n, n);
			// manual input matrix A1
			printf("Enter the elements of matrix A1 :\n");
			InputMatrix(A1, n, n);
			PrintMatrixToFile(file, (char*)"Matrix A1 = ", A1, n, n);
			// manual input vector b1
			printf("Enter the elements of vector b1 :\n");
			InputMatrix(b1, n, 1);
			PrintMatrixToFile(file, (char*)"Vector b1 = ", b1, n, 1);
			// manual input vector c1
			printf("Enter the elements of vector c1 :\n");
			InputMatrix(c1, n, 1);
			PrintMatrixToFile(file, (char*)"Vector c1 = ", c1, n, 1);
			double** A2 = CreateMatrix(n, n);
			double** B2 = CreateMatrix(n, n);
			// manual input matrix A2
			printf("Enter the elements of matrix A2 :\n");
			InputMatrix(A2, n, n);
			PrintMatrixToFile(file, (char*)"Matrix A2 = ", A2, n, n);
			// manual input matrix B2
			printf("Enter the elements of matrix B2 :\n");
			InputMatrix(B2, n, n);
			PrintMatrixToFile(file, (char*)"Matrix B2 = ", B2, n, n);
			for (int i = 0; i < n; ++i)
				MPI_Send(A2[i], n, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
			for (int i = 0; i < n; ++i)
				MPI_Send(B2[i], n, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
		}
		else
		{
			printf("Random input... \n"); \
				// random input matrix A
				FillMatrix(A, n, n);
			PrintMatrixToFile(file, (char*)"Matrix A = ", A, n, n);
			//printf("Matrix A :\n");
			//PrintMatrix(A, n, n);
			// random input matrix A1
			FillMatrix(A1, n, n);
			PrintMatrixToFile(file, (char*)"Matrix A1 = ", A1, n, n);
			//printf("Matrix A1 :\n");
			//PrintMatrix(A1, n, n);
			// random input vector b1
			FillMatrix(b1, n, 1);
			PrintMatrixToFile(file, (char*)"Vector b1 = ", b1, n, 1);
			//printf("Vector b1 :\n");
			//PrintMatrix(b1, n, 1);
			// random input vector c1
			FillMatrix(c1, n, 1);
			PrintMatrixToFile(file, (char*)"Vector c1 = ", c1, n, 1);
			//printf("Vector c1 :\n");
			//PrintMatrix(c1, n, 1);
		}
		// filling vector b (bi = 28 * i)
		for (int i = 0; i < n; i++)
			b[i][0] = 7.0 * (i + 1);
		PrintMatrixToFile(file, (char*)"Vector b = ", b, n, 1);
		//printf("Vector b:\n");
		//PrintMatrix(b, n, 1);
		double** y1 = CreateMatrix(n, 1);
		double** y2 = CreateMatrix(n, 1);
		double** Y3 = CreateMatrix(n, n);
		double** t_y1 = CreateMatrix(1, n);
		double** t_y2 = CreateMatrix(1, n);
		double** T1 = CreateMatrix(n, n);
		double** T2 = CreateMatrix(n, n);
		double** T3 = CreateMatrix(n, n);
		double** x = CreateMatrix(1, 1);
		double** t = CreateMatrix(1, n);
		printf("\nWrite intermediate results to a file? : 1 - yes, else - no :");
			fflush(stdout);
		scanf_s("%d", &out);
		MPI_Send(&out, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&start);
		if (out == 1)
		{
			// calculation vector y1 = A * b
			MultMatrix(y1, A, b, n, n, 1);
			PrintMatrixToFile(file, (char*)"Vector y1 : = A * b = ", y1, n, 1);
			// b1 = b1 + c1
			// calculation vector y2 = A1 * (b1 + c1)
			MultMatrix(y2, A1, SumMatrix(b1, b1, c1, n, 1), n, n, 1);
			PrintMatrixToFile(file, (char*)"Vector y2 = A1 * (b1 + c1) = ", y2, n, 1);
			TVector(t_y1, y1, n);
			TVector(t_y2, y2, n);
			// calculation matrix T1 = y1 * t_y2
			MultMatrix(T1, y1, t_y2, n, 1, n);
			PrintMatrixToFile(file, (char*)"Matrix y1 * t_y2 = ", T1, n, n);
			// calculation x = t_y1 * y2
				MultMatrix(x, t_y1, y2, 1, n, 1);
			for (int i = 0; i < n; ++i)
				MPI_Recv(Y3[i], n, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			//calculation matrix T3 = Y3^3
			MultMatrix(T3, Y3, MultMatrix(T3, Y3, Y3, n, n, n), n, n, n);
			PrintMatrixToFile(file, (char*)"Matrix Y3^3 = Y3*Y3*Y3 = ",
				T3, n, n);
			// calculation matrix T2 = x * Y3 = t_y1 * y2 * Y3
			MultMatrixNum(T2, x[0][0], Y3, n, n);
			PrintMatrixToFile(file, (char*)"Matrix t_y1 * y2 * Y3 = ", T2, n,
				n);
			// calculation matrix T1 = T3 + T2 + T1 = y1 * t_y2 + Y3^3 + t_y1 * y2 * Y3
			SumMatrix(T3, T3, SumMatrix(T2, T1, T2, n, n), n, n);
			PrintMatrixToFile(file, (char*)"Matrix y1 * t_y2 +Y3^3 + t_y1 * y2 * Y3 = ", T3, n, n);
			// calculation matrix t = t_y2 * T3 = t_y2 * (y1 * t_y2 +Y3^3 + t_y1 * y2 * Y3)
			MultMatrix(t, t_y2, T3, 1, n, n);
			PrintMatrixToFile(file, (char*)"t_y2 * (y1 * t_y2 +Y3^3 + t_y1 * y2 * Y3) = ", t, 1, n);
		}
		else
		{
			// calculation vector y1 = A * b
			MultMatrix(y1, A, b, n, n, 1);
			// b1 = b1 + c1
			// calculation vector y2 = A1 * (b1 + c1)
			MultMatrix(y2, A1, SumMatrix(b1, b1, c1, n, 1), n, n, 1);
			TVector(t_y1, y1, n);
			TVector(t_y2, y2, n);
			// calculation matrix T1 = y1 * t_y2
			MultMatrix(T1, y1, t_y2, n, 1, n);
			// calculation x = t_y1 * y2
			MultMatrix(x, t_y1, y2, 1, n, 1);
			for (int i = 0; i < n; ++i)
				MPI_Recv(Y3[i], n, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			//calculation matrix T3 = Y3^3
			MultMatrix(T3, Y3, MultMatrix(T3, Y3, Y3, n, n, n), n, n, n);
			// calculation matrix T2 = x * Y3 = t_y1 * y2 * Y3
			MultMatrixNum(T2, x[0][0], Y3, n, n);
			// calculation matrix T1 = T3 + T2 + T1 = y1 * t_y2 + Y3^3 + t_y1 * y2 * Y3
			SumMatrix(T3, T3, SumMatrix(T2, T1, T2, n, n), n, n);
			// calculation matrix t = t_y2 * T3 = t_y2 * (y1 * t_y2 +Y3^3 + t_y1 * y2 * Y3)
			MultMatrix(t, t_y2, T3, 1, n, n);
		}
		// calculation x = t * y1 = t_y2 * (y1 * t_y2 + Y3^3 + t_y1 * y2 * Y3)* y1
		MultMatrix(x, t, y1, 1, n, 1);
		PrintMatrixToFile(file, (char*)" x = t_y2 * (y1 * t_y2 +Y3^3 + t_y1 * y2 * Y3)* y1 = ", x, 1, 1);
		printf("\nx = ");
		PrintMatrix(x, 1, 1);
		QueryPerformanceCounter(&end);
		double elapsedTime = static_cast<double>(end.QuadPart - start.QuadPart) /
			frequency.QuadPart * 1000;
		printf("\nElapsed Time: %f milliseconds on %d process\n", elapsedTime,
			rank);
		if (n == 1)
		{
			printf("Checking the result (for n = 1)");
			double res = t_y2[0][0] * ((y1[0][0] * t_y2[0][0]) + Y3[0][0] * Y3[0][0] * Y3[0][0] + t_y1[0][0] * y2[0][0] * Y3[0][0]) * y1[0][0];
			printf("\nx = %.2lf\n", res);
		}
		DeleteMatrix(A, n, n);
		DeleteMatrix(b, n, 1);
		DeleteMatrix(y1, n, 1);
		DeleteMatrix(A1, n, n);
		DeleteMatrix(b1, n, 1);
		DeleteMatrix(c1, n, 1);
		DeleteMatrix(y2, n, 1);
		DeleteMatrix(t_y1, 1, n);
		DeleteMatrix(t_y2, 1, n);
		DeleteMatrix(T1, n, n);
		DeleteMatrix(T2, n, n);
		DeleteMatrix(T3, n, n);
		DeleteMatrix(t, 1, n);
		DeleteMatrix(x, 1, 1);
		fclose(file);
	}
	else if (rank == 1)
	{
		// Процес з рангом 1 отримує n від процесу з рангом 0
		MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		const char* fileName = "Results_1.txt";
		FILE* file = fopen(fileName, "w");
		if (file == NULL)
		{
			printf("\nError opening file. Check the path and permissions !\n");
			return 1;
		}
		double** A2 = CreateMatrix(n, n);
		double** B2 = CreateMatrix(n, n);
		double** C2 = CreateMatrix(n, n);
		double** Y3 = CreateMatrix(n, n);
		MPI_Recv(&in, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		if (in == 1)
		{
			for (int i = 0; i < n; ++i)
				MPI_Recv(A2[i], n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,
					MPI_STATUS_IGNORE);
			for (int i = 0; i < n; ++i)
				MPI_Recv(B2[i], n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,
					MPI_STATUS_IGNORE);
		}
		else
		{
			// random input matrix A2
			FillMatrix(A2, n, n);
			PrintMatrixToFile(file, (char*)"Matrix A2 = ", A2, n, n);
			//printf("Matrix A2 :\n");
			//PrintMatrix(A2, n, n);
			// random input matrix B2
			FillMatrix(B2, n, n);
			PrintMatrixToFile(file, (char*)"Matrix B2 = ", B2, n, n);
			//printf("Matrix B2 :\n");
			//PrintMatrix(B2, n, n);
		}
		// filling matrix C2 (cij = 1/(i*i*i + j*j)
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				C2[i][j] = 1 / (double)((i + 1) * (i + 1) * (i + 1) + (j + 1) * (j+ 1));
		PrintMatrixToFile(file, (char*)"Matrix C2 = ", C2, n, n);
		//printf("Matrix C2:\n");
		//PrintMatrix(C2, n, n);
		MPI_Recv(&out, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&start);
		// C2 = -1 * C2
		// B2 = B2 + C2
		// calculation matrix Y3 = A2 * B2 = A2 * (B2 - C2)
		MultMatrixNum(C2, -1, C2, n, n);
		SumMatrix(B2, B2, C2, n, n);
		MultMatrix(Y3, A2, B2, n, n, n);
		QueryPerformanceCounter(&end);
		double elapsedTime = static_cast<double>(end.QuadPart - start.QuadPart) /
			frequency.QuadPart * 1000;
		printf("\nElapsed Time: %f milliseconds on %d process\n", elapsedTime,
			rank);
		for (int i = 0; i < n; ++i)
			MPI_Send(Y3[i], n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
		if (out == 1)
		{
			PrintMatrixToFile(file, (char*)"Matrix Y3 = A2 * (B2 - C2) = ",
				Y3, n, n);
		}
		DeleteMatrix(A2, n, n);
		DeleteMatrix(B2, n, n);
		DeleteMatrix(C2, n, n);
		DeleteMatrix(Y3, n, n);
		fclose(file);
	}
	MPI_Finalize();
	return 0;
}