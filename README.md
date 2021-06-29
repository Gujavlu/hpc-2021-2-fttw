# hpc-2021-2-fftw
Parallelization of the fast fourier transform (1d) in C.

## Authors
Javier Navarro - <javiernavarro@comunidad.unam.mx>   
Juan Luis Ruiz - <juanluisruiz971@comunidad.unam.mx>   
Gustavo Zarate - <gustavo.zarate.389.cb84@gmail.com>   

## Affiliation
![UNAM ENES affiliation](UNAM_ENES.png)   
We are Bachelor of Science in Information Technologies ("Licenciatura Tecnologías para la Inforamción en Ciencias") students at 6° semester (at editing date-time) at UNAM ("Universidad Nacional Autónoma de México") at the ENES ("Escuela Nacional de Estudios Superiores Unidad Morelia").

## License
MIT License

## Objetives
With this project we want to create a solution to divide multiple frequencies mixed in a wave parallelizing the process to get fast results.

## Definition
In this project, the Fast Fourier Transform is performed using high-performance computing techniques.
Compiled in C code with the MPI library to run multiple processes where the FFTW3 library is used to perform the transformation.

## Requirements
- MPI C implementation (we used [mpich library](https://www.mpich.org/)) MPICH 3.4.2
- [FFTW  library](http://www.fftw.org/) FFTW 3.3.9

## Run
### Receiving signal
### Processing signal (fftw)
```
mpicc fftw3-mpi_helloworld.c -lfftw3_mpi -lfftw3 -lm -lmpich -Wall -Ofast
```
```
mpiexec -n <number_of_processes> ./<executable_file> <number_of_samples> <input_file_path> <output_file_path>
```
The first command is run as it is. (You can run it one time and then reuse the executable file `a.out` for any input)

In the second command the first parameter is the number of processes that are going to be generated, the second parameter is the executable file, the third parameter is the number of samples in the input file path which is the fourth parameter, and the last parameter is the file path where the fftw is going to be written.
### Visualizing results
### Benchmarking with number of processes
### Example
#### Receiving signal
#### Processing signal (fftw)
##### IN:
```
mpicc fftw3-mpi_helloworld.c -lfftw3_mpi -lfftw3 -lm -lmpich -Wall -Ofast
mpiexec -n 4 ./a.out 8192 samples/samples_8192samples_97.6Mhz.iq out.iq
```
##### OUT FILE:  
`out.iq`
#### Visualizing results
#### Benchmarking with number of processes

## Introduction

## Methodology
### Receiving signal sample 
### Processing signal (fftw)
### Visualizing results

## References
[fftw](http://www.fftw.org/)      
[open MPI](https://www.open-mpi.org/)    
[C programming absolute beginner](https://sjceodisha.in/wp-content/uploads/2019/09/C-Programming-Absolute-Beginner-ssp.indian@gmail.com_.pdf)    
[hpc-2021-2](https://github.com/JavoJavo/hpc-2021-2)    
[Docs Oracle](https://docs.oracle.com/)    

## Next
Create an open service with this process
