import glob, sys
import pandas as pd
from util import extract_test_results, recursive_extract, summary, directory_prefix

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
# order_map = {
#     "test1":0,
#     "test2":1,
#     "test3":2,
#     "test4":3,
#     "test5":4,
#     "test6":5,
#     "test7":6,
# }

# label_map = {
#     "test1":"%.2f cm" % (1.*5.2),
#     "test2":"%.2f cm" % (2.*5.2),
#     "test3":"%.2f cm" % (3.*5.2),
#     "test4":"%.2f cm" % (4.*5.2),
#     "test5":"%.2f cm" % (5.*5.2),
#     "test6":"%.2f cm" % (6.*5.2),
#     "test7":"%.2f cm" % (7.*5.2),
# }

#for configuration 13
# order_map = {
#     "test1":0,
#     "test2":1,
#     "test3":2,
#     "test4":3,
#     "test5":4,
#     "test6":5,
#     "test7":6,
#     "test8":7,
#     "test9":8,
#     "test10":9,
#     "test11":10,
# }

# label_map = {
#     "test1":"%.2f\ncm" % (1.*8.),
#     "test2":"%.2f\ncm" % (2.*8.),
#     "test3":"%.2f\ncm" % (3.*8.),
#     "test4":"%.2f\ncm" % (4.*8.),
#     "test5":"%.2f\ncm" % (5.*8.),
#     "test6":"%.2f\ncm" % (6.*8.),
#     "test7":"%.2f\ncm" % (7.*8.),
#     "test8":"%.2f\ncm" % (8.*8.),
#     "test9":"%.2f\ncm" % (9.*8.),
#     "test10":"%.2f\ncm" % (10.*8.),
#     "test11":"%.2f\ncm" % (11.*8.),
# }

# configuration 14
# order_map = {
#     "test1":0,
#     "test2":1,
#     "test3":2,
#     "test4":3,
#     "test5":4,
#     "test6":5,
#     "test7":6,
# }

# label_map = {
#     "test1":"LR: .1\nRR: .1\n",
#     "test2":"LR: .3\nRR: .1\n",
#     "test3":"LR: .3\nRR: .3\n",
#     "test4":"LR: .3\nRR: .6\n",
#     "test5":"LR: .6\nRR: .6\n",
#     "test6":"LR: .9\nRR: .6\n",
#     "test7":"LR: .9\nRR: .9\n",
# }

mapping = {
    "16" : {
        "order_map":{
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
            "test1":"0 Hops\n",
            "test2":"0-1 Hops\n",
            "test3":"1-2 Hops\n",
            "test4":"2 Hops\n",
        },
        "device_name_map" : {
            "transmitter":"Transmitter",
            "observer[1]":"Observer 1",
            "observer[2]":"Observer 2",
            "observer[3]":"Observer 3",
            "observer[5]":"Observer 4",
            "observer[6]":"Observer 5",
            "observer[7]":"Observer 6",
            "observer[8]":"Observer 7",
        },
        "color_map":["#1f77b4","#ff7f0e",'#2ca02c', '#d62728', '#7f7f7f'],
        'sort_list' : ["Transmitter", "Observer 1", "Observer 2", "Observer 3", "Observer 4"]
    },
    "17" : {
        "order_map":{
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
            "test1":""
        },
        "device_name_map" : {
            "transmitter":"Transmitter",
            "observer[1]":"Observer 1",
            "observer[2]":"Observer 2",
            "observer[3]":"Observer 3",
            "observer[5]":"Observer 4",
            "observer[6]":"Observer 5",
            "observer[7]":"Observer 6",
            "observer[8]":"Observer 7",
        },
        "sort_list" : ["Transmitter", "Observer 5", "Observer 1", "Observer 2", "Observer 3", "Observer 6", "Observer 7", "Observer 4"],
        "color_map": ["#1f77b4", '#9467bd',"#ff7f0e",'#2ca02c', '#d62728', '#8c564b', '#e377c2', '#7f7f7f']
    },
    "18" : {
        "order_map":{
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
            "test1":""
        },
        "device_name_map" : {
            "transmitter":"Transmitter",
            "observer[1]":"Observer 1",
            "observer[2]":"Observer 2",
            "observer[3]":"Observer 3",
            "observer[5]":"Observer 4",
            "observer[6]":"Observer 5",
            "observer[7]":"Observer 6",
            "observer[8]":"Observer 7",
        },
        "color_map":["#1f77b4","#ff7f0e",'#2ca02c', '#d62728', '#7f7f7f'],
        'sort_list' : ["Transmitter", "Observer 1", "Observer 2", "Observer 3", "Observer 4"]
    }
}

cfg = 0
test = 0

if len(sys.argv) == 2:
    cfg = sys.argv[1]
    test = len(order_map.keys())
elif len(sys.argv) == 3:
    cfg = sys.argv[1]
    test = int(sys.argv[2])
else:
    print("No configuration specified.")
    exit(-1)

for i in range(1, 1 + test):
    # print(cfg)
    extract_test_results(cfg, i, use_new=True)
    # print(summary)

labels = []
packet_count_data = {}
reliability_data = {}

current_map = mapping[str(cfg)]

for config in summary.keys():
    for device in summary[config].keys():

        d_name = device

        if device in current_map["device_name_map"]:
            d_name = current_map["device_name_map"][device]

        if len(labels) == 0:
            labels = [0] * len(summary[config][device].keys())
        packet_count_data[d_name] = [0] * len(labels)
        reliability_data[d_name] = [0] * len(labels)

        for idx,test in enumerate(summary[config][device].keys()):
            labels[current_map["order_map"][test]] = current_map["label_map"][test]
            packet_count_data[d_name][current_map["order_map"][test]] = sum(summary[config][device][test]["counters"])
            reliability_data[d_name][current_map["order_map"][test]] = summary[config][device][test]["reliability"]

print (packet_count_data)
print (reliability_data)

title = "configuration-"+str(cfg)+"-bar-graph"

data_frames = [{"alt_title":"packet-count","title":"Packet Count", "df": pd.DataFrame(packet_count_data, index=labels)}, {"alt_title":"reliability","title":"% of sequence numbers seen", "df": pd.DataFrame(reliability_data, index=labels)}]

for df in data_frames:

    df["df"] = df["df"][current_map["sort_list"]]
    t = title + "-" + df["alt_title"]

    spacing = [i for i in range(0,105,5)]

    if "color_map" in current_map.keys():
        plt = df["df"].plot.bar(title = df["title"], rot=0, color=current_map["color_map"], yticks=(spacing)) #, legend=False , yticks=(spacing)
    else:
        plt = df["df"].plot.bar(title = df["title"], rot=0, yticks=(spacing)) #, legend=False , yticks=(spacing)

    plt.legend(loc='lower right')
    fig = plt.get_figure()
    # fig.set_figheight(8)
    fig.savefig(t, dpi=300)