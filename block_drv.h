#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/blkdev.h>
#include "block_drv_ioctl.h"

#define MY_DEVICE_NAME "DanishevskiyNS"
#define DBGMSG(fmt, ...) printk(MY_DEVICE_NAME": "fmt, ##__VA_ARGS__)

    static int deviceMajorNumb = 0;

static int __init block_drv_init(void);
static void __exit  block_drv_exit(void);


static int block_drv_open(struct block_device* Device, fmode_t Mode);
static void block_drv_release(struct gendisk* GenDisk, fmode_t Mode);
static int block_drv_ioctl(struct block_device* Device, fmode_t Mode, unsigned int Cmd, unsigned long Arg);


static struct block_device_operations block_dev_functions = {
    .owner = THIS_MODULE,
    .open = block_drv_open,
    .release = block_drv_release,
    .ioctl = block_drv_ioctl
};

#define DEVICE_BUFFER_SIZE 1024
static struct _block_drv
{
    char* deviceBuffer;
    int deviceDataLength;
    struct request_queue *devReqQueue;
    struct gendisk *deviceGenDisk;
} dev;


//static char deviceBuffer [DEVICE_BUFFER_SIZE];
//static int deviceDataLength = 0;