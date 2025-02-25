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

	//------------ INODES START ------------//

	struct inode slash;
	slash.type = DIR;
	slash.size = DIRENTRYSIZE * 2;
	slash.datablocks[0] = 0;

	fwrite(&slash, sizeof(slash), 1, fout);

	//---------------------------------------------------------------------//

	struct inode inHome;
	inHome.type = DIR;
	inHome.size = DIRENTRYSIZE * 3;
	inHome.datablocks[0] = 1;

//	fwrite(&inHome, sizeof(inHome), 1, fout);

	//---------------------------------------------------------------------//

	struct inode inDownloads;
	inDownloads.type = DIR;
	inDownloads.size = DIRENTRYSIZE * 2;
	inDownloads.datablocks[0] = 2;
	
//	fwrite(&inDownloads, sizeof(inDownloads), 1, fout);

	//---------------------------------------------------------------------//

	struct inode inNotes;
	inNotes.type = DIR;
	inNotes.size = DIRENTRYSIZE * 2;
	inNotes.datablocks[0] = 3;
	
//	fwrite(&inNotes, sizeof(inNotes), 1, fout);

	//------------ INODES END ------------//


	//------------ DIR ENTRIES START ------------//

	struct dir_entry dot;
	strcpy(dot.name, ".");
	dot.inode_num = 0;

	struct dir_entry dotdot;
	strcpy(dotdot.name, "..");
	dotdot.inode_num = 0;

	struct dir_entry home;
	strcpy(home.name, "home");
	home.inode_num = 1;

	struct dir_entry downloads;
	strcpy(downloads.name, "downloads");
	downloads.inode_num = 2;

	fseek(fout, sizeof(super_block) + NUMOFINODES * sizeof(struct inode) + 512 * 0, SEEK_SET);
	fwrite(&dot, sizeof(dot), 1, fout);
	fwrite(&dotdot, sizeof(dotdot), 1, fout);
//	fwrite(&home, sizeof(home), 1, fout);
//	fwrite(&downloads, sizeof(downloads), 1, fout);

	//---------------------------------------------------------------------//

	strcpy(dot.name, ".");
	dot.inode_num = 1;

	strcpy(dotdot.name, "..");
	dotdot.inode_num = 0;

	struct dir_entry notes;
	strcpy(notes.name, "notes");
	notes.inode_num = 3;

	fseek(fout, sizeof(super_block) + NUMOFINODES * sizeof(struct inode) + 512 * 1, SEEK_SET);
//	fwrite(&dot, sizeof(dot), 1, fout);
//	fwrite(&dotdot, sizeof(dotdot), 1, fout);
//	fwrite(&notes, sizeof(notes), 1, fout);


	//---------------------------------------------------------------------//

	strcpy(dot.name, ".");
	dot.inode_num = 2;

	strcpy(dotdot.name, "..");
	dotdot.inode_num = 0;

	fseek(fout, sizeof(super_block) + NUMOFINODES * sizeof(struct inode) + 512 * 2, SEEK_SET);
//	fwrite(&dot, sizeof(dot), 1, fout);
//	fwrite(&dotdot, sizeof(dotdot), 1, fout);

	//---------------------------------------------------------------------//

	strcpy(dot.name, ".");
	dot.inode_num = 3;

	strcpy(dotdot.name, "..");
	dotdot.inode_num = 1;

	fseek(fout, sizeof(super_block) + NUMOFINODES * sizeof(struct inode) + 512 * 3, SEEK_SET);
//	fwrite(&dot, sizeof(dot), 1, fout);
//	fwrite(&dotdot, sizeof(dotdot), 1, fout);

	//------------ DIR ENTRIES END ------------//

	fflush(fout);

	return 0;
}
