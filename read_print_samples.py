"""
Saves data in a binary file of real and imaginary signals

Copyright (C) 2021 Juan Luis Ruiz Vanegas (juanluisruiz971@comunidad.unam.mx)

MIT License

A short and simple permissive license with conditions only requiring preservation of copyright and license notices. 
Licensed works, modifications, and larger works may be distributed under different terms and without source code.
"""

from rtlsdr import RtlSdr
import numpy as np 
import sys


def read_print_samples(center_freq):
    """
    input:
    center_freq.- Get/Set the center frequency of the device (in Hz)

    output:
    samples.iq .- binary file
    """
    sdr = RtlSdr()

    """
    sdr.sample_rate = 2.048e6  # Hz
    sdr.center_freq = 70e6     # Hz
    sdr.freq_correction = 60   # PPM
    """
    sdr.center_freq = center_freq     # Hz 
    sdr.sample_rate =  center_freq * 2 #230KHz Muestras por segundo 
    """
    #Nyquist theorem # Hz
    optimum sample_rate: 2B
    Sample rate should be twice the bandwidth
    """
    sdr.freq_correction = 1 #PPM
    sdr.gain = 'auto'
    
    samples = sdr.read_samples(1048576) #number  of  samples  or  bytes  to  read 

    # Now save to an IQ file
    samples = samples.astype(np.complex64) # Convert to 64
    print(type(samples[0])) # Verify it's 64
    samples.tofile('samples_1048576samples_97.6Mhz.iq') # Save to file

    """
    In Python, the default complex type is np.complex128, which uses two 64-bit floats per sample. 
    But in DSP/SDR, we tend to use 32-bit floats instead because the ADCs on our SDRs don’t have that
     much precision to warrant 64-bit floats. In Python we will use np.complex64, which uses two 
     32-bit floats. When you are simply processing a signal in Python it doesn’t really matter, 
     but when you go to save the 1d array to a file, you want to make sure it’s an array of 
     np.complex64 first.

     https://pysdr.org/content/iq_files.html
    """

if __name__=="__main__":
    read_print_samples( int(sys.argv[1]) ) #Center frequency (radio station [Hz])


"""
To Run the code:

    python3 <Radio station in Hertz>
    python3 96700000 
        96.7MHz
"""