#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include<fcntl.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <string.h>
#include "block_drv_ioctl.h"

int main()
{
    mknod("/dev/DanishevskiyNS_130", S_IFBLK, makedev(235,0));

    int device = open("/dev/DanishevskiyNS_130", O_RDWR, 0777);
    if(device < 0)
    {
        printf("Error\n");
    }

    //block_drv_ioctl_data data = {0};

    //printf("----IOCTL_BLOCK_DRV_GET----\n");

    //data.OutputData = malloc(1024);

    close(device);

    return 0;
}