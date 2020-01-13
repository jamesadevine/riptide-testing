import matplotlib.pyplot as plt # For ploting
import numpy as np # to work with numerical data efficiently

def shift(array,num):
    pres = len(array)
    new = np.array([0] * num)
    array = np.concatenate((new,array))
    return array[:pres]

fs = 500 # sample rate
f = 2.4 # the frequency of the signal
f2 = 0.025
f3 = 1

x = np.arange(fs) # the points on the x axis for plotting
# compute the value (amplitude) of the sin wave at the for each sample
y = np.sin(10 * np.pi * f * (x/fs))
y2 = np.sin(10 * np.pi * f3 * (x/fs))
y3 = shift(np.sin(10 * np.pi * f3 * (x/fs)),150)

plt.plot(x,y2, x,y, x, y3)
plt.show()