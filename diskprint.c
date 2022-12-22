#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "ext2_structures.h"
#include <time.h>

#define MAXFILENAME 256
#define BLOCKSIZE 4096

static const char *Weekdays[] = {
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday"
     };

int main(int argc, char *argv[]) {
    char devname[MAXFILENAME];
    strcpy (devname, argv[1]);
    int fd;
    int n;
    off_t offset;
    int blocknum;
    unsigned char buffer[BLOCKSIZE];

    fd = open(devname, O_RDONLY);
    if (fd < 0) {
        printf ("can not open disk file\n");
        exit(1);
    }

    blocknum = 0;
    offset = 0 * blocknum + 1024;
    
    // read the information related to the superblock
    lseek(fd, offset, SEEK_SET);
    n = read (fd, buffer, BLOCKSIZE);
    if (n == BLOCKSIZE) {
        int* s_inodes_count = &buffer[0];
        printf("inodes count in the filesystem (s_inodes_count): %d\n", *s_inodes_count);

        int* s_blocks_count = &buffer[4];
        printf("blocks count in the filesystem (s_blocks_count): %d\n", *s_blocks_count);

        int* s_r_blocks_count = &buffer[8];
        printf("# of blocks reserved for the usage of the super user (s_r_blocks_count): %d\n", *s_r_blocks_count);

        int* s_free_blocks_count = &buffer[12];
        printf("free blocks count (s_free_blocks_count): %d\n", *s_free_blocks_count);

        int* s_free_inodes_count = &buffer[16];
        printf("free inodes count (s_free_inodes_count): %d\n", *s_free_inodes_count);

        int* s_first_data_block = &buffer[20];
        printf("id of the block containing superblock (s_first_data_block): %d\n", *s_first_data_block);

        int* s_log_block_size = &buffer[24];
        printf("the log block size is (s_log_block_size): %dKiB\n", *s_log_block_size);

        int* s_inodes_per_group = &buffer[40];
        printf("the number of inodes per group (s_inodes_per_group): %d\n", *s_inodes_per_group);

        short* s_mnt_count = &buffer[52];
        printf("the number of times the file system mounted (s_mnt_count): %d\n", *s_mnt_count);

        int* creator_os = &buffer[72];
        printf("the id of the os that created the file system (creator_os): %d\n", *creator_os);

        int* s_first_ino = &buffer[84];
        printf("Index of the first usable inode (s_first_ino): %d\n", *s_first_ino);
    }

    blocknum = 1;
    offset = BLOCKSIZE * blocknum + 0;
    int inodeTableBlock = 0;
    // read the information related to the first group descriptor
    lseek(fd, offset, SEEK_SET);
    n = read(fd, buffer, BLOCKSIZE);
    if (n == BLOCKSIZE) {
        struct ext2_group_desc* group_descriptor = (struct ext2_group_desc*) &buffer[0];

        printf("inodes table block: %u\n", group_descriptor->bg_inode_table); // prints that the inode table starts at block 4
        inodeTableBlock = group_descriptor->bg_inode_table;
    }


    blocknum = inodeTableBlock;
    offset = BLOCKSIZE * blocknum + 128; // take the second inode
    
    // read the information related to the root directory inode
    lseek(fd, offset, SEEK_SET);
    n = read (fd, buffer, BLOCKSIZE);
    int rootDirectoryBlock = 0;
    if (n == BLOCKSIZE) {
        struct ext2_inode* root_inode = (struct ext2_inode*) &buffer[0];

        printf("i: %u\n", root_inode->i_block[0]); // prints that the root directory starts at block 36
        printf("size: %u\n", root_inode->i_size); // prints that the size of the file is 4096
        rootDirectoryBlock = root_inode->i_block[0];
    }

    blocknum = rootDirectoryBlock;
    offset = BLOCKSIZE * blocknum;
    lseek(fd, offset, SEEK_SET);
    n = read (fd, buffer, BLOCKSIZE);
    if (n == BLOCKSIZE) {
        int location = 0;

        while ( location < BLOCKSIZE ) {
            struct ext2_dir_entry* first_dir = (struct ext2_dir_entry*) &buffer[location];
            char filename[EXT_NAME_LEN + 1];
            
            if ( first_dir->inode == 0 ) {
                break; // exit when you encounter 0
            }

            memcpy(filename, first_dir->name, first_dir->name_len);
            filename[first_dir->name_len] = '\0';

            //printf("entry len: %u, ", first_dir->rec_len);
            printf("inode number: %u ", first_dir->inode);
            printf("filename: %s\n", filename);

            location += first_dir->rec_len;
        }
    }

    blocknum = rootDirectoryBlock;
    offset = BLOCKSIZE * blocknum;
    unsigned char inode_buffer[4096];
    lseek(fd, offset, SEEK_SET);
    n = read (fd, buffer, BLOCKSIZE);
    if (n == BLOCKSIZE) {
        int location = 0;

        while ( location < BLOCKSIZE ) {
            struct ext2_dir_entry* first_dir = (struct ext2_dir_entry*) &buffer[location];
            char filename[EXT_NAME_LEN + 1];

            memcpy(filename, first_dir->name, first_dir->name_len);
            filename[first_dir->name_len] = '\0';

            //printf("entry len: %u, ", first_dir->rec_len);
            int inode_number = first_dir->inode;
            
            int inode_block_number = inode_number / 32; // since 32 inodes fit into a single block
            int inode_block_offset = inode_number % 32;

            blocknum = 4 + inode_block_number; // find the block containing the inode
            offset = BLOCKSIZE * blocknum + 128 * (inode_block_offset - 1);

            printf("-----file-----\n");
            lseek(fd, offset, SEEK_SET);
            n = read (fd, inode_buffer, BLOCKSIZE);
            if (n == BLOCKSIZE) {
                struct ext2_inode* inode = (struct ext2_inode*) &inode_buffer[0];

                printf("i_mode: %u\n", inode->i_mode);
                printf("i_uid: %u\n", inode->i_uid);
                printf("i_size: %u\n", inode->i_size);
                printf("i_gid: %u\n", inode->i_gid);
                printf("i_blocks: %u\n", inode->i_blocks);
                printf("i_links_count: %u\n", inode->i_links_count);
                printf("i_faddr: %u\n", inode->i_faddr);
                printf("i_generation: %u\n", inode->i_generation);
                printf("i_file_acl: %u\n", inode->i_file_acl);
                printf("i_dir_acl: %u\n", inode->i_dir_acl);

                time_t curSec = inode->i_atime;
                struct tm *curDate;
                char dateString[32];
                
                curDate = localtime(&curSec);
                sprintf(dateString,
                    "%s-%02d/%02d/%d-%02d:%02d:%02d",
                    Weekdays[curDate->tm_wday],
                    curDate->tm_mday,
                    curDate->tm_mon+1,
                    curDate->tm_year+1900,                    
                    curDate->tm_hour,
                    curDate->tm_min,
                    curDate->tm_sec);

                printf("(i_atime): %s\n",dateString);

                curSec = inode->i_ctime;
                curDate = localtime(&curSec);
                sprintf(dateString,
                    "%s-%02d/%02d/%d-%02d:%02d:%02d",
                    Weekdays[curDate->tm_wday],
                    curDate->tm_mday,
                    curDate->tm_mon+1,
                    curDate->tm_year+1900,                    
                    curDate->tm_hour,
                    curDate->tm_min,
                    curDate->tm_sec);
                printf("(i_ctime): %s\n",dateString);

                curSec = inode->i_mtime;

                curDate = localtime(&curSec);
                sprintf(dateString,
                    "%s-%02d/%02d/%d-%02d:%02d:%02d",
                    Weekdays[curDate->tm_wday],
                    curDate->tm_mday,
                    curDate->tm_mon+1,
                    curDate->tm_year+1900,                    
                    curDate->tm_hour,
                    curDate->tm_min,
                    curDate->tm_sec);
                printf("(i_mtime): %s\n",dateString);
            }

            location += first_dir->rec_len;
        }
    }
}
