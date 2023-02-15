# OS Assignment 1: System Call

## Performance Evaluation of Buffered I/O and Direct I/O

Create a random file with specific size using `truncate`.
```sh
$ truncate -s <size> <file_path>
```

To build and run test source code,
```sh
$ cd read/
$ make
$ ./read.out <direct | buffered> <file_path> <offset> <length>
```

To iterate for performance evaluation,
```sh
$ sudo ./exec.sh <direct | buffered> <file_path> <offset> <length>
```

## Secure read/write system calls in Linux 6.1.9

To configure linux compile,

```sh
$ cd linux-6.1.9/
$ make x86_64_defconfig
$ make menuconfig
```

Inside menuconfig, enable Device Drivers -> NVME Support -> NVM Express Block Device.

To compile,

```sh
$ make -j 16
$ make modules
$ sudo make modules_install
$ sudo make install
```

## Simple application to use secure read/write system calls

To compile,

```sh
$ cd secure_rw_apps/
$ make write
$ make read
```

To run,

```sh
$ ./swrite.out <file_path>
$ ./sread.out <file_path>
```

## I/O Monitoring Kernel Module

To compile,

```sh
$ cd io-monitor/
$ make
```

To load module,
```sh
$ make load
```

To view monitoring data,
```sh
$ dmesg
```

To unload module,
```sh
$ make unload
```
