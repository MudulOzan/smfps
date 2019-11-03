#include "structures.h"
#include <stdio.h>

int main(){
	FILE * fin = fopen("simplefs.bin", "r");
       	struct inode slash;
		struct sb super_block;
	fread(&super_block,sizeof(super_block),1,fin);
	fread(&slash,sizeof(slash),1,fin);
	printf("Super Block\n");
	printf("type: %d\n", super_block.inode_bitmap);
	printf("datablocks[0]: %d\n", super_block.data_bitmap[0]);	

	printf("Slash\n");
	printf("type: %d\n", slash.type);
	printf("size: %d\n", slash.size);
	printf("datablocks[0]: %d\n", slash.datablocks[0]);	

	void ls() {
		
	}
	return 0;
}
