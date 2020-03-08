import os

for i in range(1, 12):
    for j in range(1, 4):
        os.system("cp -R configuration-15/test%d/run%d configuration-20/test%d/run%d" % (i, j, i, j + 3))