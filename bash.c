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
void mkdir(FILE *fin, int currentDir, char name[28]);
void get_dir_entry(FILE *fin, struct dir_entry *de, int db_num, int dir_entry_num);
void jump_inode(FILE *f, struct inode *inode, int inode_num);
void jump_dir_entry(FILE *fin, struct dir_entry *de, int db_num, int dir_entry_num);

int getBit(int bitnum, int n);
int setBit(int bitnum, int bitmap);

int main() {
    
    FILE *fin = fopen("simplefs.bin", "r+");
	struct dir_entry de;
	//struct 
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
	else if (strcmp(command, "anan") == 0) {
		//get_dir_entry(fin, &de, 
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
            if(inostr.type == DIR) {
                
                printf("%*s%s", tab * 5, "", de.name);
                inum++;
                printf("\t\t%d\n", inum);
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
                strcat(currentDirName, "/");
                strcat(currentDirName, de.name);
                previousDir = currentDir;
                currentDir = de.inode_num;
                flag = true;
            }

            if(strcmp(name, "..") == 0) {
                get_inode_struct(fin, &inostr, currentDir);
                get_dir_entry(fin, &de, inostr.datablocks[0], 1);
                previousDir = de.inode_num;
                currentDir = de.inode_num;

                if(currentDir == 0) {
                    strcpy(currentDirName, "̃ ");
                }
                else {
                    get_inode_struct(fin, &inostr, currentDir);
                    get_dir_entry(fin, &de, inostr.datablocks[0], 1);
                    get_inode_struct(fin, &inostr, de.inode_num);

                    for(i = 0; i < inostr.size/32; i++){
                        get_dir_entry(fin,&de,inostr.datablocks[0],i);

                        if(de.inode_num == previousDir) {
                            strcpy(currentDirName, previousDirName);
                        }
                    }
                }
                return;
            }
            inum++; // kontrol edilecek gereksiz olabilir
        }
	}
    if(!flag)
        printf("-bash: cd: %s: No such file or directory\n", name);
}



