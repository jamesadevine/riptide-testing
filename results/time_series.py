import glob, sys
import pandas as pd
from util import extract_test_results, recursive_extract, summary, directory_prefix

cfg = 0
if len(sys.argv) == 2:
    cfg = sys.argv[1]
else:
    print("No configuration specified.")
    exit(-1)

labels = []

for i in range(1,8):
    # print(cfg)
    extract_test_results(cfg, i)
    # print(summary)

print (summary)

output_p1 = {}
output_p2 = {}

# for configuration 11
# order_map = {
#     "test1":0,
#     "test2":1,
#     "test3":3,
#     "test4":5,
#     "test5":6,
#     "test6":2,
#     "test7":4,
# }
# label_map = {
#     "test1":"test1",
#     "test2":"test2",
#     "test3":"test3",
#     "test4":"test4",
#     "test5":"test5",
#     "test6":"test6",
#     "test7":"test7",
# }

# for configuration 12
order_map = {
    "test1":0,
    "test2":1,
    "test3":2,
    "test4":3,
    "test5":4,
    "test6":5,
    "test7":6,
}

label_map = {
    "test1":"%.2f cm" % (1.*5.2),
    "test2":"%.2f cm" % (2.*5.2),
    "test3":"%.2f cm" % (3.*5.2),
    "test4":"%.2f cm" % (4.*5.2),
    "test5":"%.2f cm" % (5.*5.2),
    "test6":"%.2f cm" % (6.*5.2),
    "test7":"%.2f cm" % (7.*5.2),
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
            labels[order_map[test]] = label_map[test]
            output_p1[device][order_map[test]] = summary[config][device][test]["p1"]
            output_p2[device][order_map[test]] = summary[config][device][test]["p2"]

title = "configuration-"+str(cfg)+"-timeseries"

data_frames = [{"title":"p1", "df": pd.DataFrame(output_p1, index=labels)}, {"title":"p2", "df": pd.DataFrame(output_p2, index=labels)}]

for df in data_frames:
    t = title + "-" + df["title"]
    plt = df["df"].plot.line(title = t, rot=0) #, legend=False , yticks=(spacing)
    plt.legend(loc='lower right')
    # print(str(plt))
    # for idx, bar in enumerate(plt.patches):
    #     h = bar.get_height()
    #     if h < 13:
    #         h = 14
    #     plt.text(bar.get_x() + bar.get_width()/2., h - 13, '%.2f' % float(data_points[idx]), ha='center', va='bottom', rotation=90)

    fig = plt.get_figure()
    # fig.set_figheight(8)
    fig.savefig(t, dpi=100)