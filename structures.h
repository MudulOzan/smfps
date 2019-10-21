
#define REGFILE 0
#define DIR 1
#define DATABLOCKSIZE 512
#define DIRENTRYSIZE 32
#define NUMOFINODES 32

struct sb{
	int inode_bitmap;
	int data_bitmap[10];
};

struct inode{
	int type;
	int size;
	int datablocks[10];
};

struct dir_entry{
	char name[28];
	int inode_num;
};






