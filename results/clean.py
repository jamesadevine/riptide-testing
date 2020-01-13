import glob, sys

directory_prefix = "./configuration-"

def clean_files_in_folder(configuration_number, test_number):
    directory_path = directory_prefix + str(configuration_number) + "/test" + str(test_number)
    paths = glob.glob(directory_path + "/*.txt")

    if paths == []:
        print("No paths to clean in %s. Does the path exist?" % directory_path)

    for path in paths:
        with open(path,"r+") as f:
            lines = f.readlines()
            lines = lines[:1000]

            outlines = []
            for l in lines:
                if "sequence number, p1, p2" in l:
                    continue
                outlines += [l]
            f.seek(0)
            f.truncate(1000)
            f.writelines(outlines)

if len(sys.argv) == 1:
    print("cleaning all expected configurations and test folders")
    for i in range(1, 5):
        for j in range(1, 4):
            clean_files_in_folder(i, j)
elif len(sys.argv) == 3:
    print("cleaning configuration %s, test %s" % (sys.argv[1], sys.argv[2]))
    clean_files_in_folder(sys.argv[1], sys.argv[2])
else:
    print("Not a compatible argument list")