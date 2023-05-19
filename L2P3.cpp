#include <stdio.h>
#include "mpi.h";
#include <winsock2.h>

#define MAXPROC 8
#define NAMELEN 80
#define LENGTH 8

void main(int argc, char* argv[]) {
	int i, j, np, me, count;
	const int nametag = 42;
	const int datatag = 43;
	const int root = 0;
	MPI_Status status;

	char myname[NAMELEN];
	char hostname[MAXPROC][NAMELEN];

	int x[LENGTH][LENGTH];
		int y[LENGTH];
		int* sendcount, * displs; 
		MPI_Init(&argc, &argv);
		MPI_Comm_size(MPI_COMM_WORLD, &np);
		MPI_Comm_rank(MPI_COMM_WORLD, &me);
		gethostname(myname, NAMELEN);

		sendcount = (int*)malloc(LENGTH * sizeof(int));
		displs = (int*)malloc(LENGTH * sizeof(int));
	
		for (i = 0; i < LENGTH; i++) {
			sendcount[i] = i + 1;
			displs[i] = i * LENGTH;
		}
		if (me == 0) {
			if (np != LENGTH) {
				printf("Trebuie sa utilizati %d procese\n", LENGTH);
				MPI_Finalize();
				exit(0);
			}

			for (i = 0; i < LENGTH; i++) {
				for (j = 0; j < LENGTH; j++)
					x[i][j] = i * LENGTH + j;
			}

			printf("Matricea initiala este\n");
			for (i = 0; i < LENGTH; i++) {
				for (j = 0; j < LENGTH; j++)
					printf("%4d ", x[i][j]);
				printf("\n");
			}
			printf("\nProcesul %d de pe masina %s distribuie matricea la toate celelalte % d procese\n\n", me, myname, np);

			MPI_Scatterv(&x, sendcount, displs, MPI_INT, &y, LENGTH,
				MPI_INT, root, MPI_COMM_WORLD);

			for (i = 0; i < sendcount[me]; i++)
				printf("%4d", y[i]);
			printf(" ");
			printf(" de la procesul %d de pe statia %s\n", me,
				myname);

			for (i = 1; i < np; i++) {
				MPI_Recv(&hostname[i], NAMELEN, MPI_CHAR, i, nametag, MPI_COMM_WORLD, &status);
				MPI_Recv(&y, LENGTH, MPI_INT , i, datatag, MPI_COMM_WORLD, &status);
				MPI_Get_count(&status, MPI_INT, &count);
				for (j = 0; j < count; j++)
					printf(" %4d", y[j]);
				for (j = 0; j < LENGTH - count; j++)
					printf("     ");
					printf(" de la procesul %d de pe calculatorul %s\n", i, hostname[i]);
			}
		}
		else {
			if (np != LENGTH) { MPI_Finalize(); exit(0); }
			MPI_Scatterv(&x, sendcount, displs, MPI_INT, &y, LENGTH, MPI_INT, root, MPI_COMM_WORLD);
			MPI_Send(&myname, NAMELEN, MPI_CHAR , 0, nametag, MPI_COMM_WORLD);
			MPI_Send(&y, sendcount[me], MPI_INT, 0, datatag, MPI_COMM_WORLD);
		}
		MPI_Finalize();

}
