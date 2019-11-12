#include <stdio.h>
#include "structures.h"
#include <string.h>

int inum = 0;
int dirnum = 0;

void ls(FILE *file);
void get_inode_struct(FILE *f, struct inode *inode,int inode_num);
void mkdir(FILE *file);
void get_dir_entry(FILE *fin, struct dir_entry *de, int db_num, int dir_entry_num);

int main() {
    FILE *fin = fopen("simplefs.bin", "r");

    char command[32];
    while(1) {
        printf("> ");
		scanf("%s",command);
        if(strcmp(command, "ls") == 0) {
            ls(fin);
        }
        else if (strcmp(command,"mkdir") == 0) {
            mkdir(fin);
        }
    }
}

void ls(FILE *fin) {
    struct inode inostr;
    struct dir_entry de;
    get_inode_struct(fin, &inostr, inum);

    while(inostr.size == 64) {
        for(int i = 0; i < 3; i++){ // dir entry times 
            get_dir_entry(fin, &de, 0, dirnum);
            
            if(strcmp(de.name, ".")==0 ||strcmp(de.name, "..")==0)
                printf("\t%s\n", de.name);
            else
                printf("%s\n", de.name);
            dirnum++;
        }   
        inum++;
        get_inode_struct(fin, &inostr, inum);

        printf("inum: %d\n", inum);
    }
}


void mkdir(FILE *fin, char name[28]) {
    printf("you are inside mkdir\n");
    //we need an fseek here in order to jump written inodes
	struct inode dir;
	dir.type = DIR;
	dir.size = DIRENTRYSIZE * 2;
	dir.datablocks[0] = 0;

    fwrite(&dir, sizeof(dir), 1, fout);

    fseek(fout, sizeof(super_block) + NUMOFINODES * sizeof(struct inode), SEEK_SET); // this will be calculated till the end of the dir entries
	
    struct dir_entry folder;
	strcpy(folder.name, name);
	folder.inode_num = 1; // this will be the current folders inode + 1

	struct dir_entry dot;
	strcpy(dot.name, ".");
	dot.inode_num = 1; // this will be the current folders inode + 1

	struct dir_entry dotdot;
	strcpy(dotdot.name, "..");
	dotdot.inode_num = 0; // this will be the current folders inode

    fwrite(&home, sizeof(folder), 1, fout);
	fwrite(&dot, sizeof(dot), 1, fout);
	fwrite(&dotdot, sizeof(dotdot), 1, fout);

}

void get_inode_struct(FILE *fin, struct inode *inostr, int inode_num){
    
    fseek(fin, sizeof(struct sb), SEEK_SET);
    fseek(fin, sizeof(struct inode)*inode_num, SEEK_CUR);

    fread(inostr, sizeof(struct inode), 1, fin);
}

void get_dir_entry(FILE *fin, struct dir_entry *de, int db_num, int dir_entry_num){
    fseek(fin, sizeof(struct sb) + sizeof(struct inode)*32+ 512* db_num + dir_entry_num*sizeof(struct dir_entry), SEEK_SET);
    fread(de, sizeof(struct dir_entry), 1, fin);
}