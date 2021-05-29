"""
Code to check that the data read in the C program is correct. 

Test phase, you can ignore this code.
"""
import numpy as np
import matplotlib.pyplot as plt

samples = np.fromfile('samples.iq', np.complex64) # Read in file.  We have to tell it what format it is
for i in range(15):
    #print(samples[i].real)
    print("Real:{0} -- Imaginary:{1}".format(samples[i].real,samples[i].imag) )