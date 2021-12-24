#include <fstream>
#include <iostream>
#include <iomanip>
#include <chrono>


int main(int argc, char *argv[]) {
    std::ofstream out("result.txt");
    int n = atoi(argv[1]);
//    int n;
//    std::cin >> n;
    double *matrix;
    matrix = new double[(n * (n + 1))];
    for (int i = 0; i < n * (n + 1); i++) {
//        sscanf(argv[i+2],"%lf", &matrix[i]);
        std::cin >> matrix[i];
    }

    std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
    for (int k = 0; k < n - 1; ++k) {
        for (int i = k + 1; i < n; ++i) {
            for (int j = k + 1; j < n + 1; j++) {
                matrix[i * (n + 1) + j] -= matrix[i * (n + 1) + k] / matrix[k * (n + 1) + k] * matrix[k * (n + 1) + j];
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

    std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
    std::cout << "Общее время " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "мс" << std::endl;
    for (int i = 0; i < n; ++i) {
        out << std::setprecision(10) << std::fixed<< x[i] << std::endl;
    }
    out << std::endl;
    return 0;
}

