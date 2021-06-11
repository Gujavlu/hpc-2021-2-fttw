#include <fftw3-mpi.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void print_complex_array(const fftw_complex arr[], const int n){
    for(int i=0; i < n; i++)
        printf("%f %f\n", arr[i][0],arr[i][1]);
}

void print_to_file(const fftw_complex local_out[], const int local_no, int local_o_start, char OUTPUT_FILE_PATH[]){
    FILE * fptr;
    struct Complex{
        float real, imaginary;
    };
    struct Complex signal;
    fptr = fopen(OUTPUT_FILE_PATH ,"ab");
    if (fptr == 0){
        printf("Error--file could not be opened.\n");
        exit (1);
    }
    int f_no = fileno(fptr); 
    for(int i=0; i < local_no; i++){
        signal.real = local_out[i][0];
        signal.imaginary = local_out[i][1];
        write(f_no, &signal, sizeof(struct Complex));// with pwrite (but it hasn't worked) add offset paramenter and remove loop in main: sizeof(struct Complex)*(start_write+i));
    }
    fclose(fptr);
}

void load_data(fftw_complex local_in[], int local_ni, int local_i_start, char INPUT_FILE_PATH[]){
    /* Some of this function (load_data) is from "read_binary_samples.c" file in this repository
       which has the following licence:
       Copyright (C) 2021 Juan Luis Ruiz Vanegas (juanluisruiz971@comunidad.unam.mx)
       MIT License*/
    struct Complex{
        float real, imaginary;
    };
    
    FILE *ptr_myfile;
    struct Complex signal;

    ptr_myfile=fopen(INPUT_FILE_PATH,"rb");
    if(!ptr_myfile){
        printf("Unable to open file!\n");
        //return 1;
    }
    /*for(int i=0; i < N; i++){
        fread(&signal,sizeof(struct Complex),1,ptr_myfile);  // Maybe start reading at the exact memory direction instead of reading from the beginning.
        if(i >= local_i_start){
            if(i < local_i_start+local_ni){
                local_in[i-local_i_start][0] = signal.real;
                local_in[i-local_i_start][1] = signal.imaginary;
            }
        }
    }*/

    // With the following for loop we start reading at the exact memory direction instead of reading from the beginning.
    int f_no = fileno(ptr_myfile);        
    for(int i=0; i < local_ni; i++){
        pread(f_no, &signal, sizeof(struct Complex), sizeof(struct Complex)*(local_i_start+i));
        local_in[i][0] = signal.real;
        local_in[i][1] = signal.imaginary;
    }
    fclose(ptr_myfile);
}

int main(int argc, char ** argv){
    
    // DECLARING NECESSARY VARIABLES (loading from command line arguments)
    if (argc < 4){
        printf("Missing arguments... \n");
        exit(1);
    }
    ptrdiff_t N;  sscanf(argv[1], "%ld", &N);
    char * INPUT_FILE_PATH = malloc(strlen(argv[2]));   strcpy(INPUT_FILE_PATH, argv[2]);
    char * OUTPUT_FILE_PATH = malloc(strlen(argv[3]));   strcpy(OUTPUT_FILE_PATH, argv[3]);
    
    fftw_plan p;
    ptrdiff_t alloc_local, local_ni, local_no, local_i_start, local_o_start;
    
    //REMOVING FILE TO DELETE PREVIOUS DATA
    remove(OUTPUT_FILE_PATH);

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
    load_data(local_in, local_ni, local_i_start, INPUT_FILE_PATH);

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
            print_to_file(local_out, local_no, local_o_start, OUTPUT_FILE_PATH);
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
and then.. :  (where n is the number of processes, argv[1] is the number of samples, argv[2] is the input file, and argv[3] is the output file)
Example:
mpiexec -n 4 ./a.out 8192 samples/samples_8192samples_97.6Mhz.iq out.iq
*/