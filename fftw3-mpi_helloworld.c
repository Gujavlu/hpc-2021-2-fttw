#include <fftw3-mpi.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>


void print_complex_array(const fftw_complex arr[], const int n){
    int i;
    for(i=0; i < n; i++)
        printf("%f %f\n", arr[i][0],arr[i][1]);
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
    // DECLARING NECESSARY VARIABLES
    const ptrdiff_t N = 14;
    //fftw_complex *in;//, *out;
    fftw_plan p;
    ptrdiff_t alloc_local, local_ni, local_no, local_i_start, local_o_start;
    
    // ALLOCATING AND LOADING TOY INPUT
    // It must be kind of pointless to be working in paralel and saving the whole sample in all processes but this is just for practice (the next 2 lines):
    /*in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    //out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    double x_values[N];
    for(int i = 0; i<N; i+=1){
        x_values[i] = i;
        in[i][1] = 0;
        in[i][0] = sin(2*M_PI*x_values[i] / 32);  // 32 - 256Hz   ..I think
        in[i][0] += sin(2*M_PI*x_values[i] / 64); // 64 - 128Hz   ..I think
    }*/

    MPI_Init(&argc, &argv);
    fftw_mpi_init();

    // FIGURING OUT HOW TO DIVIDE DATA INTO PROCESSES
    int rnk;
    MPI_Comm_rank(MPI_COMM_WORLD, &rnk);
    alloc_local = fftw_mpi_local_size_1d(N, MPI_COMM_WORLD,
                                        FFTW_FORWARD, FFTW_ESTIMATE,
                                        &local_ni, &local_i_start,
                                        &local_no, &local_o_start);
    printf("I am process %d | local_no = %td | local_o_start = %td    || alloc_local = %td\n", rnk+1, local_ni, local_i_start, alloc_local);
    
    // ALLOCATING DATA (at individual processes)
    fftw_complex *local_in, *local_out;
    //local_in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * alloc_local); // it can be done like this too
    local_in = fftw_alloc_complex(alloc_local);
    local_out = fftw_alloc_complex(alloc_local);


    // NOW THAT WE HAVE ALLOCATED SPACE FOR THE DATA, WE MAKE A PLAN
    p = fftw_mpi_plan_dft_1d(N, local_in, local_out, MPI_COMM_WORLD, FFTW_FORWARD, FFTW_ESTIMATE); //in and out pointers refer only to a portion of the transform data

    
    // THEN WE HAVE TO INITIALLIZE THE DATA (in each individual process)
    //for (i=0;i<local_ni;i++)
    //    local_in[i][0] = in[local_i_start+i][0];
    load_data(local_in, N, local_ni, local_i_start);

    MPI_Barrier(MPI_COMM_WORLD);

    // PRINTING THE INPUT (of each process) (in order)
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    for(int i=0;i<world_size;i++){
        if(i==rnk)
            print_complex_array(local_in, local_ni);
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    printf("\n");
    MPI_Barrier(MPI_COMM_WORLD);

    // NOW THE TRANSFORM
    fftw_execute(p);
    
    // PRINTING THE OUTPUT (of each process) (in order)
    //print_complex_array(local_out, local_ni); // use just this line but it won't be in order
    for(int i=0;i<world_size;i++){
        sleep(1);
        if(i==rnk)
            print_complex_array(local_out, local_no);
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

TO DO:
- Check why results across multiple process (exampes: 1, 2, 4) are different (kind of unnordered towards the end)
    - I did some experiments using sleep() function and the problem seems to be happening
      while printing the output. If the programm sleep() for 1 second before printing each process'
      output, it prints equal across all n's tested (1,2,3,4,5). Of course we dont want to sleep().
        - One solution might be outputing to different to one file or different files (not to terminal)
        - It might get solved if we send output to file: mpiexec -n 4 ./a.out  ?

    - Cancelled : ~Maybe work with a bigger sample (FFTW specifies in their documentation that
                                       their algorithms are not deterministic and
                                       thay may vary slightly across identic runs,
                                       our sample is of 15 so maybe we need a bigger one,
                                       though it worked perfectly with a 128 sample)~
        - Maybe not cancelled: maybe printing is so fast because the sample is very little, so maybe there's
          no time to finish printing on screen one process' output by the time the other print signal has been
          sent so now there are two printing task at the same time. Maybe?
*/
