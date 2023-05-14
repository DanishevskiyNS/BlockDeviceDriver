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
    dev.deviceGenDisk = blk_alloc_disk(1);

    if(dev.deviceGenDisk == NULL)
    {
        DBGMSG("alloc_disk failed\n");
        return -1;
    }
    dev.deviceGenDisk->major = deviceMajorNumb;
    dev.deviceGenDisk->first_minor = 0;
    dev.deviceGenDisk->minors = 1;
    dev.deviceGenDisk->fops = &block_dev_functions;
    //deviceGenDisk->queue = blk_init_queue(request_fun, NULL);
    dev.deviceGenDisk->flags = GENHD_FL_NO_PART_SCAN;

    strcpy(dev.deviceGenDisk->disk_name, MY_DEVICE_NAME"0");
    set_capacity(dev.deviceGenDisk, 1024*512);

    add_disk(dev.deviceGenDisk);

    dev.deviceBuffer = kzalloc(DEVICE_BUFFER_SIZE, GFP_KERNEL);

    //memset(deviceBuffer, 0, DEVICE_BUFFER_SIZE);
    dev.deviceDataLength = 0;

    DBGMSG("block_drv_init comleted\n");
    return 0;
} 

static void __exit block_drv_exit(void)
{
    DBGMSG("block_drv_exit called\n");

    del_gendisk(dev.deviceGenDisk);
    put_disk(dev.deviceGenDisk);

    unregister_blkdev(deviceMajorNumb, MY_DEVICE_NAME);

    kfree(dev.deviceBuffer);

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
            size_t length = dev.deviceDataLength > data->OutputLength ? data->OutputLength : dev.deviceDataLength;
            copy_to_user(data->OutputData, dev.deviceBuffer, length);

        }
        break;

        case IOCTL_BLOCK_DRV_SET:
        DBGMSG("IOCTL_BLOCK_DRV_SET\n");
        {
            size_t length = DEVICE_BUFFER_SIZE > data->InputLength ? data->InputLength: DEVICE_BUFFER_SIZE;
            dev.deviceDataLength = length;

            memset(dev.deviceBuffer, 0, DEVICE_BUFFER_SIZE);
            copy_from_user(dev.deviceBuffer, data->InputData, length);
        }
        break;

        case IOCTL_BLOCK_DRV_GET_AND_SET:
        DBGMSG("IOCTL_BLOCK_DRV_GET_AND_SET\n");
        {
            size_t outputLength, inputLength;

            outputLength = dev.deviceDataLength > data->OutputLength ? data->OutputLength : dev.deviceDataLength;
            inputLength = DEVICE_BUFFER_SIZE > data->InputLength ? data->InputLength: DEVICE_BUFFER_SIZE;

            char tmpBuffer [DEVICE_BUFFER_SIZE];
            
            memset(tmpBuffer, 0, DEVICE_BUFFER_SIZE);
            copy_to_user(tmpBuffer,dev.deviceBuffer, inputLength);

            memset(dev.deviceBuffer, 0, DEVICE_BUFFER_SIZE);
            copy_from_user(dev.deviceBuffer, data->InputData, inputLength);


            memset(dev.deviceBuffer, 0, data->InputLength);
            copy_to_user(data->OutputData, tmpBuffer, outputLength);

            memset(tmpBuffer, 0, 0);

            dev.deviceDataLength = inputLength;
        }
        break;

        case IOCTL_BLOCK_DBG_MESSAGE:
        DBGMSG("IOCTL_BLOCK_DRV_GET_AND_SET\n");
        {
            printk("%s", dev.deviceBuffer);
        }
        break;
    }
    return 0;
}

module_init(block_drv_init);
module_exit(block_drv_exit);