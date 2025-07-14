#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <windows.h>
#include <chrono>
#include <iostream>
#include <math.h>

using namespace std;
const char* fileName = "3D_Borders.txt";
FILE* file = fopen(fileName, "w");

// counters 
int Local_alg_count = 0;
int Recursive_alg_count = 0;
int Recursive_algOp_count = 0;
int Mult_alg_count = 0;
int Mult_one_time_alg_count = 0;


int** CreateMatrix(int n)
{
	int** a = new int* [n];
	for (int i = 0; i < n; i++)
		a[i] = new int[n];
	return a;
}
int*** CreateMatrix3D(int n)
{
	int*** a = new int** [n];
	for (int i = 0; i < n; i++)
	{
		a[i] = new int* [n];
		for (int j = 0; j < n; j++)
		{
			a[i][j] = new int[n + 1];
		}
	}
	return a;
}

void DeleteMatrix(int** a, int n)
{
	for (int i = 0; i < n; i++)
		delete[] a[i];
	delete[] a;
}
void DeleteMatrix3D(int*** a, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			delete[] a[i][j];
		delete[] a[i];
	}
	delete[] a;
}


void FillMatrixA(int** a, int n)
{

	int k = n;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n-i; j++)
		{
			a[i][j] = i+j+1;
		}
		for (int j = n - i; j < n; j++) {
			a[i][j] = 0;
		}
	}
}
void FillMatrixB(int** b, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
				b[i][j] = 0;
		}
	}
	for (int j = 0; j <= n/2; j++) {
		int spaces = abs(j - n);
		for (int i = (n - spaces); i < spaces; i++) {
			b[i][j] = rand() % 9 + 1;
		}
	}
}


void InputMatrix(int** a, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			printf("Enter [%d][%d] element = ", i, j);
			scanf_s("%d", &(a[i][j]));
		}
		printf("\n");
	}
}
void PrintMatrix(int** a, int n)
{
	int i, j;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
			printf("%d\t", a[i][j]);
		printf("\n");
	}
	printf("\n");
}

int** Martix3Dto2D(int** a, int*** b, int n)
{
	int i, j;
	// Копіювання значень з тривимірної матриці b у двовимірну матрицю a
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
			a[i][j] = b[i][j][n];
	}
	return a;
}

void PrintMatrix3D(int*** a, int n)
{
	int i, j;
	// Виведення значень тривимірної матриці a
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++) {
			// Виведення елементів останнього шару тривимірної матриці
			printf("%d\t", a[i][j][n]);
		}
		printf("\n");
	}
	printf("\n");
}

void PrintMatrixToFile(FILE* f, char* title, int** a, int n)
{
	fprintf(f, title);
	fprintf(f, "\n");
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			fprintf(f, "%d\t", a[i][j]);
		fprintf(f, "\n");
	}
	fprintf(f, "\n");
}
void PrintMatrixToFile3d(FILE* f, char* title, int*** a, int n)
{
	fprintf(f, title);
	fprintf(f, "\n");
	int i, j, k;
	for (k = 0; k < n; k++) {
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {



				fprintf(f, "%3d ", a[i][j][k + 1]);

			}
			fprintf(f, "\n");
		}
		fprintf(f, "\n");
	}
}



int** MultMatrix(int** c, int** a, int** b, int n)
{

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
		{

			c[i][j] = 0;

			for (int k = 0; k < n; k++)
			{

				c[i][j] = c[i][j] + a[i][k] * b[k][j];  // 2 operations

				Mult_alg_count += 2;
			}
		}
	}
	return c;
}


int** MultMatrixOneTime(int** c, int** a, int** b, int n)
{

	int*** c3D = CreateMatrix3D(n);


	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{

			c3D[i][j][0] = 0;

			for (int k = 0; k < n; k++)
			{

				c3D[i][j][k + 1] = c3D[i][j][k] + a[i][k] * b[k][j]; // 2 operations

				Mult_one_time_alg_count += 2;
			}
		}

	PrintMatrixToFile3d(file, (char*)"Borders of MultMatrixOneTime : ", c3D, n);


	Martix3Dto2D(c, c3D, n);


	DeleteMatrix3D(c3D, n);
	return c;
}


