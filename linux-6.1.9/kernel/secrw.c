#include "linux/kernel.h"
#include "linux/syscalls.h"

SYSCALL_DEFINE2(sec_read, int, fd, char *, buf) {
	printk("sec_read called\n");
	return 0;
}

SYSCALL_DEFINE2(sec_write, int, fd, char *, buf) {
	printk("sec_write called\n");
	return 0;
}
