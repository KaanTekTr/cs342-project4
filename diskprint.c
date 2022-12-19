#include <stdlib.h>
#include <stdio.h>
#include "diskprint.h"
#include <string.h>
#include <fcntl.h>

#define MAXFILENAME 256
#define BLOCKSIZE 4096

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
        printf("inodes count in the filesystem: %d\n", *s_inodes_count);

        int* s_blocks_count = &buffer[4];
        printf("blocks count in the filesystem: %d\n", *s_blocks_count);

        int* s_r_blocks_count = &buffer[8];
        printf("# of blocks reserved for the usage of the super user: %d\n", *s_r_blocks_count);

        int* s_free_blocks_count = &buffer[12];
        printf("free blocks count: %d\n", *s_free_blocks_count);

        int* s_free_inodes_count = &buffer[16];
        printf("free inodes count: %d\n", *s_free_inodes_count);

        int* s_first_data_block = &buffer[20];
        printf("id of the block containing superblock: %d\n", *s_first_data_block);

        int* s_log_block_size = &buffer[24];
        printf("the log block size is: %dKiB\n", *s_log_block_size);

        int* s_inodes_per_group = &buffer[40];
        printf("the number of inodes per group: %d\n", *s_inodes_per_group);

        short* s_mnt_count = &buffer[52];
        printf("the number of times the file system mounted: %d\n", *s_mnt_count);

        int* creator_os = &buffer[72];
        printf("the id of the os that created the file system: %d\n", *creator_os);
    }
}