void mkdir(FILE* fin, int currentDir, char name[28]) {
    struct sb sblock;
    struct inode inostr;
    struct dir_entry de;
    int inum = currentDir;
    fseek(fin, 0, SEEK_SET);
    fread(&sblock, sizeof(struct sb), 1, fin);

    #pragma region find_datablock
    bool flag = false;
    int db_array = 0;
    int db_num = 0;
    int db_bit;
        get(sblock.data_bitmap[0]);
    printf("\tdb_btmap: %d\n", sblock.data_bitmap[0]);

    for(db_array = 0; db_array < 10; db_array++) {
        while(db_num < 32) {
            if(getBit(db_num, sblock.data_bitmap[db_array]) == 0) {
                flag = true; // if datablock is empty break the loop so that we store the number
                db_bit = setBit(db_num, sblock.data_bitmap[db_array]); // setBit and return the bitmap to db_bit for ex; [1][1][0][1] -> [1][1][1][1] | so it should be stored in -> sblock.datablocks[] but needs override
                break;
            }
            db_num++;
        }
        if(flag) break;
    }
    #pragma endregion find_datablock
    get(sblock.data_bitmap[0]);

    printf("\tdb_array: %d\n", db_array);
    printf("\tdb_num: %d\n", db_num);
    printf("\tinode_bitmap: %d\n", sblock.inode_bitmap);
    printf("inodebitmap\n");
    get(sblock.inode_bitmap);
    printf("\n");
    #pragma region find_inode
    int inode_bit;
    int inode = 0;
    //printf("\ti: %d | bit: %d\n",i, getBit(i, sblock.inode_bitmap));

    for(inode = 0; inode < 32; inode++) {
        printf("\tinodes: %d", inode);
        if(getBit(inode, sblock.inode_bitmap) == 0) 
        {
            inode_bit = setBit(inode, sblock.inode_bitmap); // setBit and return the bitmap to inode_bit for ex; [1][1][0][1] -> [1][1][1][1] |so it should be stored in -> sblock.inode_bitmap but needs override
            break;
        }
    }
    #pragma endregion find_inode
    printf("\tinode: %d\n", inode);

    #pragma region new_inode
    struct inode dirInode;
	dirInode.type = DIR;
	dirInode.size = DIRENTRYSIZE * 2;
	dirInode.datablocks[db_array] = db_num;

    jump_inode(fin, &inostr, inode);
	fwrite(&dirInode, sizeof(dirInode), 1, fin);
    #pragma endregion new_inode

    #pragma region new_dir_entry
    struct dir_entry dot;
	strcpy(dot.name, ".");
	dot.inode_num = inode;

	struct dir_entry dotdot;
	strcpy(dotdot.name, "..");
	dotdot.inode_num = currentDir;
    	jump_dir_entry(fin, &de, db_array * db_num + db_num, 0);
	fwrite(&dot, sizeof(dot), 1, fin);
  	fwrite(&dotdot, sizeof(dotdot), 1, fin);
#pragma endregion new_dir_entry

    #pragma region parent_dir_entry
    struct dir_entry dir;
    get_inode_struct(fin, &inostr, currentDir);
    printf("\tname: %s\n", name);
	strcpy(dir.name, name);
	printf("dirname: %s\n", dir.name);
	dir.inode_num = inode;
  	printf("aaaaaaaaaaaaaaaaa: %d\n", inostr.size);
    jump_dir_entry(fin, &de, inostr.datablocks[0], inostr.size/32);
	fwrite(&dir, sizeof(dir), 1, fin);
	get_dir_entry(fin, &de, inostr.datablocks[0], 64);
	printf("iiiiiiiiii: %s\n", de.name);

#pragma endregion parent_dir_entry

    #pragma region parent_inode
    
    get_inode_struct(fin,&inostr,currentDir);
    
    inostr.size += DIRENTRYSIZE;
    jump_inode(fin, &inostr, currentDir);
	fwrite(&inostr, sizeof(inostr), 1, fin);
    #pragma endregion parent_inode

    #pragma region override_superblock
    sblock.inode_bitmap = inode_bit;
    sblock.data_bitmap[db_array] = db_bit;
    fseek (fin, 0, SEEK_SET);
	fwrite(&sblock, sizeof(sblock), 1, fin);
    #pragma endregion override_superblock
    printf("databitmap\n");
    get(sblock.data_bitmap[0]);
    printf("inodebitmap\n");
    get(sblock.inode_bitmap);
}

void get_inode_struct(FILE *fin, struct inode *inostr, int inode_num)
{    
    fseek(fin, sizeof(struct sb), SEEK_SET);
    fseek(fin, sizeof(struct inode)*inode_num, SEEK_CUR);
    
    fread(inostr, sizeof(struct inode), 1, fin);
}

void get_dir_entry(FILE *fin, struct dir_entry *de, int db_num, int dir_entry_num)
{
    fseek(fin, sizeof(struct sb) + sizeof(struct inode)*32+ 512* db_num + dir_entry_num*sizeof(struct dir_entry), SEEK_SET);
    fread(de, sizeof(struct dir_entry), 1, fin);
}

void jump_inode(FILE *fin, struct inode *inostr, int inode_num)
{
	fseek(fin, sizeof(struct sb) + sizeof(struct inode) * inode_num, SEEK_SET);
}

void jump_dir_entry(FILE *fin, struct dir_entry *de, int db_num, int dir_entry_num) 
{
	fseek(fin, sizeof(struct sb) + sizeof(struct inode) * 32 + 512 * db_num + dir_entry_num * sizeof(struct dir_entry), SEEK_SET);
}
		

int getBit(int bitnum, int n) { return (n >> bitnum) & 1; } 
int setBit(int bitnum, int bitmap) { return bitmap ^ (1 << bitnum); } 

int get(int bitmap) 
{ 
    int count = 0;
    for (int i = 31; i >= 0; i--) { 
        count++;
        int k = bitmap >> i; 
        if (k & 1) 
            printf("1"); 
        else
        {
            printf("0"); 
        } 
    }
    printf("\n");
} 
