//https://www.codingunit.com/c-tutorial-binary-file-io
"""
Read data from a binary file of real and imaginary signals

Copyright (C) 2021 Juan Luis Ruiz Vanegas (juanluisruiz971@comunidad.unam.mx)

MIT License

A short and simple permissive license with conditions only requiring preservation of copyright and license notices. 
Licensed works, modifications, and larger works may be distributed under different terms and without source code.
"""

#include<stdio.h>

/* Our structure */
struct Complex
//signal
{
    float real, imaginary;
};

int main()
{
    int counter;
    FILE *ptr_myfile;
    struct Complex signal;

    ptr_myfile=fopen("samples.iq","rb");
    if (!ptr_myfile)
    {
        printf("Unable to open file!\n");
        return 1;
    }
    for ( counter=1; counter <= 15; counter++)
    {
        fread(&signal,sizeof(struct Complex),1,ptr_myfile);
        printf("Real: %f -- Imaginary: %f \n",signal.real,signal.imaginary);
    }
    fclose(ptr_myfile);
    return 0;
}

/*
Print more digits on printf("signal.real") 
*/