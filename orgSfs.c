#include "structures.h"
#include <stdio.h>
#include <string.h>

int main(){
	FILE *fout = fopen("simplefs.bin", "w");
	struct sb super_block;
	super_block.inode_bitmap = 1;
	for(int i = 0; i < 10; i++)
		super_block.data_bitmap[i] = 0;
	super_block.data_bitmap[0] = 1;

	fwrite(&super_block, sizeof(super_block),1,fout);

	struct inode slash;
	slash.type = DIR;
	slash.size = DIRENTRYSIZE * 2; //. and ..
	slash.datablocks[0] = 0;

	fwrite(&slash,sizeof(slash),1,fout);

	struct dir_entry dot;
	strcpy(dot.name, ".");
	dot.inode_num = 0;

	struct dir_entry dotdot;
	strcpy(dotdot.name, "..");
	dotdot.inode_num = 0;
	
	fseek(fout,sizeof(super_block)+NUMOFINODES*sizeof(struct inode) ,SEEK_SET);
	
	fwrite(&dot,sizeof(dot),1,fout);
	fwrite(&dotdot, sizeof(dotdot),1,fout);	
	
	//fwrite(&num, sizeof(int), 1, fout);
	fflush(fout);





	return 0;
}
