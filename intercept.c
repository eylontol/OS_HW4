
#include <stdlib.h>
#include <stdio.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/utsname.h>
#include <unistd.h>

MODULE_LICENSE("GPL");

extern asmlinkage ssize_t sys_read(unsigned int fd, char * buf, size_t count);

void** sys_call_table = NULL;

// TODO: import original syscall and write new syscall

void find_sys_call_table(int scan_range) {
    int *curr = (int *)(&system_utsname), i;
    for (i = 0; i < scan_range; ++i)
        if (*(curr++) == (int)sys_read) {
            sys_call_table = ((void **)(curr - 3 /* __NR_read */));
            break;
        }
}

int init_module(void) {
    printk("init_module\n");
    find_sys_call_table(136);
    return 0;
}

void cleanup_module(void) {
    printk("cleanup_module\n");
}
