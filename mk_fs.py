from structures import *
import pickle

fout = open("simplefs.bin", "wb") #OPEN FILE AND WRITE AS BINARY
super_block = sb(1, [1]) #CREATE SUBER BLOCK AND ASSING INODE AND DATA BITMAP
super_block.inode_bitmap = 1

for x in range(10):
    print(x)
    super_block.data_bitmap[x] = 0

super_block.data_bitmap[0] = 0

pickle.dump(sb, fout) #WRITE AS BINARY

slash = inode(DIR, (DIRENTRYSIZE * 2), [0]) #CREATE ROOT DIRECTORY
pickle.dump(slash, fout)

dot = dir_entry(".", 0) #PARENT DIRECTORY
dotdot = dir_entry("..", 0) #CHILD DIRECTORY

pickle.dump(dot, fout)
pickle.dump(dotdot, fout)

print(super_block)

