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
    mknod("/dev/DanishevskiyNS0", S_IFBLK, makedev(235,0));

    int device = open("/dev/DanishevskiyNS0", O_RDWR, 0777);
    if(device < 0)
    {
        printf("Error\n");
    }

    block_drv_ioctl_data data = {0};


    printf("----IOCTL_BLOCK_DRV_SET----\n");

    data.InputData = "Data for Driver";
    data.InputLength = strlen(data.InputData) + 1;
    ioctl(device, IOCTL_BLOCK_DRV_SET, &data);

    
    printf("----IOCTL_BLOCK_DRV_GET_AND_SET----\n");
    data.OutputData = malloc(1024);
    data.OutputLength = 1024;
    memset(data.OutputData, 0, 1024);
    data.InputData = "New data for driver";
    data.InputLength = strlen(data.InputData);

    ioctl(device, IOCTL_BLOCK_DRV_GET_AND_SET, &data);
    printf("data from driver: %s\n", data.OutputData);
       
    printf("----IOCTL_BLOCK_DRV_GET----\n");
    memset(data.OutputData, 0, data.OutputLength);

    ioctl(device, IOCTL_BLOCK_DRV_GET, &data);
    printf("data from driver: %s\n", data.OutputData);


    printf("----IOCTL_BLOCK_DBG_MESSAGE----\n");
    ioctl(device, IOCTL_BLOCK_DBG_MESSAGE);


    close(device);

    return 0;
}