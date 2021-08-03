#!/bin/bash

mpicc fftw3-mpi_helloworld.c -lfftw3_mpi -lfftw3 -lm -lmpich -Wall -Ofast

TIMEFORMAT=%S

for i in {2..8}
do
  t=$( { time mpiexec -n 4 ./a.out 4194304 samples/number_samples:4194304-center_freq:97900000_Mhz.iq out.iq; } 2>&1 )
  echo "Time with $i process"
  echo "$t"
  echo "$i, $t" >> times.txt
done

./plot.py

rm times.txt
