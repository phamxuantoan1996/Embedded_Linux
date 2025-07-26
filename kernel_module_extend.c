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
#include<linux/moduleparam.h> //include for passing argument

int valueETX,arr_valueETX[5]; /*create variable and array for passing argument*/
char *nameETX;

/*use  macro passing argument*/
module_param(valueETX,int,S_IRUSR|S_IWUSR);
module_param(nameETX,charp,S_IRUSR|S_IWUSR);
module_param_array(arr_valueETX,int,NULL,S_IRUSR|S_IWUSR);
 
/*
** Module Init function
*/
static int __init kernel_module_extend_init(void)
{
	int i = 0;
	printk(KERN_INFO "Kernel Module Inserted Successfully...\n");
	printk(KERN_INFO "valueETX = %d\n",valueETX);
        printk(KERN_INFO "nameETX = %s\n",nameETX);
        printk(KERN_INFO "Array:\n");
        for(i = 0; i < 5; i++)
        {
                printk(KERN_INFO "pt[%d] = %d\n",i,arr_valueETX[i]);
        }
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
MODULE_DESCRIPTION("Passing argument");
MODULE_VERSION("1:1.0");

/*
Note : To pass argument when load module by using command below:
-> sudo insmod kernel_module_extend.ko valueETX=14 nameETX='"pham xuan toan"' arr_valueETX=1,2,3,4,5

*/
