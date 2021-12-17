#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include "/usr/lib/x86_64-linux-gnu/openmpi/include/mpi.h"

int main(int argc, char **argv) {

    int tag = 55;
    int tag2 = 56;
    int tag3 = 57;
    int rank = 0;
    int p = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (p < 2) {
        MPI_Finalize();
    }
    int n = 7;
    int block_size = (n + 1) / p;
    double *matrix;
    double *buff;

    if (rank == 0) {
        matrix = new double[(n * (n + 1))]{1.0, 2.0, 3.0, 1.0, 3.0, 0.0, 0.0, 4.0,
                                           2.0, 3.0, 4.0, 1.0, 0.0, 0.0, 0.0, 5.0,
                                           10.0, 4.0, 5.0, 0.0, 2.0, 0.0, 0.0, 6.0,
                                           0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 7.0,
                                           0.0, 0.0, 0.0, 0.0, 3.0, 4.0, 0.0, 8.0,
                                           0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 9.0,
                                           0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 10.0,};
        std::cout << block_size << std::endl;
        for (int pr = p - 1; pr >= 0; --pr) {
            buff = new double[n * block_size];
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < block_size; ++j) {
                    buff[i * block_size + j] = matrix[i * (n + 1) + block_size * pr + j];
                }
            }

            if (pr != 0) {
                MPI_Send(buff, n * block_size, MPI_DOUBLE, pr, tag, MPI_COMM_WORLD);
            }
        }
    }
    if (rank != 0) {
        buff = new double[n * block_size];
        MPI_Recv(buff, n * block_size, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
    }

    for (int k = 0; k < block_size * (rank + 1); ++k) {
        double *l = new double[n];
        if (rank == k / block_size) {
            for (int i = k + 1; i < n; ++i) {
                l[i] = buff[i * block_size + k - block_size * rank] / buff[k * block_size + k - block_size * rank];
            }
            if (rank + 1 != p) {
                for (int pr = rank + 1; pr < p; ++pr) {
                    MPI_Send(l, n, MPI_DOUBLE, pr, tag2, MPI_COMM_WORLD);
                }
            }
        } else {
            MPI_Recv(l, n, MPI_DOUBLE, k / block_size, tag2, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
        }
        for (int i = k + 1; i < n; ++i) {
            for (int j = std::max(k + 1 - rank * block_size, 0); j < block_size; j++) {
                buff[i * block_size + j] -= l[i] * buff[k * block_size + j];
            }
        }
    }

    if (rank != 0) {
        MPI_Send(buff, n * block_size, MPI_DOUBLE, 0, tag3, MPI_COMM_WORLD);
    } else {
        for (int pr = 0; pr < p; ++pr) {
            if (pr != 0) {
                buff = new double[n * block_size];
                MPI_Recv(buff, n * block_size, MPI_DOUBLE, pr, tag3, MPI_COMM_WORLD,

                         MPI_STATUS_IGNORE);
            }
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < block_size; ++j) {
                    matrix[i * (n + 1) + block_size * pr + j] = buff[i * block_size + j];
                }
            }
        }

        double *x = new double[n];
        x[n - 1] = matrix[(n - 1) * (n + 1) + n] / matrix[(n - 1) * (n + 1) + n - 1];

        for (int i = n - 2; i >= 1; --i) {
            x[i] = matrix[i * (n + 1) + n];
            for (int j = n - 1; j >= i + 1; --j) {
                x[i] -= matrix[i * (n + 1) + j] * x[j];
            }
            x[i] /= matrix[i * (n + 1) + i];
        }
        for (int i = 0; i < n; ++i) {
            std::cout << std::setprecision(3) << std::fixed << x[i] << std::endl;
        }
        std::cout << std::endl;
    }
    
    MPI_Finalize();
}
