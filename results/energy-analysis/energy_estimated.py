import pandas as pd


out_file = "energy-comparison-protocols"
title = "Expected Device Lifetime on a 2000mAh Battery Pack"
x_axis = "Protocol"
y_axis = "Lifetime (days) "
labels = ["Glossy", "Whisper", "RipTide"]

results = {
    "Glossy":[1243],
    "Whisper":[2450],
    "Riptide": [3858],
}
t = [1243, 2450, 3858]
color_map = ["#1f77b4","#ff7f0e",'#2ca02c', '#d62728', '#7f7f7f']
df = pd.DataFrame({'lab':labels,'val':t}) #, index=labels
plt = df.plot.bar(x='lab',y='val',title = title, rot=0, color=color_map, legend=False) #, legend=False , yticks=(spacing)

plt.set_xlabel(x_axis)
plt.set_ylabel(y_axis)

# plt.legend(bbox_to_anchor=(1.04,0), loc="lower left", borderaxespad=0)
fig = plt.get_figure()

fig.savefig(out_file, dpi=300, bbox_inches = "tight")