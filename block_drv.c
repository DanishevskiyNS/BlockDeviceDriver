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
    dev.GenDisk = blk_alloc_disk(1);

    if(dev.GenDisk == NULL)
    {
        DBGMSG("alloc_disk failed\n");
        return -1;
    }
    dev.GenDisk->major = deviceMajorNumb;
    dev.GenDisk->first_minor = 0;
    dev.GenDisk->minors = 1;
    dev.GenDisk->fops = &block_dev_functions;
    dev.GenDisk->flags = GENHD_FL_NO_PART;

    strcpy(dev.GenDisk->disk_name, MY_DEVICE_NAME"0");
    set_capacity(dev.GenDisk, 1024*512);

    add_disk(dev.GenDisk);

    dev.Buffer = kzalloc(DEVICE_BUFFER_SIZE, GFP_KERNEL);

    dev.DataLength = 0;


    DBGMSG("block_drv_init comleted\n");
    return 0;
} 

static void __exit block_drv_exit(void)
{
    DBGMSG("block_drv_exit called\n");

    kfree(dev.Buffer);

    del_gendisk(dev.GenDisk);
    put_disk(dev.GenDisk);

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
            size_t length = dev.DataLength > data->OutputLength ? data->OutputLength : dev.DataLength;
            copy_to_user(data->OutputData, dev.Buffer, length);

        }
        break;

        case IOCTL_BLOCK_DRV_SET:
        DBGMSG("IOCTL_BLOCK_DRV_SET\n");
        {
            size_t length = DEVICE_BUFFER_SIZE > data->InputLength ? data->InputLength: DEVICE_BUFFER_SIZE;
            dev.DataLength = length;

            memset(dev.Buffer, 0, DEVICE_BUFFER_SIZE);
            copy_from_user(dev.Buffer, data->InputData, length);
            DBGMSG("Data from user: %s", dev.Buffer);
        }
        break;

        case IOCTL_BLOCK_DRV_GET_AND_SET:
        DBGMSG("IOCTL_BLOCK_DRV_GET_AND_SET\n");
        {
            size_t outputLength, inputLength;

            outputLength = dev.DataLength > data->OutputLength ? data->OutputLength : dev.DataLength;
            inputLength = DEVICE_BUFFER_SIZE > data->InputLength ? data->InputLength: DEVICE_BUFFER_SIZE;

            copy_to_user(data->OutputData,dev.Buffer, inputLength);

            memset(dev.Buffer, 0, DEVICE_BUFFER_SIZE);
            copy_from_user(dev.Buffer, data->InputData, inputLength);
            
            DBGMSG("Data from user: %s", dev.Buffer);

            dev.DataLength = inputLength;
        }
        break;

        case IOCTL_BLOCK_DBG_MESSAGE:
        DBGMSG("IOCTL_BLOCK_DBG_MESSAGE\n");
        {
            DBGMSG("Device buffer conten: %s\n", dev.Buffer);
        }
        break;
    }
    return 0;
}

module_init(block_drv_init);
module_exit(block_drv_exit);