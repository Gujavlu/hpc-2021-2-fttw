import numpy as np
import matplotlib.pyplot as plt
import sys

def plot_file(INPUT_FILE_PATH):
    samples = np.fromfile(INPUT_FILE_PATH, np.complex64)
    plt.plot(np.real(samples), np.imag(samples), '.')
    plt.grid(True)
    plt.show()

if __name__=="__main__":
    plot_file( sys.argv[1] ) #IQ FILE PATH 
    print(sys.argv[1])
    """
    Run:
        python3 plot_samples.py <INPUT_FILE_PATH>
    Run Example:
        python3 plot_samples.py out.iq
    """