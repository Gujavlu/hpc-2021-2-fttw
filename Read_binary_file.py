"""
Code to check that the data read in the C program is correct. 

Test phase, you can ignore this code.
"""
import numpy as np
import matplotlib.pyplot as plt
import sys
"""
samples = np.fromfile('samples.iq', np.complex64) # Read in file.  We have to tell it what format it is
for i in range(15):
    #print(samples[i].real)
    print("Real:{0} -- Imaginary:{1}".format(samples[i].real,samples[i].imag) )
"""
def read_binary(INPUT_FILE_PATH, number_samples):
    samples = np.fromfile(INPUT_FILE_PATH, np.complex64)
    for i in range(15):
        print("Real:{0} -- Imaginary:{1}".format(samples[i].real,samples[i].imag) )

if __name__=="__main__":
    read_binary( sys.argv[1],int(sys.argv[2]) ) #Center frequency (radio station [Hz])
    print(sys.argv[1],int(sys.argv[2]))
    """
    Run:
        python3 Read_binary_file.py <INPUT_FILE_PATH> <number_samples of binary file>
    Run Example:
        python3 Read_binary_file.py out.iq 131072
    """