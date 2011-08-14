#!/bin/bash
export OMP_NUM_THREADS=4
g++ main.cpp -o main.out1 -O2
g++ main.cpp -o main.out2 -fopenmp -O2
g++ main.cpp -DPARALLEL -o main.out3 -fopenmp -O2
time ./main.out1 < input > result1
time ./main.out2 < input > result2
time ./main.out3 < input > result3
