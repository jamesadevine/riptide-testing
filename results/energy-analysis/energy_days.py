import pandas as pd


out_file = "energy-consumption-days"
title = "Device lifetime for different scenarios"
x_axis = "Scenario"
y_axis = "Lifetime (days) "
labels = ["Riptide (active)", "RipTide (idle)", "BLE (active)", "BLE (idle)"]

results = {
    "RipTide (idle)":[20.5],
    "RipTide (active)":[11.6],
    "BLE (idle)": [28.1],
    "BLE (active)": [26.7]
}
t = [11.6, 20.5, 26.7, 28.1]
color_map = ["#1f77b4","#ff7f0e",'#2ca02c', '#d62728', '#7f7f7f']
df = pd.DataFrame({'lab':labels,'val':t}) #, index=labels
spacing = [i for i in range(0,31,2)]
plt = df.plot.bar(x='lab',y='val',title = title, rot=0, yticks=(spacing), color=color_map, legend=False) #, legend=False , yticks=(spacing)

plt.set_xlabel(x_axis)
plt.set_ylabel(y_axis)

# plt.legend(bbox_to_anchor=(1.04,0), loc="lower left", borderaxespad=0)
fig = plt.get_figure()

fig.savefig(out_file, dpi=300, bbox_inches = "tight")