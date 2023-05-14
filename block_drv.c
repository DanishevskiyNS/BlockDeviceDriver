#include "block_drv.h"

MODULE_LICENSE("Dual MIT/GPL");



static int __init block_drv_init(void)
{
    DBGMSG("block_drv_init called\n");
    deviceMajorNumb = register_blkdev(0, MY_DEVICE_NAME);

    if(deviceMajorNumb < 0)
    {
        DBGMSG("failed to register my block_drv\n");
        return -1;
    }
    deviceGenDisk = blk_alloc_disk(1);

    if(deviceGenDisk == NULL)
    {
        DBGMSG("alloc_disk failed\n");
        return -1;
    }
    deviceGenDisk->major = deviceMajorNumb;
    deviceGenDisk->first_minor = 0;
    deviceGenDisk->minors = 1;
    deviceGenDisk->fops = &block_dev_functions;
    //deviceGenDisk->queue = blk_init_queue(request_fun, NULL);
    deviceGenDisk->flags = GENHD_FL_NO_PART_SCAN;

    strcpy(deviceGenDisk->disk_name, MY_DEVICE_NAME"0");
    set_capacity(deviceGenDisk, 1024*512);

    add_disk(deviceGenDisk);

    memset(deviceBuffer, 0, DEVICE_BUFFER_SIZE);
    deviceDataLength = 0;

    DBGMSG("block_drv_init comleted\n");
    return 0;
} 

static void __exit block_drv_exit(void)
{
    DBGMSG("block_drv_exit called\n");

    del_gendisk(deviceGenDisk);
    put_disk(deviceGenDisk);

    unregister_blkdev(deviceMajorNumb, MY_DEVICE_NAME);

    DBGMSG("block_drv_exit comleted\n");
}

static int block_drv_open(struct block_device* Device, fmode_t Mode)
{
    DBGMSG("block_drv_open called\n");
    return 0;
}

static void block_drv_release(struct gendisk* GenDisk, fmode_t Mode)
{
    DBGMSG("block_drv_release called\n");
}

static int block_drv_ioctl(struct block_device* Device, fmode_t Mode, unsigned int Cmd, unsigned long Arg)
{
    DBGMSG("block_drv_ioctl called\n");

    block_drv_ioctl_data* data = (block_drv_ioctl_data*)Arg;

    switch(Cmd)
    {
        case IOCTL_BLOCK_DRV_GET:
        DBGMSG("IOCTL_BLOCK_DRV_GET\n");
        {
            size_t length = deviceDataLength > data->OutputLength ? data->OutputLength : deviceDataLength;
            copy_to_user(data->OutputData, deviceBuffer, length);

        }
        break;

        case IOCTL_BLOCK_DRV_SET:
        DBGMSG("IOCTL_BLOCK_DRV_SET\n");
        {
            size_t length = DEVICE_BUFFER_SIZE > data->InputLength ? data->InputLength: DEVICE_BUFFER_SIZE;
            deviceDataLength = length;

            memset(deviceBuffer, 0, DEVICE_BUFFER_SIZE);
            copy_from_user(deviceBuffer, data->InputData, length);
        }
        break;

        case IOCTL_BLOCK_DRV_GET_AND_SET:
        DBGMSG("IOCTL_BLOCK_DRV_GET_AND_SET\n");
        {
            size_t outputLength, inputLength;

            outputLength = deviceDataLength > data->OutputLength ? data->OutputLength : deviceDataLength;
            inputLength = DEVICE_BUFFER_SIZE > data->InputLength ? data->InputLength: DEVICE_BUFFER_SIZE;

            char tmpBuffer [DEVICE_BUFFER_SIZE];
            
            memset(tmpBuffer, 0, DEVICE_BUFFER_SIZE);
            copy_to_user(tmpBuffer,deviceBuffer, inputLength);

            memset(deviceBuffer, 0, DEVICE_BUFFER_SIZE);
            copy_from_user(deviceBuffer, data->InputData, inputLength);


            memset(deviceBuffer, 0, data->InputLength);
            copy_to_user(data->OutputData, tmpBuffer, outputLength);

            memset(tmpBuffer, 0, 0);

            deviceDataLength = inputLength;
        }
        break;

        case IOCTL_BLOCK_DBG_MESSAGE:
        DBGMSG("IOCTL_BLOCK_DRV_GET_AND_SET\n");
        {
            printk("%s", deviceBuffer);
        }
        break;
    }
    return 0;
}

module_init(block_drv_init);
module_exit(block_drv_exit);