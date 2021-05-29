#include <stdio.h>
#include <stdlib.h>
#include <fftw3-mpi.h>
#include <string.h>

FILE *fptr; // Defining file pointer

void print_complex_array(const fftw_complex arr[], const int n){
    int i;
    for(i=0; i < n; i++)
        printf("%f %f\n", arr[i][0],arr[i][1]);
}

void string_to_complex(const char *array_in, int n, fftw_complex comp_out){//, double **array_out){
    char arr_copy[n+1], string_c[n];
    for(int i=0; i<n; i++){
        arr_copy[i] = *array_in;
        array_in++;
    }arr_copy[n+1] = '\0';
    strcpy(string_c, arr_copy);
    
    char *p;
    p = strtok(arr_copy, " ");
    sscanf(p, "%lf", &comp_out[0]);
    p = strtok(NULL, " ");
    sscanf(p, "%lf", &comp_out[1]);
}


void load_data(fftw_complex in[], int arr_len, int n, int start_pos){
    /* Most of this function is from "read_binary_samples.c" 
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
    for(int i=0; i < arr_len; i++){
        if(i >= start_pos){
            if(i < start_pos+n){
                fread(&signal,sizeof(struct Complex),1,ptr_myfile);
                in[i][0] = signal.real;
                in[i][1] = signal.imaginary;
            }
        }
    }
    fclose(ptr_myfile);
}


int main(){
    const ptrdiff_t N = 128;
    char fileLine[100]; // Will hold each line of input
    fptr = fopen("out.txt","r");

    fftw_complex *local_in;
    local_in = fftw_alloc_complex(N);

    if(fptr != 0){
        int i = 0;
        while (!feof(fptr)){
            fgets(fileLine, 100, fptr);
            if(!feof(fptr))
                string_to_complex(fileLine, 100, local_in[i]);  //puts(fileLine);
            i++;
        }
    }else
        printf("\nError opening file.\n");
    

    //print_complex_array(local_in, N);
    fclose(fptr);
    
    fftw_complex *local_in_2;
    local_in_2 = fftw_alloc_complex(N);
    load_data(local_in_2, 15, 7, 7);
    print_complex_array(local_in_2, 15);
    return 0;
}

// Run like this:
// gcc read_file.c -lfftw3

// I guess it would be easier if I read the file directly in the
// function instead of passing it like an argument from main.

/*
#include<string.h>
#include<stdio.h>
int main()
{
    char input[24] = "-0.000000 64.000000    \n";
    char *p;
    p = strtok(input, " ");

    if(p)
    {
    printf("%s\n", p);
    }
    p = strtok(NULL, " ");

    if(p)
           printf("%s\n", p);
    double i;
    sscanf(p, "%lf", &i);
    printf("%lf\n",i+0.005);
    return 0;
}
*/