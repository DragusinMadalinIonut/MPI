#include <stdio.h>
#include <conio.h>
#include <cstdlib>
#include <time.h>
#include <chrono>

void InitializareSimpla(double* pMatrice, double* pVector, int Dim) {
	int i, j;
	for (i = 0; i < Dim; i++) {
		pVector[i] = 1;
		for (j = 0; j < Dim; j++) {
			pMatrice[i * Dim + j] = i;
		}
	}
}


void InitializareAleatoare(double* pMatrice, double* pVector, int Dim) {
	int i, j;
	srand(unsigned(clock()));
	for (i = 0; i < Dim; i++) {
		pVector[i] = rand() / double(1000);
		for (j = 0; j < Dim; j++) {
			pMatrice[i * Dim + j] = rand() / double(1000);
		}
	}
}

void InitializareProces(double* &pMatrice,
	double* &pVector,
	double* &pRezultat, int &Dim
)
{
	do {
		printf("\nIntroduceti dimensiunea obiectelor: ");
		scanf("%d", &Dim);
		printf("\nDimensiunea obiectelor este: %d", Dim);
		if (Dim <= 0) {
			printf("\nDimensiunea trebuie sa fiem ai mare decat 0!\n");
		}
	} while (Dim <= 0);

	pMatrice	= new double[Dim * Dim];
	pVector		= new double[Dim];
	pRezultat	= new double[Dim];
	
	//InitializareSimpla(pMatrice, pVector, Dim);
	InitializareAleatoare(pMatrice, pVector, Dim);
}

void PrintMatrice(double* pMatrice, int RowCount, int ColCount) {
	int i, j;
	for (i = 0; i < RowCount; i++) {
		for (j = 0; j < ColCount; j++) {
			printf("%7.4f ", pMatrice[i * RowCount + j]);
		}
		printf("\n");
	}
}

void PrintVector(double* pVector, int Dim) {
	int i;
	for (i = 0; i < Dim; i++) {
		printf("%7.4f ", pVector[i]);
	}

	printf("\n");
}

void FinalizareProces(double* pMatrice, double* pVector, double* pRezultate) {
	delete[] pMatrice;
	delete[] pVector;
	delete[] pRezultate;
}

void CalculareRezultat(double* pMatrice, double* pVector, double* pRezultat, int Dim) {
	int i, j;
	for (i = 0; i < Dim; i++) {
		pRezultat[i] = 0;
		for (j = 0; j < Dim; j++) {
			pRezultat[i] += pMatrice[i * Dim + j] * pVector[j];
		}
	}
}

void main() {
	double* pMatrice;
	double* pVector;
	double* pRezultat;
	int Dim;

	printf("Program pentru inmultirea seriala a matricelor cu vectori\n");
	
	InitializareProces(pMatrice, pVector, pRezultat, Dim);


	//printf("Matricea Initiala\n");
	//PrintMatrice(pMatrice, Dim, Dim);
	//printf("Vectorul Initial \n");
	//PrintVector(pVector, Dim);

	auto start = std::chrono::high_resolution_clock::now();
	CalculareRezultat(pMatrice, pVector, pRezultat, Dim);
	auto end = std::chrono::high_resolution_clock::now();

	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	auto elapsed_sec = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);


	//printf("\nVectorul Rezultat:\n");
	//PrintVector(pRezultat, Dim);

	//printf("Timpul de executie: %f\n", durata);

	printf("\nTime measured: %f seconds.\n", elapsed.count() * 1e-9);
	//printf("\nTime measured: %lld us.\n", elapsed_sec.count());

	FinalizareProces(pMatrice, pVector, pRezultat);
}