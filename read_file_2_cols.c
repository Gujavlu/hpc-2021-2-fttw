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

int main(){
    char fileLine[100]; // Will hold each line of input
    fptr = fopen("out.txt","r");

    fftw_complex *local_in;
    local_in = fftw_alloc_complex(128);

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
    

    print_complex_array(local_in, 128);
    fclose(fptr);
  
    return 0;
}

// Run like this:
// gcc read_file.c -lfftw3


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