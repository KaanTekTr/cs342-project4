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
    offset = 0 * blocknum;
    
    lseek(fd, offset, SEEK_SET);
    n = read (fd, buffer, BLOCKSIZE);
    if (n == BLOCKSIZE) {
        printf("block read success\n");
    }
}
