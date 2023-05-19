#include<iostream>
#include<mpi.h>
#include<stdio.h>
#include"mpi.h"
#include<stdlib.h>
#include<winsock2.h>
#include<conio.h>
#include<time.h>


int ProcRank, ProcNum;

void PrintVector(double* pVector, int Dim)
{
	int i;
	for (i = 0; i < Dim; i++)
		printf("%7.4f ", pVector[i]);
	printf("\n");

}

void CalculareRezultat(double* pProcRanduri, double* pVector, double* pProcRezultat, int Dim, int NrRanduri)
{

	int i, j;
	for (i = 0; i < NrRanduri; i++)
	{
		pProcRezultat[i] = 0;
		for (j = 0; j < Dim; j++)
			pProcRezultat[i] += pProcRanduri[i * Dim + j] * pVector[j];

	}
}
void TestareRezultatPartiale(double* pProcRezultat, int NrRanduri)
{
	int i, j;
	for (i = 0; i < ProcNum; i++)
	{
		if (ProcRank == i)
		{
			printf("\nProc. rank =%d\n Vectorul partial rezultat: \n", ProcRank);
			PrintVector(pProcRezultat, NrRanduri);
		}
	}
}

void PrintMatrice(double* pMatrice, int RowCount, int ColCount)
{
	int i, j;
	for (i = 0; i < RowCount; i++)
	{
		for (j = 0; j < ColCount; j++)
			printf("%7.4f ", pMatrice[i * RowCount + j]);
		printf("\n");
	}
}


void TestDistributie(double* pMatrice, double* pVector, double* pProcRanduri, int Dim, int NrRanduri)
{
	if (ProcRank == 0)
	{
		printf("matricea initiala: \n");
		PrintMatrice(pMatrice, Dim, Dim);
		printf("vectorul initial: \n");
		PrintVector(pVector, Dim);
	}
	for (int i = 0; i < ProcNum; i++)
	{
		if (ProcRank == i)
		{
			printf("\nProc. Rank= %d \n", ProcRank);
			printf("Elemente Matrice:\n");
			PrintMatrice(pMatrice, Dim, Dim);
			printf("vector:\n");
			PrintVector(pVector, Dim);
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
}

void DistribuitieDate(double* pMatrice, double* pProcRanduri, double* pVector, int Dim, int NrRanduri)
{
	MPI_Bcast(pVector, Dim, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Scatter(pMatrice, NrRanduri * Dim, MPI_DOUBLE, pProcRanduri, NrRanduri * Dim, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}


void InitializareSimpla(double*& pMatrice, double*& pVector, int& Dim)
{
	int i, j;
	for (i = 0; i < Dim; i++)
	{
		pVector[i] = 1;
		for (j = 0; j < Dim; j++)
			pMatrice[i * Dim + j] = i;
	}
}


void ReplicareRezultate(double* pProcReazultat, double* pReazultat, int Dim, int NrRanduri) {
	MPI_Allgather(pProcReazultat, NrRanduri, MPI_DOUBLE, pReazultat, NrRanduri, MPI_DOUBLE, MPI_COMM_WORLD);
}


void InitializareProces(double*& pMatrice, double*& pVector, double*& pRezultat, double*& pProcRanduri, double*& pProcRezultat, int& Dim, int& NrRanduri)
{
	if (ProcRank == 0)
	{
		printf("\n introduceti dimensiunea obiectelor ");
		scanf_s("%d", &Dim);
		printf("\nDimensiunea obiectelor este %d\n", Dim);
	}
	MPI_Bcast(&Dim, 1, MPI_INT, 0, MPI_COMM_WORLD);

	NrRanduri = Dim / ProcNum;

	pVector = new double[Dim];
	pRezultat = new double[Dim];
	pProcRanduri = new double[NrRanduri * Dim];
	pProcRezultat = new double[NrRanduri];


	if (ProcRank == 0)
	{
		pMatrice = new double[Dim * Dim];
		InitializareSimpla(pMatrice, pVector, Dim);
	}
}
void FinalizareProces(double* pMatrice, double* pVector, double* pRezultat, double* pProcRanduri, double* pProcRezultat)
{
	delete[] pMatrice;
	delete[]pVector;
	delete[] pRezultat;
	delete[]pProcRanduri;
	delete[]pProcRezultat;
}

void main(int argc, char* argv[])
{
	double* pMatrice;
	double* pVector;
	double* pRezultat;
	int Dim;
	double start, stop, durata;
	
	double* pProcRanduri;
	double* pProcRezultat;
	int NrRanduri;


	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);


	InitializareProces(pMatrice, pVector, pRezultat, pProcRanduri, pProcRezultat, Dim, NrRanduri);
	start = MPI_Wtime();
	DistribuitieDate(pMatrice, pProcRanduri, pVector, Dim, NrRanduri);
	//TestDistributie(pMatrice, pVector, pProcRanduri, Dim, NrRanduri);
	CalculareRezultat(pProcRanduri, pVector, pProcRezultat, Dim, NrRanduri);
	ReplicareRezultate(pProcRezultat, pRezultat, Dim, NrRanduri);
	stop = MPI_Wtime();
	durata = stop-start;
	if (ProcRank == 0)
	printf ("timpul de lucru pt algoritmul paralel de inmultiure matrrice vector este: %f\n", durata);
	//TestareRezultatPartiale(pProcRezultat, NrRanduri);
	
	if (ProcRank == 0) {
		printf("Program pentru inmultirea paralela a matricelor cu vectori\n");
		printf("Numar de procese disponibile: %d\n", ProcNum);

	}
	printf("Numarul procesului curent %d\n", ProcRank);

	MPI_Finalize();
}