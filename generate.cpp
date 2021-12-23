#include <random>
#include <ctime>
#include <fstream>

using namespace std;
mt19937 rnd(time(0));

int main(int argc, char *argv[]) {
    std::ofstream file("input.txt");
    int size = atoi(argv[1]);
    for (int i = 0; i < size * (size + 1); i++) {
        file << rnd() % 100 << " ";
    }
}

