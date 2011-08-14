#!/bin/bash
g++ main.cpp -o main.out1 -O2 -fopenmp
g++ main.cpp -DPARALLEL -o main.out2 -fopenmp -O2
time ./main.out1 < input > result1
time ./main.out2 < input > result2
