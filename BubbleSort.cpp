#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

void initializareAleatoare(double*& pData, int& DimDate) {
    do {
        cout << "Dimensiunea datelor ce trebuiesc sortate: ";
        cin >> DimDate;
        if (DimDate <= 0)
            cout << "Dimensiunea sa fie mai mare ca zero." << endl;                               
    } while (DimDate <= 0);
    pData = new double[DimDate];

    srand((unsigned)time(0));
    for (int i = 0; i < DimDate; i++) {
        pData[i] = double(rand()) / RAND_MAX * 1000.0;
    }
}

void initializareSimple(double*& pData, int& DimDate) {
    do {
        cout << "Introduce-ti dimensiunea datelor ce trebuies sortate: ";
        cin >> DimDate;
        if (DimDate <= 0)
            cout << "Marimea trebuie sa fie mai mare ca zero." << endl;
    } while (DimDate <= 0);

    pData = new double[DimDate];
    for (int i = 0; i < DimDate; i++) {
        pData[i] = static_cast<double>(DimDate) - i;
    }
}

void printData(double* pData, int DimDate) {
    for (int i = 0; i < DimDate; i++) {
        printf("%7.4f ", pData[i]);
    }
    printf("\n");
}

void bubbleSort(double* pData, int DimDate) {
    double temp;
    for (int i = 0; i < DimDate - 1; i++) {
        for (int j = 0; j < DimDate - i - 1; j++) {
            if (pData[j] > pData[j + 1]) {
                temp = pData[j];
                pData[j] = pData[j + 1];
                pData[j + 1] = temp;
            }
        }
    }
}


int main(int argc, char* argv[]) {
    double* pData = nullptr;
    int DimDat;
    initializareSimple(pData, DimDat);

    printf("Datele dupa sortare:\n");
  
    int alegere = 0;

    do {
        cout << "\n1. Sortare date aleatorii" << endl;
        cout << "2. Iesire" << endl;
        cout << "3. Introduce-ti datele" << endl;
        cin >> alegere;

        switch (alegere) {
        case 1: 
            initializareAleatoare(pData, DimDat);
            break;
        case 2:
                exit(0);
                break;
        }
    }
    while (alegere != 1 && alegere !=2 && alegere != 3);
    printData(pData, DimDat);
    time_t start, final;
    double durata;
    start = clock();
    bubbleSort(pData, DimDat);
    final = clock();
    durata = (final - start) / static_cast<double>(CLOCKS_PER_SEC);
    printf("\nTimpul dureaza :%f\n", durata);
    printf("Datele dupa sortare:\n");
    printData(pData, DimDat);
    delete[] pData;
    return 0;
}
