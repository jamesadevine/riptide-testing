import matplotlib.pyplot as plt # For ploting
import numpy as np # to work with numerical data efficiently
import pandas as pd

def shift(array,num):
    pres = len(array)
    print("SHIFT BY", int(num))
    new = np.array([np.nan] * int(num))
    array = np.concatenate((new,array))
    return array[:pres]

def neg_shift(array, shift):
    return np.concatenate((array[abs(shift):],array[:abs(shift)]))

def combine(arr1, arr2):
    out = [0] * len(arr1)
    print (arr2[0],arr2[0] == np.nan)
    for i in range(0, len(arr1)):
        if np.isnan(arr2[i]) and not np.isnan(arr1[i]):
            out[i] = arr1[i]
            continue

        if np.isnan(arr1[i]) and not np.isnan(arr2[i]):
            out[i] = arr2[i]
            continue

        out[i] = (arr1[i] + arr2[i])/2.0
    return out

fs = 1000 # number of samples
cycle_time = 62.5
f_mult = 2

x = np.arange(fs) # the points on the x axis for plotting
# compute the value (amplitude) of the sin wave at the for each sample
y = np.sin(f_mult * (2 * np.pi) * (x/fs)) #0
y2 = np.sin(1 * (2 * np.pi) * (x/fs)) #0
# y = np.sin(10 * np.pi * .5 * (x/fs)) #1

xt = [i * 100 for i in range(0,11)]
spacing = [1.0,0.5,0.0,-0.5,-1.0]

plt.subplot(3,1,1)
plt.plot(x,y, 'r')#,x,shift(y, sf * 3.),x,shift(y, sf * 4.))
plt.ylabel("Phase")
plt.xlabel("Time (nanoseconds)")
plt.yticks(spacing)
plt.xticks(xt)
plt.subplot(3,1,2)
plt.plot(x,shift(y, cycle_time * 4.),'g')
plt.ylabel("Phase")
plt.xlabel("Time (nanoseconds)")
# plt.xlim(cycle_time * 4., 1000)
plt.yticks(spacing)
plt.xticks(xt)
plt.subplot(3,1,3)
comb = combine(y, np.array(shift(y, cycle_time * 4.)))
# print(comb)
plt.plot(x, comb,'b')
# plt.subplot(x,y2)
# plt.xticks(xt)
plt.yticks(spacing)
plt.xticks(xt)
plt.ylabel("Phase")
plt.xlabel("Time (nanoseconds)")
# plt.set_ylabel("Phase")
# plt.set_xlabel("Time (nanoseconds)")
plt.show()