import sys, glob, os

directory_prefix = "./configuration-"

def integrity_check_files_in_folder(configuration_number, test_number):
    directory_path = directory_prefix + str(configuration_number) + "/test" + str(test_number)
    paths = [p for p in glob.glob(directory_path + "/*.txt") if "-clean.txt" not in p]

    if paths == []:
        print("No paths to clean in %s. Does the path exist?" % directory_path)

    for path in paths:
        with open(path,"r+") as f:
            lines = f.readlines()
            if len(lines) != 1005:
                print("Integrity check failed for %s" % (path))

if len(sys.argv) == 1:
    print("cleaning all expected configurations and test folders")
    for i in range(1, 5):
        for j in range(1, 4):
            integrity_check_files_in_folder(i, j)
elif len(sys.argv) == 3:
    print("cleaning configuration %s, test %s" % (sys.argv[1], sys.argv[2]))
    integrity_check_files_in_folder(sys.argv[1], sys.argv[2])
else:
    print("Not a compatible argument list")