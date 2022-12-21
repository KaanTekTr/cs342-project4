#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "ext2_structures.h"
#include <time.h>

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

    // start the timer
    time_t t;
    struct timeval startTime;
    struct timeval currTime;
    srand((unsigned) time(&t));
    gettimeofday(&startTime, NULL);

    // access data from different blocks
    for ( int i = 0; i < 3000; i++ ) {
        blocknum = i;
        offset = BLOCKSIZE * blocknum + 1024;
        
        // read the information related to the superblock
        lseek(fd, offset, SEEK_SET);
        n = read (fd, buffer, BLOCKSIZE);
    }

    gettimeofday(&currTime, NULL);
    printf("took: %ld ms\n", (currTime.tv_sec - startTime.tv_sec) * 1000 + ((currTime.tv_usec - startTime.tv_usec) / 1000));

    gettimeofday(&startTime, NULL);
    for ( int i = 0; i < 3000; i++ ) {
        blocknum = 0;
        offset = BLOCKSIZE * blocknum + i;
        
        // read the information related to the superblock
        lseek(fd, offset, SEEK_SET);
        n = read (fd, buffer, BLOCKSIZE);
    }

    gettimeofday(&currTime, NULL);
    printf("took: %ld ms\n", (currTime.tv_sec - startTime.tv_sec) * 1000 + ((currTime.tv_usec - startTime.tv_usec) / 1000));
}