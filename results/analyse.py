import glob, sys
import pandas as pd
from util import extract_test_results, summary, directory_prefix

title_map = {
    "configuration1" : "Shorts, no timer intervention [ERROR]",
    "configuration2" : "Timer and processor intervention",
    "configuration3" : "Shorts, no processor/timer intervention.\nObserver above transmitter.",
    "configuration4" : "Timer and processor intervention.\nObserver above transmitter."
}

tests_per_configuration = 3.0

# if len(sys.argv) == 1:
#     print("generating graphs for expected configurations and test folders")
#     for i in range(1, 5):
#         for j in range(1, 4):
#             extract_test_results(i, j)

if len(sys.argv) == 3:
    print("generating graphs configuration %s, test %s" % (sys.argv[1], sys.argv[2]))
    extract_test_results(sys.argv[1], sys.argv[2], "configuration-%s-test-%s" % (sys.argv[1], sys.argv[2]))
else:
    exit(-1)

for config in summary.keys():
    dnames = []
    davgs = []
    p1 = []
    p2 = []
    for device in summary[config].keys():
        new_dname = device.replace("-","\n")
        dnames += [new_dname]
        avg = 0.0
        for test in summary[config][device].keys():
            p1 += [summary[config][device][test]["p1"]]
            p2 += [summary[config][device][test]["p2"]]
            avg += float(summary[config][device][test]["avg"])
        avg /= tests_per_configuration / 3.0
        davgs += [avg]

    spacing = [i for i in range(0,105,5)]

    if config not in title_map.keys():
        title = config
    else:
        title = title_map[config]


    data_points = []
    data_points += davgs
    data_points += p1
    data_points += p2

    df = pd.DataFrame({"Avg. Reception (%)" : davgs,'Reception of 1st Frame (%)':p1, 'Reception of 2nd Frame (%)': p2}, index=dnames)
    plt = df.plot.bar(yticks=(spacing), rot=0) #, legend=False , title = title,
    plt.legend(loc='lower right')
    # print(str(plt))
    for idx, bar in enumerate(plt.patches):
        h = bar.get_height()
        if h < 13:
            h = 14
        plt.text(bar.get_x() + bar.get_width()/2., h - 13, '%.2f' % float(data_points[idx]), ha='center', va='bottom', rotation=90)

    fig = plt.get_figure()
    # fig.set_figheight(8)
    fig.savefig(config,dpi=100)

    # exit()
print(summary)