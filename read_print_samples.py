#Simple way to read and print some samples
from rtlsdr import RtlSdr
from math import e
import numpy as np 
import sys

#def read_print_samples(sample_rate, center_freq, freq_correction):
def read_print_samples(center_freq):
    """
    center_freq.- Get/Set the center frequency of the device (in Hz)
    """
    sdr = RtlSdr()
    
    # configure device
    """
    sdr.sample_rate = 2.048e6  # Hz
    sdr.center_freq = 70e6     # Hz
    sdr.freq_correction = 60   # PPM
    """
    sdr.center_freq = center_freq     # Hz 

    sdr.sample_rate =  230000 #66KHz
    """
    #Teorema de Nyquist  # Hz
    sample_rate óptimo: 2B
    Frecuencia de muestreo debe ser dos veces el ancho de banda
    """
    sdr.freq_correction = 1   # PPM
    sdr.gain = 'auto'
    
    samples = sdr.read_samples(1024) #number  of  samples  or  bytes  to  read 
    np.save('samples.npy', samples)
    #print (samples)

if __name__=="__main__":
    #print(read_print_samples( int(sys.argv[1]),int(sys.argv[2]),int(sys.argv[3]) ) )
    read_print_samples( int(sys.argv[1]) ) #Frecuancia centra (estación de radio)
