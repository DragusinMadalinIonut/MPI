#include <mpi.h>
#include<stdio.h>
#include "mpi.h"
#include<winsock2.h>

#define MAXPROC 8
#define NAMELEN 80

int main(int argc, char* argv[])
{
	int i, np, me, index;
	const int tag = 42;
	const int root = 0;

	MPI_Status status;

	MPI_Request recv_req[MAXPROC];
	char myname[NAMELEN];
	char hostname[MAXPROC][NAMELEN];
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &np);

	MPI_Comm_rank(MPI_COMM_WORLD, &me);
	gethostname(myname, NAMELEN);
	if (me == 0) {
		if (np<2 || np>MAXPROC) {
			printf("Trebuie minim 2 si maxim %d procese\n", MAXPROC);
			MPI_Finalize();
			exit(0);
		}

		printf("Procesul %d de pe calculatorul %s broadcast la toate celelalte procese\n",me,myname);

		MPI_Bcast(&me, 1, MPI_INT, root, MPI_COMM_WORLD);
		printf("Receptie mesaje de la celelalte procese\n");

		for (i = 1; i < np; i++) {
			MPI_Irecv(&hostname[i], NAMELEN, MPI_CHAR, i, tag, MPI_COMM_WORLD, &recv_req[i]);

		}
		for (i = 1; i < np; i++) {
			MPI_Waitany(np - 1, &recv_req[1], &index, &status);
			printf("Mesaj primit de la procesul %d de pe calc. %s \n", status.MPI_SOURCE, hostname[status.MPI_SOURCE]);
		}
		printf("Gata\n");
	}
	else {
		int y;
		if (np<2 || np>MAXPROC) {
			MPI_Finalize();
			exit(0);
		}

		MPI_Bcast(&y, 1, MPI_INT, root, MPI_COMM_WORLD);
		MPI_Send(&myname, NAMELEN, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
	}
	MPI_Finalize();
}