#include <stdio.h>
#include "structures.h"
#include <string.h>
#include <stdbool.h>

int inum = 0;
int dirnum = 0;
int tab = 0; 
int currentDir = 0;
int previousDir = 0;
char name[28];
char currentDirName[28] = "~";
char previousDirName[28] = "~";
void ls(FILE *file, int inum, int tab);
void cd(FILE *file, char name[28], int inum);
void get_inode_struct(FILE *f, struct inode *inode,int inode_num);
//void mkdir(FILE *fin, char name[28]);
void mkdir(FILE *fin, int currentDir, char name[28]);
void get_dir_entry(FILE *fin, struct dir_entry *de, int db_num, int dir_entry_num);

int main() {
    
    FILE *fin = fopen("simplefs.bin", "r");

    char command[32];
    while(1) {

        printf("\033[1;32m\03user@DESKTOP:\033[1;34m\03%s\033[0m\03$ ", currentDirName);
		scanf("%s",command);
        if(strcmp(command, "ls") == 0) {
            printf("/\n");
            ls(fin, 0, tab);
        }
        else if (strcmp(command,"mkdir") == 0) {
            //mkdir(fin, name);
            scanf("%s", name);
            mkdir(fin, currentDir, name);
        }
        else if (strcmp(command,"cd") == 0) {
            //mkdir(fin, name);
            scanf("%s", name);
            cd(fin, name, currentDir);
        }
        else {
            printf("%s: command not found\n", command);
        }

    }
}

void ls(FILE *fin, int inum, int tab) {
	
    struct inode inostr;
	struct dir_entry de;
	get_inode_struct(fin,&inostr,inum);
    tab++;
	int i;
	for(i = 0; i < inostr.size/32; i++){
        get_dir_entry(fin,&de,inostr.datablocks[0],i);
        
        if(strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0) 
        {
            if(inostr.type == 1) {
                
                printf("%*s%s\n", tab * 5, "", de.name);
                inum++;
                ls(fin, inum, tab);
            }
        }
	}
    tab--;
}

void cd(FILE *fin, char name[28], int inum) {
	char str1[28];
    char str2[28];
    int ret;
    bool flag = false;
    //-------------------------------\\ 
    
    struct inode inostr;
	struct dir_entry de;
	get_inode_struct(fin,&inostr,inum);

	int i;
	for(i = 0; i < inostr.size/32; i++){
        get_dir_entry(fin,&de,inostr.datablocks[0],i);
        
        if(inostr.type == 1) {
            strcpy(str1, de.name);
            strcpy(str2, name);
            ret = strcmp(str1, str2);
            if(ret == 0) {
                strcpy(previousDirName, currentDirName);
                strcpy(currentDirName, de.name);
                previousDir = currentDir;
                currentDir = de.inode_num;
                flag = true;
            }

            if(strcmp(name, "..") == 0) {
                strcpy(currentDirName, previousDirName);
                currentDir = previousDir;
                return;
            }
            inum++;
        }
	}
    if(!flag)
        printf("-bash: cd: %s: No such file or directory\n", name);
}



void mkdir(FILE* fin, int currentDir, char name[28]) {
	struct dir_entry de;

    for(int i = 0; i < 15; i++) {
                    get_dir_entry(fin,&de,i,0);

        printf("%d\n", de.inode_num);
        if(strlen(de.name) == 0) {printf("ding dong\n");}

    }

/*
    struct inode inostr;
    int inum = currentDir;
	get_inode_struct(fin,&inostr,inum);
    while(inostr.size != 0) {
        get_inode_struct(fin,&inostr,inum);
        printf("inostrsize: %d\n", inostr.size);
        inum++;
    }

    struct inode dirInode;
	inHome.type = DIR;
	inHome.size = DIRENTRYSIZE * 2;
	inHome.datablocks[0] = (inum-1);

	fwrite(&inHome, sizeof(inHome), 1, fout);
        
    get_inode_struct(fin,&inostr,currentDir);
    inostr.size += DIRENTRYSIZE;

    struct dir_entry dot;
	strcpy(dot.name, ".");
	dot.inode_num = (inum-1);

	struct dir_entry dotdot;
	strcpy(dotdot.name, "..");
	dotdot.inode_num = currentDir;

    struct dir_entry dir;
	strcpy(dotdot.name, name);
	dotdot.inode_num = currentDir;
*/

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
