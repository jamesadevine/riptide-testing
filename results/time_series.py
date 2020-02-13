import glob, sys
import pandas as pd
from util import extract_test_results, recursive_extract, summary, directory_prefix

mapping = {
    "11":{
        "order_map" : {
            "test1":0,
            "test2":1,
            "test3":3,
            "test4":5,
            "test5":6,
            "test6":2,
            "test7":4,
        },
        "label_map": {
            "test1":"test1",
            "test2":"test2",
            "test3":"test3",
            "test4":"test4",
            "test5":"test5",
            "test6":"test6",
            "test7":"test7",
        }
    },
    "12": {
        "order_map" : {
            "test1":0,
            "test2":1,
            "test3":2,
            "test4":3,
            "test5":4,
            "test6":5,
            "test7":6,
        },

        "label_map" : {
            "test1":"%.2f cm" % (1.*5.2),
            "test2":"%.2f cm" % (2.*5.2),
            "test3":"%.2f cm" % (3.*5.2),
            "test4":"%.2f cm" % (4.*5.2),
            "test5":"%.2f cm" % (5.*5.2),
            "test6":"%.2f cm" % (6.*5.2),
            "test7":"%.2f cm" % (7.*5.2),
        }
    },
    "13":{
        "order_map" : {
            "test1":0,
            "test2":1,
            "test3":2,
            "test4":3,
            "test5":4,
            "test6":5,
            "test7":6,
            "test8":7,
            "test9":8,
            "test10":9,
            "test11":10,
        },
        "label_map" : {
            "test1":"%.2f\ncm" % (1.*8.),
            "test2":"%.2f\ncm" % (2.*8.),
            "test3":"%.2f\ncm" % (3.*8.),
            "test4":"%.2f\ncm" % (4.*8.),
            "test5":"%.2f\ncm" % (5.*8.),
            "test6":"%.2f\ncm" % (6.*8.),
            "test7":"%.2f\ncm" % (7.*8.),
            "test8":"%.2f\ncm" % (8.*8.),
            "test9":"%.2f\ncm" % (9.*8.),
            "test10":"%.2f\ncm" % (10.*8.),
            "test11":"%.2f\ncm" % (11.*8.),
        }
    },
    "14":{
        "x_axis":[
            "Distance from transmitter (metres)",
            "Distance from transmitter (metres)"
        ],
        "y_axis":[
            "Sequence numbers seen (%)",
            "Sequence numbers seen (%)",
        ],
        "title": [
            "Packet Reception vs. Distance (Initial Transmission)",
            "Packet Reception vs. Distance (Repetition)",
        ],
        "order_map" : {
            "test1":0,
            "test2":1,
            "test3":2,
            "test4":3,
            "test5":4,
            "test6":5,
            "test7":6,
        },
        "device_name_map":{
            "top-middle":"Top Middle",
            "transmitter":"Transmitter",
            "bottom-right":"Bottom Right",
            "transmitter-observer":"Transmitter Observer",
            "bottom-middle":"Bottom Middle",
            "top-left":"Top Left",
            "bottom-left":"Bottom Left",
            "top-right":'Top Right'
        },
        'sort_list' : ["Transmitter", "Transmitter Observer", "Top Left", "Top Middle", "Top Right", "Bottom Left", "Bottom Middle", "Bottom Right"],
        "label_map" : {
            "test1":"(.1, .1)",
            "test2":"(.3, .1)",
            "test3":"(.3, .3)",
            "test4":"(.3, .6)",
            "test5":"(.6, .6)",
            "test6":"(.9, .6)",
            "test7":"(.9, .9)",
        }
    },
    "15":{
        "x_axis":[
            "Introduced error (cycles)",
            "Introduced error (cycles)"
        ],
        "y_axis":[
            "Sequence numbers seen (%)",
            "Sequence numbers seen (%)",
        ],
        "title": [
            "Packet Reception vs. Introduced Error (Initial Transmission)",
            "Packet Reception vs. Introduced Error (Repetition)",
        ],
        "order_map" : {
            "test1":0,
            "test2":1,
            "test3":2,
            "test4":3,
            "test5":4,
            "test6":5,
            "test7":6,
            "test8":7,
            "test9":8,
            "test10":9,
            "test11":10,
        },
        "device_name_map":{
            "observer":"Observer"
        },
        "sort_list" : [ "Observer" ],
        "label_map" : {
            "test1":"0\n",
            "test2":"1\n",
            "test3":"2\n",
            "test4":"3\n",
            "test5":"4\n",
            "test6":"5\n",
            "test7":"6\n",
            "test8":"7\n",
            "test9":"8\n",
            "test10":"9\n",
            "test11":"10\n",
        }
    }
}

cfg = 0
if len(sys.argv) == 2:
    cfg = sys.argv[1]
else:
    print("No configuration specified.")
    exit(-1)

current_map = mapping[str(cfg)]

labels = []

for i in range(1,len(current_map["order_map"].keys()) + 1):
    # print(cfg)
    extract_test_results(cfg, i)
    # print(summary)

print (summary)

output_p1 = {}
output_p2 = {}

print(summary)

label = []

for config in summary.keys():
    for device in summary[config].keys():

        d_name = device

        if device in current_map["device_name_map"]:
            d_name = current_map["device_name_map"][device]

        if len(labels) == 0:
            labels = [0] * len(summary[config][device].keys())
        output_p1[d_name] = [0] * len(summary[config][device].keys())
        output_p2[d_name] = [0] * len(summary[config][device].keys())

        for test in summary[config][device].keys():
            labels[current_map["order_map"][test]] = current_map["label_map"][test]
            output_p1[d_name][current_map["order_map"][test]] = summary[config][device][test]["p1"]
            output_p2[d_name][current_map["order_map"][test]] = summary[config][device][test]["p2"]

title = "configuration-"+str(cfg)+"-timeseries"
data_frames = [{"alt_title":"p1", "df": pd.DataFrame(output_p1, index=labels)}, {"alt_title":"p2", "df": pd.DataFrame(output_p2, index=labels)}]

for idx, df in enumerate(data_frames):
    t = title + "-" + df["alt_title"]
    # sort
    df["df"] = df["df"][current_map["sort_list"]]

    spacing = [i for i in range(0,105,5)]

    plt = df["df"].plot.line(title = current_map["title"][idx], rot=0, yticks=(spacing)) #, legend=False

    plt.set_ylabel(current_map["y_axis"][idx])
    plt.set_xlabel(current_map["x_axis"][idx])
    plt.legend(bbox_to_anchor=(1.04,0), loc="lower left", borderaxespad=0)
    fig = plt.get_figure()
    fig.savefig(t, dpi=300, bbox_inches = "tight")