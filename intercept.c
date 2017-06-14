
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/utsname.h>
#include <linux/syscall.h>

MODULE_LICENSE("GPL");


void** sys_call_table = NULL;

// TODO: import original syscall and write new syscall


void find_sys_call_table(int scan_range)
{
    void* i = &system_utsname;
    for (;*((unsigned long *)i) != (unsigned long)sys_read && scan_range > 0; i++, scan_range--);
    if (*((unsigned long *)i) == (unsigned long)sys_read)
    {
        i-=__NR_read;
        sys_call_table = i;
    }
}

int init_module(void) {
    printk("init_module\n");
    find_sys_call_table(136);
    if (sys_call_table) printk("sys_call_table address = 0x%08x\n", (int)sys_call_table);
    else printk("Couldn't find syscall_table address\n");

    return 0;
}

void cleanup_module(void) {

   // TODO: complete the function
    printk("done\n");
}
