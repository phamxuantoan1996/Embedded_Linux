/***************************************************************************//**
*  \file       driver.c
*
*  \details    Simple kernel module
*
*  \author     phamtoan
*
* *******************************************************************************/
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
 
/*
** Module Init function
*/
static int __init kernel_module_extend_init(void)
{
    printk(KERN_INFO "Kernel Module Inserted Successfully...\n");
    return 0;
}

/*
** Module Exit function
*/
static void __exit kernel_module_extend_exit(void)
{
    printk(KERN_INFO "Kernel Module Removed Successfully...\n");
}
 
module_init(kernel_module_extend_init);
module_exit(kernel_module_extend_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("PhamToan");
MODULE_DESCRIPTION("A simple kernel module");
MODULE_VERSION("1:1.0");