int** MultMatrixLocalAlgorithm(int** c, int** a, int** b, int n)
{
	int*** c3D = CreateMatrix3D(n);
	int*** a3D = CreateMatrix3D(n);
	int*** b3D = CreateMatrix3D(n);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
		{
			c3D[i][j][0] = 0;
			a3D[i][j][0] = a[i][j];
			b3D[i][j][0] = b[i][j];

		}
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			for (int k = 0; k < n; k++)
			{
				c3D[i][j][k + 1] = c3D[i][j][k] + a3D[i][k][j] * b3D[k][j][i]; // 2 operations

				a3D[i][k][j + 1] = a3D[i][k][j];
				b3D[k][j][i + 1] = b3D[k][j][i];


				Local_alg_count += 2;
			}
		}
	}
	PrintMatrixToFile3d(file, (char*)"Borders of MultMatrixLocalAlgorithm :", c3D, n);

	Martix3Dto2D(c, c3D, n);

	DeleteMatrix3D(c3D, n);
	DeleteMatrix3D(a3D, n);
	DeleteMatrix3D(b3D, n);
	return c;
}


void MultMatrixRecursAalgorithm(int*** c3D, int*** a3D, int*** b3D, int& n)
{

	static int i = 0, j = 0, k = 0;

	if (i < n)
	{

		if (j < n)
		{

			if (k < n)
			{


				c3D[i][j][k + 1] = c3D[i][j][k] + a3D[i][k][j] * b3D[k][j][i]; // 2 operations


				a3D[i][k][j + 1] = a3D[i][k][j];
				b3D[k][j][i + 1] = b3D[k][j][i];

				Recursive_alg_count += 2;

				k++;



				MultMatrixRecursAalgorithm(c3D, a3D, b3D, n);
			}

			k = 0;

			j++;

			MultMatrixRecursAalgorithm(c3D, a3D, b3D, n);
		}

		j = 0;

		i++;

		MultMatrixRecursAalgorithm(c3D, a3D, b3D, n);
	}
}


void MultMatrixRecursAalgorithm_Optimized(int*** c3D, int*** a3D, int*** b3D, int& n)
{

	static int i = 0, j = 0, k = 0;


	if (i < n)
	{

		if (j < n)
		{

			if (k < n)
			{

				a3D[i][k][j + 1] = a3D[i][k][j];
				b3D[k][j][i + 1] = b3D[k][j][i];

				if ((a3D[i][k][0] != 0 && b3D[k][j][0] != 0) && (a3D[i][k][0] != 1) && (b3D[k][j][0] != 1))
				{

					c3D[i][j][k + 1] = c3D[i][j][k] + a3D[i][k][j] * b3D[k][j][i]; // 2 operations


					Recursive_algOp_count += 2;

				}
				else if ((a3D[i][k][0] == 1) && b3D[k][j][0] != 0)
				{
					c3D[i][j][k + 1] = c3D[i][j][k] + b3D[k][j][i]; // 1 operation

					Recursive_algOp_count++;
				}
				else if ((b3D[k][j][0] == 1) && a3D[i][k][0] != 0)
				{
					c3D[i][j][k + 1] = c3D[i][j][k] + a3D[i][k][j]; // 1 operation

					Recursive_algOp_count++;
				}
				else
				{
					c3D[i][j][k + 1] = c3D[i][j][k];
				}

				k++;
				MultMatrixRecursAalgorithm_Optimized(c3D, a3D, b3D, n);
			}

			k = 0;
			j++;
			MultMatrixRecursAalgorithm_Optimized(c3D, a3D, b3D, n);
		}

		j = 0;
		i++;
		MultMatrixRecursAalgorithm_Optimized(c3D, a3D, b3D, n);
	}
}


int** MultMatrixRA(int** c, int** c1, int** a, int** b, int n)
{

	int*** c3D = CreateMatrix3D(n);
	int*** a3D = CreateMatrix3D(n);
	int*** b3D = CreateMatrix3D(n);

	// Створення додаткових тривимірних матриць для зберігання проміжних результатів множення в оптимізованому алгоритмі
	int*** c3D1 = CreateMatrix3D(n);
	int*** a3D1 = CreateMatrix3D(n);
	int*** b3D1 = CreateMatrix3D(n);



	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			c3D[i][j][0] = 0;
			a3D[i][j][0] = a[i][j];
			b3D[i][j][0] = b[i][j];

			c3D1[i][j][0] = 0;
			a3D1[i][j][0] = a[i][j];
			b3D1[i][j][0] = b[i][j];


		}


	MultMatrixRecursAalgorithm(c3D, a3D, b3D, n);

	PrintMatrixToFile3d(file, (char*)"Borders of MultMatrixRecursAalgorithm : ", c3D, n);


	Martix3Dto2D(c, c3D, n);



	MultMatrixRecursAalgorithm_Optimized(c3D1, a3D1, b3D1, n);
	PrintMatrixToFile3d(file, (char*)"Borders of MultMatrixRecursAalgorithm_Optimized : ", c3D1, n);

	Martix3Dto2D(c1, c3D1, n);




	DeleteMatrix3D(c3D, n);
	DeleteMatrix3D(a3D, n);
	DeleteMatrix3D(b3D, n);

	DeleteMatrix3D(c3D1, n);
	DeleteMatrix3D(a3D1, n);
	DeleteMatrix3D(b3D1, n);


	return c;
}


