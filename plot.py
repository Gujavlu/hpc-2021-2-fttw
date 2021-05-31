#!/usr/bin/env python
#!/usr/bin/python
#!python

import numpy as np
import matplotlib.pyplot as plt

timesrecord = np.genfromtxt('times.txt', delimiter = ',')

fig, ax = plt.subplots()
ax.plot(timesrecord[:,0], timesrecord[:,1], 'o')
ax.set_title('Execution times n. process / time (s)')
ax.set_ylabel('time (s)')
ax.set_xlabel('number of process')
plt.savefig('executionTimes.png')
print('Plot file name:\n executionTimes.png')
fig.show()
