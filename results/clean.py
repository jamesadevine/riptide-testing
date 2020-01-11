import glob

directory_prefix = "./configuration-"

def get_text_file_paths(directory):
    print(directory)
    return glob.glob(directory + "/*.txt")

for i in range(1, 5):
    for j in range(1, 4):
        directory_path = directory_prefix + str(i) + "/test" + str(j)
        paths = get_text_file_paths(directory_path)

        for path in paths:
            with open(path,"r+") as f:
                lines = f.readlines()

                outlines = []
                for l in lines:
                    if "sequence number, p1, p2" in l:
                        continue
                    outlines += [l]

                f.writelines(outlines)