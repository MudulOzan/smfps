from structures import *

fin = open("simplefs.bin", "rb")

slash = inode

contents = fin.read()
print("type: {size} ".format(slash.size())