# hpc-2021-2-fftw
Parallelization of the fast fourier transform (1d) in C.

## Requirements
- MPI C implementation (we used [mpich library](https://www.mpich.org/))
- [FFTW  library](http://www.fftw.org/)

## Run
### Processing toy signal and printing its fft to a file (fftw3+mpi implemented)
##### IN:
```
gcc fftw3-mpi_helloworld.c -lfftw3_mpi -lfftw3 -lm -lmpich -Wall -Ofast
mpiexec -n 4 ./a.out
```
##### OUT FILE:  
`out.txt`

## TO DO
- Read signal from file
