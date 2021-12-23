#include <random>
#include <ctime>
#include<fstream>

using namespace std;
mt19937 rnd(time(0));

int main(int argc, char *argv[]) {
    int size = atoi(argv[1]);
    ofstream file("input.txt");
    for (int i = 0; i < size * (size + 1); i++) {
        file << rnd() % 100 << " ";
    }
}

