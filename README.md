# hpc-2021-2-fftw
Parallelization of the fast fourier transform (1d) in C.

## Objetives
With this project we want to create a solution to divide multiple frequencies mixed in a wave parallelizing the process to get fast results.

## Definition
In this project, the Fast Fourier Transform is performed using high-performance computing techniques.
Compiled in C code with the MPI library to run multiple processes where the FFTW3 library is used to perform the transformation.

## Requirements
- MPI C implementation (we used [mpich library](https://www.mpich.org/)) MPICH 3.4.2
- [FFTW  library](http://www.fftw.org/) FFTW 3.3.9

## Run
### Processing toy signal and printing its fft to a file (fftw3+mpi implemented)
##### IN:
```
mpicc fftw3-mpi_helloworld.c -lfftw3_mpi -lfftw3 -lm -lmpich -Wall -Ofast
mpiexec -n 4 ./a.out
```
##### OUT FILE:  
`out.txt`

## TO DO
- Read signal from file

## Next
Create a open service with this process
