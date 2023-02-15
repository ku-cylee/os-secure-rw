obj-m := io_monitor.o

KDIR = ../linux-6.1.9/

all:
	$(MAKE) -C $(KDIR) M=$(PWD) SUBDIRS=$(PWD) modules

load:
	sudo insmod io_monitor.ko

unload:
	sudo rmmod io_monitor

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) SUBDIRS=$(PWD) clean
