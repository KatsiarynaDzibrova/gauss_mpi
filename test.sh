#!/bin/bash

n=7
matrix=(1.0, 2.0, 3.0, 1.0, 3.0, 0.0, 0.0, 4.0,
        2.0, 3.0, 4.0, 1.0, 0.0, 0.0, 0.0, 5.0,
        10.0, 4.0, 5.0, 0.0, 2.0, 0.0, 0.0, 6.0,
        0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 7.0,
        0.0, 0.0, 0.0, 0.0, 3.0, 4.0, 0.0, 8.0,
        0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 9.0,
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 10.0)

g++ -o test  test.cpp
echo "Not parallel"
result=$(time ./test $n ${matrix[@]})

echo "Parallel"
result_mpi=$(time ./PIRV_3 -n 4 $n ${matrix[@]} 2>/dev/null)

echo "Results Parallel"
echo ${result_mpi[0]}

echo "Result Not parallel"
echo ${result[0]}
