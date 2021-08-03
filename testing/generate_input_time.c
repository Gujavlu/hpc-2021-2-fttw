#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

//signal
typedef struct Complex
{
    float real, imaginary;
} Complex;


void print_array(const Complex arr[], const int n){
    for(int i=0; i < n; i++)
        printf("%f %f\n", arr[i].real, arr[i].imaginary);
}

void write_to_file(const Complex out[], const int N, char OUTPUT_FILE_PATH[]){
    FILE * fptr;
    fptr = fopen(OUTPUT_FILE_PATH ,"ab");
    if (fptr == 0){
        printf("Error--file could not be opened.\n");
        exit (1);
    }
    //int f_no = fileno(fptr); 
    //for(int i=0; i < N; i++){
    //    printf("%f %f\n",out[i].real, out[i].imaginary);
        fwrite(out, sizeof(Complex), N, fptr);
    //}
    fclose(fptr);
}

int main(){
    int sampe_rate = 8192;
    int N = 128;
    Complex x[N];
    for(int i=0; i < N; i++){
        x[i].imaginary = 0;
        x[i].real = sin(((2*M_PI*i)/32)); // 32 - 256Hz
        x[i].real += sin((2*M_PI*i)/64); // 64 - 128Hz
    }
    //print_array(x, N);
    remove("test_input.iq");
    write_to_file(x, N, "test_input.iq");

    return 0;
}

/*SAMPLE_RATE = 8192
N = 128 # Windowing

x_values = np.arange(0, N, 1)

x = np.sin((2*np.pi*x_values / 32.0)) # 32 - 256Hz
x += np.sin((2*np.pi*x_values / 64.0)) # 64 - 128Hz

X = fft(x)


# Plotting 
_, plots = plt.subplots(2)

## Plot in time domain
plots[0].plot(x)

## Plot in frequent domain
powers_all = np.abs(np.divide(X, N/2))
powers = powers_all[0:N//2]
frequencies = np.divide(np.multiply(SAMPLE_RATE, np.arange(0, N/2)), N)
plots[1].plot(frequencies, powers)


## Show plots
plt.show()
*/