#define _CRT_SECURE_NO_WARNINGS
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
			a[i][j] = rand() % 9900 + 100;
			//a[i][j] = 3;
			//a[i][j] = rand() % 9 + 1;
			//a[i][j] = rand() % 90 + 10;
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
			fprintf(f, "%.0f\t", a[i][j]);
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
	LARGE_INTEGER frequency, start, end;
	double** A = CreateMatrix(size1, size2);
	double** B = CreateMatrix(size2, size3);
	double** C = CreateMatrix(size1, size3);
	FILE* file = fopen("Results.txt", "w");
	if (file == NULL)
	{
		printf("\nError opening file. Check the path and permissions !\n");
		return 1;
	}
	// random input matrix A
	FillMatrix(A, size1, size2);
	PrintMatrixToFile(file, (char*)"Matrix A = ", A, size1, size2);
	// random input matrix B
	FillMatrix(B, size2, size3);
	PrintMatrixToFile(file, (char*)"Matrix B = ", B, size2, size3);
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);
	MultMatrix(C, A, B, size1, size2, size3);
	QueryPerformanceCounter(&end);
	double elapsedTime = static_cast<double>(end.QuadPart - start.QuadPart) /
		frequency.QuadPart * 1000;
	printf("\nElapsed Time: %f milliseconds\n\n", elapsedTime);
	PrintMatrixToFile(file, (char*)"Matrix C = ", C, size1, size3);
	DeleteMatrix(A, size1, size2);
	DeleteMatrix(B, size2, size3);
	DeleteMatrix(C, size1, size3);
	fclose(file);
	return 0;
}