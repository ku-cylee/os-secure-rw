#define _GNU_SOURCE

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "fcntl.h"
#include "unistd.h"
#include "malloc.h"

#include "errno.h"

#define DIRECT_IO

#ifdef DIRECT_IO
#define OPEN_FLAG   O_RDONLY | O_DIRECT
#else
#define OPEN_FLAG   O_RDONLY
#endif

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Not enough parameters\n");
        return -1;
    }

    int fd = open(argv[1], OPEN_FLAG);
    if (fd < 0) {
        fprintf(stderr, "Failed to open file %s (Error #%d)\n", argv[1], errno);
        return -1;
    }

    int offset = atoi(argv[2]);
    int length = atoi(argv[3]);

    char *buf = memalign(getpagesize(), getpagesize());
    if (!buf) {
        fprintf(stderr, "Failed to create buffer (Error #%d)\n", errno);
        close(fd);
        return -1;
    }

    int lseek_result = lseek(fd, offset, SEEK_SET);
    if (lseek_result < 0) {
        fprintf(stderr, "Failed to seek to offset %d (Error #%d)\n", offset, errno);
        free(buf);
        close(fd);
        return -1;
    }

    int read_result = read(fd, buf, length);
    if (read_result < 0) {
        fprintf(stderr, "Failed to read the file with length %d (Error #%d)\n", length, errno);
        free(buf);
        close(fd);
        return -1;
    }

    fprintf(stdout, "Read success: %d bytes\n", read_result);

    free(buf);
    close(fd);

    return 0;
}
