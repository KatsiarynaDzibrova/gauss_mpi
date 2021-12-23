#!/bin/bash

n=100

g++ -o generate_test generate.cpp
matrix=$(./generate_test $n)

g++ -o test  test.cpp
echo "Not parallel"
result=$(time ./test $n ${matrix[@]})

echo "Parallel"
result_mpi=$(time ./PIRV_3 -n 4 $n ${matrix[@]} 2>/dev/null)

echo
echo "Results Parallel"
echo ${result_mpi[0]}

echo
g++ -o compare  compare_results.cpp
./compare