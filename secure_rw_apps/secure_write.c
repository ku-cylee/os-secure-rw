#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"
#include "unistd.h"
#include "errno.h"
#include "sys/syscall.h"

#define SYS_SEC_WRITE   451
#define TOTAL_SIZE      4096

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Not enough parameters\n");
        return -1;
    }

    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
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

    fprintf(stdout, "Input data: ");
    fgets(buf, TOTAL_SIZE, stdin);

    syscall(SYS_SEC_WRITE, fd, buf);
    fprintf(stdout, "Data is written to file: %s\n", argv[1]);

    free(buf);
    close(fd);

    return 0;
}
