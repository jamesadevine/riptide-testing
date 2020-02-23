import matplotlib.pyplot as plt # For ploting
import numpy as np # to work with numerical data efficiently
import pandas as pd

def shift(array,num):
    pres = len(array)
    print("SHIFT BY", int(num))
    new = np.array([0] * int(num))
    array = np.concatenate((new,array))
    return array[:pres]

def neg_shift(array, shift):
    return np.concatenate((array[abs(shift):],array[:abs(shift)]))

fs = 1000 # number of samples
cycle_time = 62.5
f_mult = 2

x = np.arange(fs) # the points on the x axis for plotting
# compute the value (amplitude) of the sin wave at the for each sample
y = np.sin(f_mult * (2 * np.pi) * (x/fs)) #0
y2 = np.sin(1 * (2 * np.pi) * (x/fs)) #0
# y = np.sin(10 * np.pi * .5 * (x/fs)) #1

xt = [i * 100 for i in range(0,11)]

plt.plot(x,y,x,shift(y, cycle_time * 1.0))#,x,shift(y, sf * 3.),x,shift(y, sf * 4.))
# plt.subplot(x,y2)
plt.xticks(xt)
plt.ylabel("Phase")
plt.xlabel("Time (nanoseconds)")
# plt.set_ylabel("Phase")
# plt.set_xlabel("Time (nanoseconds)")
plt.show()