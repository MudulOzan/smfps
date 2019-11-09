#include "structures.h"
#include <stdio.h>
#include <string.h>

int main() {
	FILE *fout = fopen("simplefs.bin", "w");
	struct sb super_block;
	super_block.inode_bitmap = 1;
	for(int i = 0; i < 10; i++)
		super_block.data_bitmap[i] = 0;
	super_block.data_bitmap[0] = 1;

	fwrite(&super_block, sizeof(super_block), 1, fout);

	struct inode slash;
	slash.type = DIR;
	slash.size = DIRENTRYSIZE * 2;
	slash.datablocks[0] = 0;

	fwrite(&slash, sizeof(slash), 1, fout);

	struct dir_entry root;
	strcpy(root.name, "/");
	root.inode_num = 0;

	struct dir_entry dot;
	strcpy(dot.name, ".");
	dot.inode_num = 0;

	struct dir_entry dotdot;
	strcpy(dotdot.name, "..");
	dotdot.inode_num = 0;


	
	fseek(fout, sizeof(super_block) + NUMOFINODES * sizeof(struct inode), SEEK_SET);
	fwrite(&root, sizeof(root), 1, fout);
	fwrite(&dot, sizeof(dot), 1, fout);
	fwrite(&dotdot, sizeof(dotdot), 1, fout);
	//..........//

	struct inode home;
	home.type = DIR;
	home.size = DIRENTRYSIZE * 2;
	home.datablocks[0] = 0;


	strcpy(dot.name, ".home");
	dot.inode_num = 1;

	strcpy(dotdot.name, "..");
	dotdot.inode_num = 1;

	fwrite(&dot, sizeof(dot), 1, fout);
	fwrite(&dotdot, sizeof(dotdot), 1, fout);

	struct inode downloads;
	downloads.type = DIR;
	downloads.size = DIRENTRYSIZE * 2;
	downloads.datablocks[0] = 0;


	strcpy(dot.name, ".downloads");
	dot.inode_num = 2;

	strcpy(dotdot.name, "..");
	dotdot.inode_num = 2;

	fwrite(&dot, sizeof(dot), 1, fout);
	fwrite(&dotdot, sizeof(dotdot), 1, fout);


	fflush(fout);

	return 0;
}
