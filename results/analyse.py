import glob, sys
import pandas as pd

title_map = {
    "configuration1" : "Shorts, no timer intervention [ERROR]",
    "configuration2" : "Timer and processor intervention",
    "configuration3" : "Shorts, no processor/timer intervention.\nObserver above transmitter.",
    "configuration4" : "Timer and processor intervention.\nObserver above transmitter."
}

directory_prefix = "./configuration-"

summary = {}

def extract_test_results(configuration_number, test_number, output_key = ""):
    directory_path = directory_prefix + str(configuration_number) + "/test" + str(test_number)
    paths = glob.glob(directory_path + "/*.txt")

    config_key = "configuration"+str(configuration_number)

    if len(output_key):
        config_key = output_key

    test_key = "test"+str(test_number)

    for path in paths:

        if config_key not in summary.keys():
            summary[config_key] = {}

        device_name = path.split("/")[-1].replace(".txt","")
        with open(path,"r+") as f:
            lines = f.readlines()

            if len(lines) == 0:
                continue

            packets_seen = 0

            for l in lines:
                l = l.strip("\n")
                l = l.split(",")

                if len(l) == 1:
                    continue
                print (len (l), l)

                packets_seen += int(l[1])
                packets_seen += int(l[2])


            if device_name not in summary[config_key].keys():
                summary[config_key][device_name] = {}

            if device_name == "transmitter":
                summary[config_key][device_name][test_key] = float(packets_seen) / 1000.0 * 100.0
            else:
                summary[config_key][device_name][test_key] = float(packets_seen) / 2000.0 * 100.0

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