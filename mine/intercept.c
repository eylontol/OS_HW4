
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/utsname.h>
#include <linux/syscall.h>
#include <asm-i386/unistd.h>
// TODO: add more #include statements as necessary

MODULE_LICENSE("GPL");


void** sys_call_table = NULL;

int scan_range=0;
MODULE_PARM(scan_range, "i");

// TODO: import original syscall and write new syscall


void find_sys_call_table(int scan_range)
{
    int* i = &system_utsname;
    for (;*((unsigned int *)i) != (unsigned int)sys_read && scan_range > 0; i++, scan_range--);
    if (*((unsigned int *)i) == (unsigned int)sys_read)
    {
        i-=__NR_read;
        sys_call_table = i;
    }
    else
    {
        printk("DIDNT FOUND\n");
    }
}

int init_module(void) {
    printk("%d\n",scan_range);
    find_sys_call_table(scan_range);
    return 0;
}

void cleanup_module(void) {

   // TODO: complete the function
    printk("done\n");
}
