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

        raw_data = summary[config][device]["raw"]

        t1 = 0
        t2 = 0

        for r in raw_data.keys():
            t1 += int(raw_data[r][0])
            t2 += int(raw_data[r][1])

        # if device == "transmitter":
        #     continue

        output[device] = {}

        output[device]["t1"] = t1
        output[device]["t2"] = t2

        most_similar = {
            "dname": "",
            "similarity": 0.0
        }

        for comparison_device in summary[config].keys():
            if comparison_device == device or comparison_device == "transmitter":
                continue

            similarity = (1000.0 - float(len(set(summary[config][device]["processed"].items()) - set(summary[config][comparison_device]["processed"].items())))) / 10.0
            output[device][comparison_device] = similarity

            if similarity > most_similar["similarity"]:
                most_similar["dname"] = comparison_device
                most_similar["similarity"] = similarity

        output[device]["most_similar"] = most_similar["dname"]


for o in output.keys():
    print ("%s [%.2f%%, %.2f%%]:" % (o, float(output[o]["t1"]) / 1000.0 * 100.0, float(output[o]["t2"]) / 1000.0 * 100.0))
    avg_difference = 0.0

    filtered_keys = [k for k in output[o].keys() if k not in ["most_similar", "t1", "t2"]]

    for n in filtered_keys:

        avg_difference += output[o][n]

        similarity_string = ""

        if n == output[o]["most_similar"]:
            similarity_string = "[most similar]"

        tabulation = "\t\t"

        if len(n) > 12:
            tabulation = "\t"
        print ("\t%s: %s%f%% %s" % (n, tabulation, output[o][n], similarity_string))
    print ("\tavg similarity: \t%f%%" % (avg_difference / float(len(filtered_keys))))