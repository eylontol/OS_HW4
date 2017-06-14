
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/utsname.h>
#include <linux/syscall.h>
#include <asm-i386/unistd.h>

MODULE_LICENSE("GPL");


void** sys_call_table = NULL;

char *filepath = "sys";
//MODULE_PARM(filepath,”s”);

static int original_sys_unlink = (int)sys_unlink;

asmlinkage long our_sys_unlink(const char *pathname) {
    if (!strcmp(pathname, filepath)) return -EACCES;
    return sys_unlink(pathname);
}

<<<<<<< HEAD

void find_sys_call_table(int scan_range)
{
    void* i = &system_utsname;
    for (;*((unsigned long *)i) != (unsigned long)sys_read && scan_range > 0; i++, scan_range--);
    if (*((unsigned long *)i) == (unsigned long)sys_read)
    {
        i-=__NR_read;
        sys_call_table = i;
    }
=======
void find_sys_call_table(int scan_range) {
    int *curr = (int *)(&system_utsname), i;
    for (i = 0; (i < scan_range) && (!sys_call_table); ++i)
        if (*(curr++) == (int)sys_read)
            sys_call_table = ((void **)(curr - __NR_read - 1));
>>>>>>> origin/master
}

int init_module(void) {
    
    // Finding the sys_call_table address like a p.i.m.p
    find_sys_call_table(136);
<<<<<<< HEAD
    if (sys_call_table) printk("sys_call_table address = 0x%08x\n", (int)sys_call_table);
    else printk("Couldn't find syscall_table address\n");

=======
    
    // Hacking the sys_call_table like a motherfucker
    sys_call_table[__NR_unlink] = our_sys_unlink;
    
>>>>>>> origin/master
    return 0;
}

void cleanup_module(void) {
<<<<<<< HEAD

   // TODO: complete the function
    printk("done\n");
=======
    // restore original address like a bitch
    sys_call_table[__NR_unlink] = (void *)original_sys_unlink;
>>>>>>> origin/master
}
