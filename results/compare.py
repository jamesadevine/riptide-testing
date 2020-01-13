import glob, sys
import pandas as pd
from util import extract_test_results, summary, directory_prefix

tests_per_configuration = 3.0




if len(sys.argv) == 1:
    print("cannot compare between runs yet...")

elif len(sys.argv) == 3:
    print("comparing devices in configuration %s, test %s" % (sys.argv[1], sys.argv[2]))
    extract_test_results(sys.argv[1], sys.argv[2], "configuration-%s-test-%s" % (sys.argv[1], sys.argv[2]), True)

for config in summary.keys():
    dnames = []
    davgs = []
    output = {}

    for device in summary[config].keys():

        if device == "transmitter":
            continue

        output[device] = {}
        for comparison_device in summary[config].keys():
            if comparison_device == device or comparison_device == "transmitter":
                continue

            print(set(summary[config][device]["raw"].items()) - set(summary[config][comparison_device]["raw"].items()))
            output[device][comparison_device] = (1000.0 - float(len(set(summary[config][device]["raw"].items()) - set(summary[config][comparison_device]["raw"].items())))) / 10.0

    print(output)


for o in output.keys():
    print ("%s:" % (o))
    avg_difference = 0.0
    for n in output[o].keys():
        avg_difference += output[o][n]
        print ("\t%s: %f%%" % (n, output[o][n]))
    print ("\tavg similarity: %f%%" % (avg_difference / float(len(output[o].keys()))))