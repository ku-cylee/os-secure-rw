#include "linux/fs.h"
#include "linux/kernel.h"
#include "linux/slab.h"
#include "linux/syscalls.h"

#define DATA_SIZE	4000
#define CHKS_SIZE	96
#define TOTAL_SIZE	DATA_SIZE + CHKS_SIZE

static void generate_checksum(char *data_buf, char *chks_buf) {
	int data_idx, chks_idx;
	for (chks_idx = 0; chks_idx < CHKS_SIZE; chks_idx++) {
		chks_buf[chks_idx] = 0;
	}

	for (data_idx = 0; data_idx < DATA_SIZE; data_idx++) {
		chks_buf[chks_idx] += data_buf[data_idx];
		chks_idx = (chks_idx + 1) % CHKS_SIZE;
	}
}

static int compare_checksum(char *orig_buf, char *new_buf) {
	int idx;
	for (idx = 0; idx < CHKS_SIZE; idx++) {
		int diff = new_buf[idx] - orig_buf[idx];
		if (diff) return diff;
	}
	return 0;
}

SYSCALL_DEFINE2(sec_read, unsigned int, fd, char *, buf) {
	int res;

	char *read_buf = kmalloc(TOTAL_SIZE, GFP_KERNEL);
	char *checksum_buf = kmalloc(CHKS_SIZE, GFP_KERNEL);
	if (!read_buf || !checksum_buf) return -1;

	ksys_read(fd, buf, TOTAL_SIZE);

	if ((res = copy_from_user(read_buf, buf, TOTAL_SIZE)) != 0) {
		kfree(read_buf);
		return -1;
	}

	generate_checksum(read_buf, checksum_buf);
	return compare_checksum(read_buf + DATA_SIZE, checksum_buf);
}

SYSCALL_DEFINE2(sec_write, unsigned int, fd, char *, buf) {
	int res;

	char *write_buf = kmalloc(TOTAL_SIZE, GFP_KERNEL);
	if (!write_buf) return -1;

	if ((res = copy_from_user(write_buf, buf, DATA_SIZE)) != 0) {
		kfree(write_buf);
		return -1;
	}

	generate_checksum(write_buf, write_buf + DATA_SIZE);

	if ((res = copy_to_user(buf, write_buf, TOTAL_SIZE)) != 0) {
		kfree(write_buf);
		return -1;
	}

	res = ksys_write(fd, buf, TOTAL_SIZE);

	kfree(write_buf);

	return res;
}
