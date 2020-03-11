import pandas as pd


out_file = "radio-ontime"
title = "Radio on time (microseconds) for comparative protocols"
y_axis = "Radio on time (microseconds)"
x_axis = ""
labels = ["Splash", "Whisper", "Glossy"]
t = [1.2, 1.9, 3.7]
color_map = ["#1f77b4","#ff7f0e",'#2ca02c', '#d62728', '#7f7f7f']
df = pd.DataFrame({'lab':labels,'val':t}) #, index=labels
spacing = [0.1 * i  for i in range(0,40,2)]
plt = df.plot.bar(x='lab',y='val',title = title, rot=0, yticks=(spacing), color=color_map, legend=False) #, legend=False , yticks=(spacing)

plt.set_xlabel(x_axis)
plt.set_ylabel(y_axis)

# plt.legend(bbox_to_anchor=(1.04,0), loc="lower left", borderaxespad=0)
fig = plt.get_figure()

fig.savefig(out_file, dpi=300, bbox_inches = "tight")