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
result=$(./test ${n} ${matrix[@]})

result_mpi=$(/home/user/gauss_mpi/cmake-build-debug/PIRV_3 -n 4 ${n} ${matrix[@]})

#for i in ${result[@]}; do echo $i; done
for ((i=0; i< ${n} * (${n} + 1); i++))
do
  if ($(${result[i]} - ${result_mpi[i]}) <= 0.1 && $(${result[i]} - ${result_mpi[i]}) >= -0.1); then
    echo "FALSE"
    break
  fi
done

echo "TRUE"