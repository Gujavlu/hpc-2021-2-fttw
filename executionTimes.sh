#!/bin/bash

mpicc fftw3-mpi_helloworld.c -lfftw3_mpi -lfftw3 -lm -lmpich -Wall -Ofast
rm times.txt
TIMEFORMAT=%R

for i in {2..8}
do
  t=$( { time mpiexec -n $i ./a.out; } 2>&1 )
  echo "Time with $i process"
  echo "$t"
  echo "$i, $t" >> times.txt
done

./plot.py

rm times.txt
