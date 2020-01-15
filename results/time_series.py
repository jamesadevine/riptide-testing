import glob, sys
import pandas as pd
from util import extract_test_results, summary, directory_prefix

cfg = 0
if len(sys.argv) == 2:
    cfg = sys.argv[1]
else:
    print("No configuration specified.")
    exit(-1)

labels = []

for i in range(1,8):
    print(cfg)
    extract_test_results(cfg, i)
    print(summary)

output_p1 = {}
output_p2 = {}

order_map = {
    "test1":0,
    "test2":1,
    "test3":3,
    "test4":5,
    "test5":6,
    "test6":2,
    "test7":4,
}

print(summary)

label = []

for config in summary.keys():

    for device in summary[config].keys():
        if len(labels) == 0:
            labels = [0] * len(summary[config][device].keys())
        output_p1[device] = [0] * len(summary[config][device].keys())
        output_p2[device] = [0] * len(summary[config][device].keys())

        for test in summary[config][device].keys():
            labels[order_map[test]] = test
            output_p1[device][order_map[test]] = summary[config][device][test]["p1"]
            output_p2[device][order_map[test]] = summary[config][device][test]["p2"]

title = "configuration-"+str(cfg)+"-timeseries"

df = pd.DataFrame(output_p2, index=labels)
plt = df.plot.line(title = title, rot=0) #, legend=False , yticks=(spacing)
plt.legend(loc='lower right')
# print(str(plt))
# for idx, bar in enumerate(plt.patches):
#     h = bar.get_height()
#     if h < 13:
#         h = 14
#     plt.text(bar.get_x() + bar.get_width()/2., h - 13, '%.2f' % float(data_points[idx]), ha='center', va='bottom', rotation=90)

fig = plt.get_figure()
# fig.set_figheight(8)
fig.savefig(title,dpi=100)