int main(void)
{
	srand((unsigned int)time(NULL));
	int n;
	printf("Enter the matrix size n : ");
	scanf_s("%d", &n);

	int** A = CreateMatrix(n);
	int** B = CreateMatrix(n);
	int in;
	printf("\nSelect an input option : \n1 - manual input\nelse - random input\n Choice: ");
	scanf_s("%d", &in);
	printf("\n");
	const char* fileName = "Results.txt";
	FILE* file = fopen(fileName, "w");
	if (file == NULL)
	{
		printf("\nError opening file. Check the path and permissions !\n");
		return 1;
	}
	if (in == 1)
	{
		// manual input matrix A
		printf("Enter the elements of matrix A :\n");
		InputMatrix(A, n);
		PrintMatrixToFile(file, (char*)"Matrix A = ", A, n);
		// manual input matrix B
		printf("Enter the elements of matrix B :\n");
		InputMatrix(B, n);
		PrintMatrixToFile(file, (char*)"Matrix B = ", B, n);
	}
	else
	{
		printf("Random input... \n\n"); \

			FillMatrixA(A, n);
		PrintMatrixToFile(file, (char*)"Matrix A = ", A, n);
		printf("Matrix A :\n");
		PrintMatrix(A, n);


		FillMatrixB(B, n);
		PrintMatrixToFile(file, (char*)"Matrix B = ", B, n);
		printf("Matrix B :\n");
		PrintMatrix(B, n);
	}
	int** C = CreateMatrix(n);
	int** C1 = CreateMatrix(n);



	MultMatrix(C, A, B, n);
	printf("Matrix C = A * B :\n");
	PrintMatrixToFile(file, (char*)"Matrix C = A * B = MultMatrix = ", C, n);
	PrintMatrix(C, n);


	MultMatrixOneTime(C, A, B, n);
	printf("Matrix C = A * B, program with one-time assignment :\n");
	PrintMatrixToFile(file, (char*)"Matrix C = A * B = MultMatrixOneTime = ", C, n);
	PrintMatrix(C, n);


	MultMatrixLocalAlgorithm(C, A, B, n);
	printf("Matrix C = A * B, program with the localized algorithm:\n");
	PrintMatrixToFile(file, (char*)"Matrix C = A * B = MultMatrixLocalAlgorithm = ", C, n);
	PrintMatrix(C, n);


	MultMatrixRA(C, C1, A, B, n);
	printf("Matrix C = A * B, program with the recursive localized algorithm :\n");
	PrintMatrixToFile(file, (char*)"Matrix C = A * B = MultMatrixRecursAalgorithm =  ", C, n);
	PrintMatrix(C, n);

	printf("Matrix C = A * B, program with the recursive localized optimized algorithm :\n");
	PrintMatrixToFile(file, (char*)"Matrix C = A * B = MultMatrixRecursAalgorithm_Optimized = ", C1, n);
	PrintMatrix(C1, n);


	cout << endl << endl;

	cout << "Operations : " << endl;
	cout << "Classic mult matrix - " << Mult_alg_count << endl;
	cout << "One time assignment mult matrix - " << Mult_one_time_alg_count << endl;
	cout << "Local mult matrix - " << Local_alg_count << endl;
	cout << "Recursive mult matrix - " << Recursive_alg_count << endl;
	cout << "Recursive optimized mult matrix - " << Recursive_algOp_count << endl;

	DeleteMatrix(A, n);
	DeleteMatrix(B, n);
	DeleteMatrix(C, n);
	DeleteMatrix(C1, n);

	fclose(file);


	return 0;
}