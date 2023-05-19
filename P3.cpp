#include <iostream>
#include <mpi.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int main(int argc, char** argv)
{
	int my_id;
	int p;
	int dreapta, stanga;
	MPI_Status stare;
		char msend[100], mrecv[100], buffer[1000];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	dreapta = (my_id + 1) % p;
	stanga = (my_id- 1 + p) % p;

	sprintf_s(msend, "mesaj de la %d", my_id);
	MPI_Buffer_attach(buffer, 1000);
	MPI_Bsend(msend, strlen(msend) + 1, MPI_CHAR, dreapta, 99, MPI_COMM_WORLD);
	MPI_Recv(mrecv, 100, MPI_CHAR, stanga, 99, MPI_COMM_WORLD, &stare);

	printf("%d am primit:%s\n", my_id, mrecv);

	MPI_Finalize();
	return 0;
}