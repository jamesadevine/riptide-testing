import sys, glob, os

directory_prefix = "./configuration-"

# expected_files_names = ["transmitter-observer.txt", "transmitter.txt", "bottom-left.txt", "top-left.txt", "bottom-right.txt","top-right.txt"]
expected_files_names = ["observer.txt"]

def integrity_check_files_in_folder(configuration_number, test_number):
    directory_path = directory_prefix + str(configuration_number) + "/test" + str(test_number)
    paths = [p for p in glob.glob(directory_path + "/*.txt") if "-clean.txt" not in p]

    if paths == []:
        paths = [p for p in glob.glob(directory_path + "/**/*.txt") if "-clean.txt" not in p]
        if paths == []:
            print("No files to check in %s. Does the path exist?" % directory_path)

    for path in paths:
        with open(path,"r+") as f:
            lines = f.readlines()
            if len(lines) != 1005:
                print("Integrity check failed for %s" % (path))
                exit(-1)
            else:
                print("Integrity check complete for %s" % (path))

    found_files = {}

    for path in paths:
        for fname in expected_files_names:
            if fname in path.split("/")[-1]:
                if not fname in found_files.keys():
                    found_files[fname] = 1
                else:
                    found_files[fname] += 1

    if len(found_files.keys()) == len(expected_files_names):
        n = -1
        for k in found_files:
            if n == -1:
                n = found_files[k]
            elif n != found_files[k]:
                print("ERROR number of files is different:")
                print(found_files)
                exit(-1)
        print("All files present and correct")
    else:
        print("Missing files!!")
        print(found_files, expected_files_names)
        print(list(set(found_files) - set(expected_files_names)))

if len(sys.argv) == 1:
    print("checking all expected configurations and test folders")
    for i in range(1, 5):
        for j in range(1, 4):
            integrity_check_files_in_folder(i, j)
elif len(sys.argv) == 3:
    print("checking configuration %s, test %s" % (sys.argv[1], sys.argv[2]))
    integrity_check_files_in_folder(sys.argv[1], sys.argv[2])
else:
    print("Not a compatible argument list")