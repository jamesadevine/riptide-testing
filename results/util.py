import glob, sys

summary = {}
directory_prefix = "./configuration-"

def extract_test_results(configuration_number, test_number, output_key = "", extract_raw = False):
    directory_path = directory_prefix + str(configuration_number) + "/test" + str(test_number)
    paths = glob.glob(directory_path + "/*-clean.txt")

    config_key = "configuration"+str(configuration_number)

    if len(output_key):
        config_key = output_key

    test_key = "test"+str(test_number)

    for path in paths:

        if config_key not in summary.keys():
            summary[config_key] = {}

        device_name = path.split("/")[-1].replace("-clean.txt","")
        with open(path,"r+") as f:
            lines = f.readlines()

            if len(lines) == 0:
                continue

            packets_seen = 0
            raw = {}

            for l in lines:
                l = l.strip("\n")
                l = l.split(",")

                if len(l) == 1:
                    continue
                raw[l[0]] = int(l[1]) + int(l[2]) #[l[1], l[2]]
                packets_seen += int(l[1])
                packets_seen += int(l[2])


            if device_name not in summary[config_key].keys():
                summary[config_key][device_name] = {}

            if extract_raw:
                summary[config_key][device_name]["raw"] = raw

            if device_name == "transmitter":
                summary[config_key][device_name][test_key] = float(packets_seen) / 1000.0 * 100.0
            else:
                summary[config_key][device_name][test_key] = float(packets_seen) / 2000.0 * 100.0
