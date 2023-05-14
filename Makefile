TARGET := DANISHEVSKIY_BLK_DRV
PWD := $(shell pwd)
KERNEL := $(shell uname -r)

obj-m := $(TARGET).o
$(TARGET)-objs += block_drv.o

all:
	make -C /lib/modules/$(KERNEL)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KERNEL)/build M=$(PWD) clean