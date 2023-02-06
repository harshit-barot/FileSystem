// #include<stdio.h>

// meta information about the filesystem
// number of inodes 
// number of disk blocks
// size of the disk blocks 
struct superblock {
   int num_inodes; 
   int num_blocks; 
   int size_blocks;

};

struct inode {
   int size;
   int first_block;
   char name[8];
};

struct disk_block { 
   int next_block_num;
   char data[512];
};

void create_fs ();// initialize new filesystem 
void mount_fs (); // load a file system 
void sync_fs (); // write the file system
int allocate_file(char name[8]); //return file number
void print_fs(); 