#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

double f(double x) {
    return x * x;
}

double integrate(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.0;
    int i;
    for (i = 0; i < n; i++) {
        double x = a + (i + 0.5) * h;
        sum += f(x);
    }
    return sum * h;
}

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double a = 0.0;
    double b = 1.0;
    int n = 1000000;
    double result = 0.0;
    double local_result = integrate(a + rank * (b - a) / size, a + (rank + 1) * (b - a) / size, n / size);
    MPI_Reduce(&local_result, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        printf("Result: %f\n", result);
    }
    MPI_Finalize();
    return 0;
}
