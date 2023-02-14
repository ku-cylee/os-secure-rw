#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"
#include "unistd.h"
#include "errno.h"
#include "sys/syscall.h"

#define SYS_SEC_READ    452
#define TOTAL_SIZE      4096

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Not enough parameters\n");
        return -1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Failed to open file %s (Error #%d)\n", argv[1], errno);
        return -1;
    }

    char *buf = (char *)malloc(TOTAL_SIZE);
    if (!buf) {
        fprintf(stderr, "Failed to create buffer (Error #%d)\n", errno);
        close(fd);
        return -1;
    }

    int read_result = syscall(SYS_SEC_READ, fd, buf);
    fprintf(stdout, "File %s is %s.\n", argv[1], read_result ? "corrupted" : "secure");

    free(buf);
    close(fd);

    return 0;
}
