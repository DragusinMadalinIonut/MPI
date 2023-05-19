
#include<stdio.h>
#include "mpi.h"
#include<winsock2.h>

#define MAXPROC 8
#define NAMELEN 80
#define LENGTH 24

void main(int argc, char* argv[])
{
	int i, j, np, me;
	const int nametag = 42;
	const int datatag = 43;
	const int root = 0;
	MPI_Status status;

	
	char myname[NAMELEN];
	char hostname[MAXPROC][NAMELEN];

	int x[LENGTH];
    int y[LENGTH];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	MPI_Comm_rank(MPI_COMM_WORLD, &me);

	gethostname(myname, NAMELEN);
	
		for (i = 0; i <LENGTH/np; i++)
			x[i] = (LENGTH/np) * me + i;
		if (me == 0) {
			if (np > MAXPROC || np % 2 != 0) {
				printf("Trebuie sa utilizati un numar par de procesoare(cel mult %d)\n", MAXPROC);
				MPI_Finalize();
				exit(0);
			}
			printf("Procesorul %d de pe calculatoruul radacina %scolecteaza datele de la cele %d procese\n\n", me, myname, np);
			MPI_Gather(&x, LENGTH/np, MPI_INT, &y, LENGTH/np, MPI_INT, root, MPI_COMM_WORLD);
			printf("procesul %d de pe calculatorul %s are elementele\n", me, myname);
			for (i = 0; i < LENGTH / np; i++)
				printf("%d", x[i]);
			printf("\n");
			printf("procesul %d de pe calculatorul %s are elementele\n", me, myname);
			for (i = 0; i < LENGTH; i++)
				printf("%d", y[i]);
			printf("\n\n");
		for (i = 1; i < np; i++) {
			MPI_Recv(&hostname[i], NAMELEN, MPI_CHAR, i, nametag, MPI_COMM_WORLD, &status);
			printf("Procesul %d de pe calculatorul %s are elementele", i, hostname[i]);
			for (j = 0; j < LENGTH / np; j++)
				printf("%d", y[i]);
			printf("/n");
		}
		printf("FInalizare\n");
		}
		else {
			if(np>MAXPROC || np%2 != 0) {
				MPI_Finalize();
				exit(0);
			}
		MPI_Gather(&x, LENGTH/np, MPI_INT, &y, LENGTH/np, MPI_INT, root, MPI_COMM_WORLD);
		MPI_Send(&myname, NAMELEN, MPI_CHAR, 0, nametag, MPI_COMM_WORLD);
		MPI_Send(&x, LENGTH/np, MPI_INT, 0, datatag, MPI_COMM_WORLD);
	}
	MPI_Finalize();
}