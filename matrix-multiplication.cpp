#include <iostream>
#include <mpi.h>

int main() {
    int rank, size;
    int N; // Number of rows/columns in matrices A, B, and C

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Input matrix dimensions
    if (rank == 0) {
        std::cout << "Enter the dimension (N) of the matrices A, B, and C: ";
        std::cin >> N;
    }
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Divide the rows of matrix C among processes
    int local_rows = N / size;
    int remaining_rows = N % size;

    // Adjust local_rows for processes with remaining rows
    if (rank < remaining_rows) {
        local_rows++;
    }

    // Create matrices A, B, and C
    int* A = new int[N * N];
    int* B = new int[N * N];
    int* C = new int[N * N];
    int* local_A = new int[local_rows * N];
    int* local_C = new int[local_rows * N];

    // Initialize matrices A and B on the root process
    if (rank == 0) {
        std::cout << "Enter the elements of matrix A:" << std::endl;
        for (int i = 0; i < N * N; i++) {
            std::cin >> A[i];
        }

        std::cout << "Enter the elements of matrix B:" << std::endl;
        for (int i = 0; i < N * N; i++) {
            std::cin >> B[i];
        }
    }

    // Scatter matrix A to local_A
    MPI_Scatter(A, local_rows * N, MPI_INT, local_A, local_rows * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Broadcast matrix B to all processes
    MPI_Bcast(B, N * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Perform local matrix multiplication
    for (int i = 0; i < local_rows; i++) {
        for (int j = 0; j < N; j++) {
            local_C[i * N + j] = 0;
            for (int k = 0; k < N; k++) {
                local_C[i * N + j] += local_A[i * N + k] * B[k * N + j];
            }
        }
    }

    // Gather local_C from all processes to C on the root process
    MPI_Gather(local_C, local_rows * N, MPI_INT, C, local_rows * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Print the result on the root process
    if (rank == 0) {
        std::cout << "Matrix A:" << std::endl;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                std::cout << A[i * N + j] << " ";
            }
            std::cout << std::endl;
        }

        std::cout << "Matrix B:" << std::endl;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                std::cout << B[i * N + j] << " ";
            }
            std::cout << std::endl;
        }

        std::cout << "Matrix C (Result):" << std::endl;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                std::cout << C[i * N + j] << " ";
            }
            std::cout << std::endl;
        }
    }

    // Clean up dynamically allocated memory
    //so we can use it again later
    //it also takes out the user from the program
    delete[] A;
    delete[] B;
    delete[] C;
    delete[] local_A;
    delete[] local_C;

    MPI_Finalize();
    return 0;
}
