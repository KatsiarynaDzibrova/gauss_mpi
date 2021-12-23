#!/bin/bash

n=1000

g++ -o generate_test generate.cpp
matrix=$(./generate_test $n)

g++ -o test  test.cpp
echo "Not parallel"
result=$(time ./test $n < input.txt)

echo "Parallel"
result_mpi=$(time ./PIRV_3 -n 4 $n < input.txt 2>/dev/null)

echo
echo "Results Parallel"
echo ${result_mpi[0]}

echo
g++ -o compare  compare_results.cpp
./compare