
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/utsname.h>
#include <linux/syscall.h>
#include <asm-i386/unistd.h>

MODULE_LICENSE("GPL");


void** sys_call_table = NULL;

char *filepath;
MODULE_PARM(filepath,"s");

asmlinkage long (*original_sys_unlink)(const char * pathname);

#define STRINGS_ARE_EQUAL(s1, s2)   (!strcmp((s1), (s2)))
asmlinkage long our_sys_unlink(const char *pathname) {
    if (STRINGS_ARE_EQUAL(pathname, filepath)) return -EACCES;
    return original_sys_unlink(pathname);
}

void find_sys_call_table(int scan_range) {
    int *curr = (int *)(&system_utsname), i;
    for (i = 0; (i < scan_range) && (!sys_call_table); ++i)
        if (*(curr++) == (int)sys_read)
            sys_call_table = ((void **)(curr - __NR_read - 1));
}

int init_module(void) {
    find_sys_call_table(136 /* lucky number */);
    original_sys_unlink = sys_call_table[__NR_unlink];
    sys_call_table[__NR_unlink] = our_sys_unlink;
    return 0;
}

void cleanup_module(void) {
    sys_call_table[__NR_unlink] = original_sys_unlink;
}
