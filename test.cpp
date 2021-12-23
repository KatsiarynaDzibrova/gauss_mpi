#include <iostream>

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    double *matrix;
    matrix = new double[(n * (n + 1))];
    for (int i = 0; i < n * (n + 1); i++) {
        sscanf(argv[i+2],"%lf", &matrix[i]);
    }
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
    for (int i = 0; i < n; ++i) {
        std::cout << x[i] << "\t";
    }
    std::cout << std::endl;

    return 0;
}

