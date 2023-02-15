#include "linux/init.h"
#include "linux/kernel.h"
#include "linux/kprobes.h"
#include "linux/module.h"
#include "linux/ktime.h"

static int io_handler(struct pt_regs *regs, char op) {
    long length;
    long long timestamp, offset;
    const unsigned char *filename;

    struct file *file;
    struct timespec64 ts;

    file = (struct file *)regs->di;
    filename = file->f_path.dentry->d_name.name;

    ts = ktime_to_timespec64(ktime_get());
    timestamp = ts.tv_sec * 1000000000 + ts.tv_nsec;

    offset = regs->si;
    length = regs->dx;

    printk("OP[%c] NAME[%s] TIME[%lld]ns OFF[%lld] LEN [%ld]\n", op, filename, timestamp, offset, length);
    return 0;
}

static int read_handler(struct kprobe *kp, struct pt_regs *regs) {
    return io_handler(regs, 'R');
}

static int write_handler(struct kprobe *kp, struct pt_regs *regs) {
    return io_handler(regs, 'W');
}

static struct kprobe kprobe_read = {
    .symbol_name = "vfs_read",
    .pre_handler = read_handler,
};

static struct kprobe kprobe_write = {
    .symbol_name = "vfs_write",
    .pre_handler = write_handler,
};

static int __init iom_init(void) {
    register_kprobe(&kprobe_read);
    register_kprobe(&kprobe_write);
    return 0;
}

static void __exit iom_exit(void) {
    unregister_kprobe(&kprobe_read);
    unregister_kprobe(&kprobe_write);
}

module_init(iom_init);
module_exit(iom_exit);

MODULE_AUTHOR("Chanyoung Lee");
MODULE_DESCRIPTION("CARES-OS");
MODULE_LICENSE("GPL");
MODULE_VERSION("NEW");
