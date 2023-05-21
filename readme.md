This repo contains simple block device driver for linux. Driver uses only ioctls. Users App are used to interact with the driver. They communicate with each other using shared memory and signal.
Preparetion:
```
sudo apt update
sudo apt upgrade
```
Install kernel linux headers
```
sudo apt install linux-headers-$(uname -r)
```
Install gcc and make utilities:
```
sudo apt install gcc
sudo apt install make
``` 
Driver installation and launching:
```
make all
sudo insmod DANISHEVSKIY_BLK_DRV.ko
```
To delete driver and clean folder you can use rmmod and make clean as follows
```
sudo rmmod DANISHEVSKIY_BLK_DRV
make clean
```
In order to interact with the driver I suggest using to Apps. user.c interact with the driver using ioctls. Also it uploads data from driver to shared memory. Second App, user2.c, takes information from shared memory and displays it. To let user2 know, that shared memory isn't empty user send FPE signal to it.
Firstly get executable files;
```
gcc user.c -o App1
gcc user2.c -o App2
After that launch App2:
```
sudo ./App2
```
When App2 is active, it displays its pid. It is needed to send SUGFPE from App1. For example:
```
user2 pid: 3524
waiting for signal...
```
Next step is to launch App1 and parse user2 pid. For example:
```
sudo ./App1 3524
```