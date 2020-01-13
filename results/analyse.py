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

if len(sys.argv) == 1:
    print("generating graphs for expected configurations and test folders")
    for i in range(1, 5):
        for j in range(1, 4):
            extract_test_results(i, j)

elif len(sys.argv) == 3:
    tests_per_configuration = 1.0
    print("generating graphs configuration %s, test %s" % (sys.argv[1], sys.argv[2]))
    extract_test_results(sys.argv[1], sys.argv[2], "configuration-%s-test-%s" % (sys.argv[1], sys.argv[2]))

for config in summary.keys():
    dnames = []
    davgs = []
    for device in summary[config].keys():
        new_dname = device.replace("-","\n")
        dnames += [new_dname]
        avg = 0.0
        for test in summary[config][device].keys():
            avg += float(summary[config][device][test])
        avg /= tests_per_configuration
        davgs += [avg]

    spacing = [i for i in range(0,105,5)]

    if config not in title_map.keys():
        title = config
    else:
        title = title_map[config]

    df = pd.DataFrame({'Device':dnames, 'Reliability': davgs})
    fig = df.plot.bar(x='Device', y='Reliability', yticks=(spacing), title = title, rot=0).get_figure()
    fig.savefig(config)

    # exit()
print(summary)