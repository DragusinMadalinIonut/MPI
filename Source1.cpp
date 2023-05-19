#include <iostream>
#include <mpi.h>

using namespace std;

int main() {
    int id, p, n;
    float h, sum = 0, my_pi = 0, x;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    int option = 0;
    while (option != 3) {
        if (id == 0) {
            cout << "Please choose an option:\n";
            cout << "1. Calculate Pi\n";
            cout << "2. Change number of intervals\n";
            cout << "3. Exit\n";
            cin >> option;
            MPI_Bcast(&option, 1, MPI_INT, 0, MPI_COMM_WORLD);
        }
        else {
            MPI_Bcast(&option, 1, MPI_INT, 0, MPI_COMM_WORLD);
        }

        switch (option) {
        case 1:
            if (n > 0) {
                h = 10.0 / n;

                for (int i = id + 1; i <= n; i += p) {
                    x = (i - 0.5) * h;
                    sum += 4.0 / (1 + x * x);
                }

                MPI_Reduce(&sum, &my_pi, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

                if (id == 0) {
                    my_pi *= h;
                    cout << "Pi = " << my_pi << endl;
                }
            }
            break;

        case 2:
            if (id == 0) {
                cout << "Number of intervals = ";
                cin >> n;
                MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
            }
            else {
                MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
            }
            break;

        case 3:
            break;

        default:
            if (id == 0) {
                cout << "Invalid option.\n";
            }
            break;
        }
    }

    MPI_Finalize();
    return 0;
}
