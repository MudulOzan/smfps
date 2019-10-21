from collections import namedtuple

#DEFINITIONS
REGFILE = 0
DIR = 1
DATABLOCKSIZE = 512
DIRENTRYSIZE = 32
NUMOFINODES = 32

#SUPER BLOCK
class sb(object):
    __slots__ = ['inode_bitmap', 'data_bitmap'] #REDUCE RAM USAGE
    def __init__(bruh, inode_bitmap, data_bitmap):
        bruh.inode_bitmap = inode_bitmap
        bruh.data_bitmap = [None] * 10 #DEFINITION OF ARRAY


#INODE
class inode(object):
    __slots__ = ['ftype', 'size', 'datablocks']
    def __init__(bruh, ftype, size, datablocks):
        bruh.ftype = ftype
        bruh.size = size
        bruh.datablocks = [None] * 10


#DIRECTORY ENTRIES

class dir_entry(object):
    __slots__ = ['name', 'inode_num']
    def __init__(bruh, name, inode_num):
        bruh.name = [None] * 28
        bruh.inode_num = inode_num
