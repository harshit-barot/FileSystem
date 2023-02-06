#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include "fs.h"

struct superblock sb;
struct inode *inodes;
struct disk_block *dbs;

int find_empty_inode(){
    int i;
    for(i=0;i<sb.num_inodes;i++){
        if(inodes[i].first_block==-1){
            return i;
        }
    }
    return -1;
}

int find_empty_block(){
    int i;
    for(i=0;i<sb.num_blocks;i++){
        if(dbs[i].next_block_num==-1){
            return i;
        }
    }
    return -1;
}

// initialize new filesystem 
void create_fs(){
    sb.num_inodes=10;
    sb.num_blocks=100;
    sb.size_blocks=sizeof(struct disk_block);
    
    inodes=malloc(sizeof(struct inode) *sb.num_inodes);
    for(int i=0;i<sb.num_inodes;i++){
        inodes[i].size=-1;
        inodes[i].first_block=-1;
        strcpy(inodes[i].name,"emptyfi");
        }
    
    dbs=malloc(sizeof(struct disk_block) *sb.num_blocks);
    for(int i=0;i<sb.num_blocks;i++){
        dbs[i].next_block_num=-1;
        }    
    
}

// load a file system 
void mount_fs (){
    FILE *file;
    file=fopen("fs_data","r");
    inodes=malloc(sizeof(struct inode) *sb.num_inodes);
    dbs=malloc(sizeof(struct disk_block) *sb.num_blocks);
    
    fread(&sb,sizeof(struct superblock),1,file);
    fread(inodes,sizeof(struct inode),sb.num_inodes,file);
    fread(dbs,sizeof(struct disk_block),sb.num_blocks,file);
    
    
    fclose(file);
    
} 

// write the file system
void sync_fs (){
    FILE *file;
    
    file=fopen("fs_data","w+");
    // superblock
    fwrite(&sb,sizeof(struct superblock),1,file);
    // inodes
    fwrite(inodes,sizeof(struct inode),sb.num_inodes,file);
    // disk blocks
    fwrite(dbs,sizeof(struct disk_block),sb.num_blocks,file);
    

    fclose(file);

} 

void print_fs(){
    printf("Superblock info:\n");
    printf("\t%d num inodes\n",sb.num_inodes);
    printf("\t%d num blocks\n",sb.num_blocks);
    printf("\t%d size blocks\n",sb.size_blocks);

    printf("inodes:\n");
     for(int i=0;i<sb.num_inodes;i++){
        printf("\t size: %d \t block: %d name:%s\n",inodes[i].size,inodes[i].first_block,inodes[i].name);
        }
    for(int i=0;i<sb.num_blocks;i++){
        printf("\t block number: %d next block number:%d\n",i,dbs[i].next_block_num);
        }

}



int allocate_file(char name[8]){
    //find an empty file node
    int in=find_empty_inode();
    //find and claim a disc block 
    int block=find_empty_block();
    // printf("%d %d %s\t herex",in,block,name);
    //claim it
    inodes[in].first_block=block;
    dbs[block].next_block_num=-2;

    strcpy(inodes[in].name,name);
    

    //return the file discriptor
    return in;
}

