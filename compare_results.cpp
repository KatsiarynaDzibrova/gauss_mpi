#include <fstream>
#include <iostream>
#include <cmath>

int main() {
    std::ifstream res_mpi("result_mpi.txt");
    std::ifstream res("result.txt");
    while(res_mpi || res) {
        double line_mpi;
        res_mpi >> line_mpi;
        double line;
        res >> line;
        if (fabs(line_mpi - line) > 0.00001) {
            std::cout << "Results are not equal!" << std::endl;
            return 0;
        }
    }
    std::cout << "Results are equal" << std::endl;
}
