import glob, sys, os

summary = {}
directory_prefix = "./configuration-"

def extract_data(path, extract_raw = False):
    device_name = path.split("/")[-1].replace("-clean.txt","")
    with open(path,"r+") as f:
        lines = f.readlines()

        if len(lines) == 0:
            return {}

        packets_seen = 0
        out = {}
        raw = {}
        processed = {}
        p1 = 0
        p2 = 0

        for l in lines:
            l = l.strip("\n")
            l = l.split(",")

            if len(l) == 1:
                continue

            if device_name == "transmitter":
                raw[l[0]] = [l[1], l[2]]
                p1 += int(l[1])
                p2 += int(l[2])
            else:
                #some dofus got the packing the wrong way round for observers...
                raw[l[0]] = [l[2], l[1]]
                p1 += int(l[2])
                p2 += int(l[1])

            processed[l[0]] = int(l[1]) + int(l[2]) #

            if extract_raw:
                out["processed"] = processed
                out["raw"] = raw

            out["p1"] = float(p1) / 1000.0 * 100.0
            out["p2"] = float(p2) / 1000.0 * 100.0
            out["avg"] = ((float(p2) + float(p1))  / 2000.0) * 100.0

        return out

def combine_data_dicts(dicts):
    combined = {}

    for d in dicts:
        for k in d.keys():
            if k not in combined.keys():
                combined[k] = d[k]
            else:
                for k2 in d[k]:
                    print(combined[k][k2])
                    print(d[k][k2])
                    combined[k][k2] = (combined[k][k2] + d[k][k2]) / 2.0

    return combined

def recursive_extract(path):

    merge_array = []

    for fname in os.listdir(path):
        fp = os.path.join(path, fname)
        if os.path.isdir(fp):
            merge_array += [recursive_extract(fp)]

    out = combine_data_dicts(merge_array)

    data_files = glob.glob(path + "/*-clean.txt")

    if data_files == []:
        return out

    new = {}

    for df in data_files:
        device_name = df.split("/")[-1].replace("-clean.txt","")
        new[device_name] = extract_data(df)

    return combine_data_dicts([out, new])

def extract_basic(config_key, test_key, paths, extract_raw):
    for path in paths:

        if config_key not in summary.keys():
            summary[config_key] = {}

        device_name = path.split("/")[-1].replace("-clean.txt","")

        if device_name not in summary[config_key].keys():
            summary[config_key][device_name] = {}

        if test_key not in summary[config_key][device_name].keys():
            summary[config_key][device_name][test_key] = {}

        out = extract_data(path, extract_raw)

        if extract_raw:
            summary[config_key][device_name]["processed"] = out["processed"]
            summary[config_key][device_name]["raw"] = out["raw"]
        summary[config_key][device_name][test_key]["p1"] = out["p1"]
        summary[config_key][device_name][test_key]["p2"] = out["p2"]
        summary[config_key][device_name][test_key]["avg"] = out["avg"]


def extract_test_results(configuration_number, test_number, output_key = "", extract_raw=False):
    directory_path = directory_prefix + str(configuration_number) + "/test" + str(test_number)
    paths = glob.glob(directory_path + "/*-clean.txt")

    config_key = "configuration"+str(configuration_number)

    if len(output_key):
        config_key = output_key

    test_key = "test"+str(test_number)

    if paths == []:
        results = recursive_extract(directory_path)

        if results == {}:
            return

        if config_key not in summary.keys():
            summary[config_key] = {}

        for device_name in results:
            if device_name not in summary[config_key].keys():
                summary[config_key][device_name] = {}

            summary[config_key][device_name][test_key] = results[device_name]
    else:

        extract_basic(config_key, test_key, paths, extract_raw)