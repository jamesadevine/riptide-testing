import pandas as pd


out_file = "energy-consumption"
title = "Energy Consumption vs. Number of Hops"
x_axis = "Number of hops"
y_axis = "Energy consumption (mA)"
labels = ["Transmitter Only", "0", "1", "2", "3"]

results = {
    "Idle":[4.31, 4.24, 4.67, 4.78, 5.07],
    "Active":[9.94, 7.38, 7.82, 8.33, 8.98],
}

df = pd.DataFrame(results, index=labels)
spacing = [i for i in range(0,105,5)]

plt = df.plot.bar(title = title, rot=0) #, legend=False , yticks=(spacing)

plt.set_xlabel(x_axis)
plt.set_ylabel(y_axis)

plt.legend(bbox_to_anchor=(1.04,0), loc="lower left", borderaxespad=0)
fig = plt.get_figure()
fig.savefig(out_file, dpi=300, bbox_inches = "tight")