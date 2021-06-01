#include <fftw3-mpi.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>

void print_complex_array(const fftw_complex arr[], const int n){
    for(int i=0; i < n; i++)
        printf("%f %f\n", arr[i][0],arr[i][1]);
}

void print_to_file(const fftw_complex arr[], const int n){
    FILE * fptr;
    fptr = fopen("test.txt" ,"a");
    if (fptr == 0){
        printf("Error--file could not be opened.\n");
        exit (1);
    }
    for(int i=0; i < n; i++)
        fprintf(fptr, "%f %f\n", arr[i][0],arr[i][1]);
    fclose(fptr);
}

void load_data(fftw_complex in[], int N, int n, int start_read){
    /* Most of this function is from "read_binary_samples.c" file in this repository
       which has the following licence:
       Copyright (C) 2021 Juan Luis Ruiz Vanegas (juanluisruiz971@comunidad.unam.mx)
       MIT License*/
    struct Complex{
        float real, imaginary;
    };
    
    FILE *ptr_myfile;
    struct Complex signal;

    ptr_myfile=fopen("samples.iq","rb");
    if(!ptr_myfile){
        printf("Unable to open file!\n");
        //return 1;
    }
    for(int i=0; i < N; i++){
        fread(&signal,sizeof(struct Complex),1,ptr_myfile);  // Maybe start reading at the exact memory directin instead of reading from the beginning.
        if(i >= start_read){
            if(i < start_read+n){
                in[i-start_read][0] = signal.real;
                in[i-start_read][1] = signal.imaginary;
            }
        }
    }
        fclose(ptr_myfile);
}

int main(int argc, char ** argv){
    // REMOVING FILE TO DELETE PREVIOUS DATA
    remove("test.txt");
    
    // DECLARING NECESSARY VARIABLES
    const ptrdiff_t N = 14;
    fftw_plan p;
    ptrdiff_t alloc_local, local_ni, local_no, local_i_start, local_o_start;
    

    MPI_Init(&argc, &argv);
    fftw_mpi_init();

    // FIGURING OUT HOW TO DIVIDE DATA INTO PROCESSES
    int rnk;
    MPI_Comm_rank(MPI_COMM_WORLD, &rnk);
    alloc_local = fftw_mpi_local_size_1d(N, MPI_COMM_WORLD,
                                        FFTW_FORWARD, FFTW_ESTIMATE,
                                        &local_ni, &local_i_start,
                                        &local_no, &local_o_start);
    //printf("I am process %d | local_no = %td | local_o_start = %td    || alloc_local = %td\n", rnk+1, local_ni, local_i_start, alloc_local);
    
    // ALLOCATING DATA (at individual processes)
    fftw_complex *local_in, *local_out;
    //local_in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * alloc_local); // it can be done like this too
    local_in = fftw_alloc_complex(alloc_local);
    local_out = fftw_alloc_complex(alloc_local);


    // NOW THAT WE HAVE ALLOCATED SPACE FOR THE DATA, WE MAKE A PLAN
    p = fftw_mpi_plan_dft_1d(N, local_in, local_out, MPI_COMM_WORLD, FFTW_FORWARD, FFTW_ESTIMATE); //in and out pointers refer only to a portion of the transform data

    
    // THEN WE HAVE TO INITIALLIZE THE DATA (in each individual process)
    load_data(local_in, N, local_ni, local_i_start);

    //MPI_Barrier(MPI_COMM_WORLD);

    // PRINTING THE INPUT (of each process) (in order)
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    /*for(int i=0;i<world_size;i++){
        if(i==rnk)
            print_complex_array(local_in, local_ni);
        MPI_Barrier(MPI_COMM_WORLD);
    }*/

    /*MPI_Barrier(MPI_COMM_WORLD);
    printf("\n");
    MPI_Barrier(MPI_COMM_WORLD);*/

    // NOW THE TRANSFORM
    fftw_execute(p);
    
    // PRINTING THE OUTPUT (of each process) (in order)
    for(int i=0;i<world_size;i++){
        //sleep(1);
        if(i==rnk){
            //print_complex_array(local_out, local_no);
            print_to_file(local_out, local_no);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    
    // END AND CLEANUP
    fftw_mpi_cleanup();
    MPI_Finalize();
    fftw_mpi_cleanup();
        
    return 0;
}

/*
We also have to link mpich with -lmpich:
gcc fftw3-mpi_helloworld.c -lfftw3_mpi -lfftw3 -lm -lmpich -Wall
and then.. :  (where n is the number of processes)
mpiexec -n 4 ./a.out 
*/