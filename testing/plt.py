import matplotlib.pyplot as plt
import numpy as np


SAMPLE_RATE = 8192
N = 128 # Windowing


'''
with open('in.txt') as f:
    lines = f.readlines()

X = []
for line in lines:
    n_i = float(line.strip())
    X.append([n_i,0])
    #output.append()  
X = np.array(X)
'''

X = []
samples = np.fromfile('test_input.iq', np.complex64) # Read in file.  We have to tell it what format it is
for i in range(N):
    #print(samples[i].real)
    #print("Real:{0} -- Imaginary:{1}".format(samples[i].real,samples[i].imag) )
    X.append(samples[i].real)

Y = []
samples = np.fromfile('out.iq', np.complex64) # Read in file.  We have to tell it what format it is
for i in range(N):
    #print(samples[i].real)
    #print("Real:{0} -- Imaginary:{1}".format(samples[i].real,samples[i].imag) )
    Y.append([samples[i].real,samples[i].imag])
Y = np.array(Y)

# Plotting 
_, plots = plt.subplots(2)

## Plot in time domain
plots[0].plot(X)


## Plot in frequent domain
powers_all = np.abs(np.divide(Y, N/2))
powers = powers_all[0:N//2]
frequencies = np.divide(np.multiply(SAMPLE_RATE, np.arange(0, N/2)), N)
plots[1].plot(frequencies, powers)


## Show plots
plt.show()