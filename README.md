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
#### Example
##### IN:
```
mpicc fftw3-mpi_helloworld.c -lfftw3_mpi -lfftw3 -lm -lmpich -Wall -Ofast
mpiexec -n 4 ./a.out 8192 samples/samples_8192samples_97.6Mhz.iq out.iq
```
##### OUT FILE:  
`out.iq`

## Methodology

## References
[fftw](http://www.fftw.org/)      
[open MPI](https://www.open-mpi.org/)    
[C programming absolute beginner](https://sjceodisha.in/wp-content/uploads/2019/09/C-Programming-Absolute-Beginner-ssp.indian@gmail.com_.pdf)    
[hpc-2021-2](https://github.com/JavoJavo/hpc-2021-2)    
[Docs Oracle](https://docs.oracle.com/)    

## Next
Create an open service with this process
