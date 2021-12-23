#include <random>
#include <ctime>
#include <iostream>

using namespace std;
mt19937 rnd(time(0));

int main(int argc, char *argv[]) {
    int size = atoi(argv[1]);
    for (int i = 0; i < size * (size + 1); i++) {
        std::cout << rnd() % 100 << " ";
    }
}

