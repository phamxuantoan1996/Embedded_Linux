/******************************************************************************
*  \file       driver.c
*
*  \details    Creating Kernel Module with File Operations
*
*  \author     PhamToan
*
*******************************************************************************/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

#include <linux/err.h>

/*create device file*/ 
dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;

/*function prototypes of file operations*/
static int      etx_open(struct inode *inode, struct file *file);
static int      etx_release(struct inode *inode, struct file *file);
static ssize_t  etx_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  etx_write(struct file *filp, const char *buf, size_t len, loff_t * off);

/*create file operation structure*/
static struct file_operations fops = 
{
	.owner		= THIS_MODULE,
	.read		= etx_read,
	.write		= etx_write,
	.open		= etx_open,
	.release	= etx_release,
};

/*
** This function will be called when we open the Device file
*/
static int etx_open(struct inode *inode, struct file *file)
{
        pr_info("Driver Open Function Called...!!!\n");
        return 0;
}
/*
** This function will be called when we close the Device file
*/
static int etx_release(struct inode *inode, struct file *file)
{
        pr_info("Driver Release Function Called...!!!\n");
        return 0;
}
/*
** This function will be called when we read the Device file
*/
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        pr_info("Driver Read Function Called...!!!\n");
        return 0;
}
/*
** This function will be called when we write the Device file
*/
static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        pr_info("Driver Write Function Called...!!!\n");
        return len;
}


/*
** Module init function
*/
static int __init kernel_module_extend_init(void)
{
        /*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 1, "etx_dev")) <0){
                pr_err("Cannot allocate major number for device.\n");
                return -1;
        }
        pr_info("Major = %d Minor = %d.\n",MAJOR(dev), MINOR(dev));

	/*Creating cdev structure*/
	cdev_init(&etx_cdev,&fops);

	/*adding character device to system*/
	if((cdev_add(&etx_cdev,dev,1)) < 0)
	{
		pr_err("Cannot add the device to the system.\n");
	}

        /*Creating struct class*/
        dev_class = class_create(THIS_MODULE,"etx_class");
        if(IS_ERR(dev_class)){
            pr_err("Cannot create the struct class for device.\n");
            goto r_class;
        }
 
        /*Creating device*/
        if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"etx_device"))){
            pr_err("Cannot create the Device.\n");
            goto r_device;
        }
        pr_info("Kernel Module Inserted Successfully...\n");
        return 0;
 
r_device:
        class_destroy(dev_class);

r_class:
        unregister_chrdev_region(dev,1);
        return -1;
}
 
/*
** Module exit function
*/
static void __exit kernel_module_extend_exit(void)
{
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
	cdev_del(&etx_cdev);
        unregister_chrdev_region(dev, 1);
        pr_info("Kernel Module Removed Successfully...\n");
}
 
module_init(kernel_module_extend_init);
module_exit(kernel_module_extend_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pham Xuan Toan");
MODULE_DESCRIPTION("Kernel Module with File Operations");
MODULE_VERSION("1.2");


