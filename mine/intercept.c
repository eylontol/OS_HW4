
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/utsname.h>
#include <linux/syscall.h>
#include <asm-i386/unistd.h>
// TODO: add more #include statements as necessary

MODULE_LICENSE("GPL");


void** sys_call_table = NULL;
void* old_address;

int scan_range=0;
char* filepath=NULL;
MODULE_PARM(scan_range, "i");
MODULE_PARM(filepath, "s");

// TODO: import original syscall and write new syscall


asmlinkage long our_sys_unlink(const char *pathname)
{
    if(strcmp(pathname, filepath)==0) return -EACCES;
    else return ((asmlinkage long(*)(const char *))old_address)(pathname);
}

void find_sys_call_table(int scan_range)
{
    int* i = &system_utsname;
    for (;*((unsigned int *)i) != (unsigned int)sys_read && scan_range > 0; i++, scan_range--);
    if (*((unsigned int *)i) == (unsigned int)sys_read)
    {
        printk("FOUND\n");
        i-=__NR_read;
        sys_call_table = i;
        old_address = sys_call_table[__NR_unlink];
        sys_call_table[__NR_unlink] = our_sys_unlink;
    }
    else
    {
        printk("DIDNT FOUND\n");
    }
}



int init_module(void) {
    find_sys_call_table(scan_range);
    return 0;
}

void cleanup_module(void) {

    sys_call_table[__NR_unlink] = old_address;
    printk("done\n");
}
