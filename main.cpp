#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <fstream>
#include "/usr/lib/x86_64-linux-gnu/openmpi/include/mpi.h"

int main(int argc, char **argv) {
    std::ofstream out("result_mpi.txt");
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
    int n = atoi(argv[3]);
//    int n;
//    std::cin >> n;
    int block_size = (n + 1) / p;
    double *matrix;
    double *buff;
    double start_time;
    double end_time;
    double receive_time = 0;
    double operation_time = 0;
    double total_start_time = MPI_Wtime();
    if (rank == 0) {
        matrix = new double[(n * (n + 1))];
        for (int i = 0; i < n * (n + 1); i++) {
//            sscanf(argv[i+4],"%lf", &matrix[i]);
            std::cin >> matrix[i];
        }
//        std::cout << block_size << std::endl;
        for (int pr = p - 1; pr >= 0; --pr) {
            buff = new double[n * block_size];
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < block_size; ++j) {
                    buff[i * block_size + j] = matrix[i * (n + 1) + block_size * pr + j];
                }
            }

            if (pr != 0) {
                start_time = MPI_Wtime();
                MPI_Send(buff, n * block_size, MPI_DOUBLE, pr, tag, MPI_COMM_WORLD);
                end_time = MPI_Wtime();
                receive_time += end_time - start_time;
            }
        }
    }
    if (rank != 0) {
        buff = new double[n * block_size];
        start_time = MPI_Wtime();
        MPI_Recv(buff, n * block_size, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        end_time = MPI_Wtime();
        receive_time += end_time - start_time;
    }

    for (int k = 0; k < block_size * (rank + 1); ++k) {
        double *l = new double[n];
        if (rank == k / block_size) {
            for (int i = k + 1; i < n; ++i) {
                l[i] = buff[i * block_size + k - block_size * rank] / buff[k * block_size + k - block_size * rank];
            }
            if (rank + 1 != p) {
                for (int pr = rank + 1; pr < p; ++pr) {
                    start_time = MPI_Wtime();
                    MPI_Send(l, n, MPI_DOUBLE, pr, tag2, MPI_COMM_WORLD);
                    end_time = MPI_Wtime();
                    receive_time += end_time - start_time;
                }
            }
        } else {
            start_time = MPI_Wtime();
            MPI_Recv(l, n, MPI_DOUBLE, k / block_size, tag2, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
            end_time = MPI_Wtime();
            receive_time += end_time - start_time;
        }
        start_time = MPI_Wtime();
        for (int i = k + 1; i < n; ++i) {
            for (int j = std::max(k + 1 - rank * block_size, 0); j < block_size; j++) {
                buff[i * block_size + j] -= l[i] * buff[k * block_size + j];
            }
        }
        end_time = MPI_Wtime();
        operation_time += end_time - start_time;
    }

    if (rank != 0) {
        start_time = MPI_Wtime();
        MPI_Send(buff, n * block_size, MPI_DOUBLE, 0, tag3, MPI_COMM_WORLD);
        end_time = MPI_Wtime();
        receive_time += end_time - start_time;
    } else {
        for (int pr = 0; pr < p; ++pr) {
            if (pr != 0) {
                start_time = MPI_Wtime();
                buff = new double[n * block_size];
                MPI_Recv(buff, n * block_size, MPI_DOUBLE, pr, tag3, MPI_COMM_WORLD,

                         MPI_STATUS_IGNORE);
                end_time = MPI_Wtime();
                receive_time += end_time - start_time;
            }
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < block_size; ++j) {
                    matrix[i * (n + 1) + block_size * pr + j] = buff[i * block_size + j];
                }
            }
        }
        start_time = MPI_Wtime();
        double *x = new double[n];
        x[n - 1] = matrix[(n - 1) * (n + 1) + n] / matrix[(n - 1) * (n + 1) + n - 1];

        for (int i = n - 2; i >= 1; --i) {
            x[i] = matrix[i * (n + 1) + n];
            for (int j = n - 1; j >= i + 1; --j) {
                x[i] -= matrix[i * (n + 1) + j] * x[j];
            }
            x[i] /= matrix[i * (n + 1) + i];
        }
        end_time = MPI_Wtime();
        std::cout << "Время обратного хода " << std::setprecision(3) << std::fixed << (end_time - start_time) * 1000 << "мс" << "\n";
        std::cout << "Время пересылок " << std::setprecision(3) << std::fixed << receive_time  * 1000 << "мс" << "\n";
        std::cout << "Время операций " << std::setprecision(3) << std::fixed << operation_time  * 1000 << "мс" << "\n";
        std::cout << "Общее время " << std::setprecision(3) << std::fixed << (end_time - total_start_time)  * 1000 << "мс" << "\n";
        for (int i = 0; i < n; ++i) {
            out << std::setprecision(10) << std::fixed << x[i] << std::endl;
        }
        out << std::endl;
    }
    MPI_Finalize();
}
