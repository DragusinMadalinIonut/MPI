#include <iostream>
#include <mpi.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int main(int argc, char** argv)
{
	int my_id;
	int p,i;

	MPI_Status stare;
	char mesaj[100];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	if (p < 3) {
		if (my_id == 0)
			printf("Sunt necesare cel putin 3 procesoare!\n");
		exit(0);
	}
	if (my_id == 0)
	{
		sprintf_s(mesaj, "mesaj de la %d", my_id);
		for (i = 1; i <= p - 1; i++)
		{
			MPI_Send(mesaj, strlen(mesaj) + 1, MPI_CHAR, i, 99, MPI_COMM_WORLD);
		}
		printf("%d: am trimis: %s\n", my_id, mesaj);
	}

	else //if (my_id == 1 || my_id == p - 1)
	{
		MPI_Recv(mesaj, 50, MPI_CHAR, 0, 99, MPI_COMM_WORLD, &stare);
		printf("%d: am primit: %s\n", my_id, mesaj);
	}
	MPI_Finalize();
	return 0;
}