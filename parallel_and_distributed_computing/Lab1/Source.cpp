#include <stdio.h>
#include <iostream>
#include "mpi.h"
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

int main(int argc, char* argv[])
{
    int ProcNum, ProcRank, RecvRank;


    MPI_Status Status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (ProcRank == 0)
    {
        float a, b, x;
        // Дії виконуються тільки процесом з рангом 0
        cout << "\n Hello from process " << ProcRank << endl;
        cout << "\nEnter a value a: " << endl;
        cin >> a;
        cout << "\nEnter a value b: " << endl;
        cin >> b;

        for (int i = 1; i < ProcNum; i++)
        {
            MPI_Send(&a, 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&b, 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
        }

        for (int i = 1; i < ProcNum; i++)
        {

            MPI_Recv(&x, 1, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);

            MPI_Recv(&RecvRank, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);


            cout << "\n Res From 0 Proces Rank -> :" << RecvRank << " x: " << x << endl;
        }

        cout << "Good bye!" << endl;

    }
    else
    {
        float a, b, x;

        MPI_Recv(&a, 1, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);

        MPI_Recv(&b, 1, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);

        x = a + b * float(ProcRank);
        cout << "Res From Proceses Rank -> :" << ProcRank << " x:" << x << endl;

        MPI_Send(&x, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);

        MPI_Send(&ProcRank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}