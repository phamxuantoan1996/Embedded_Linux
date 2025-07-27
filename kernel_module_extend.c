/******************************************************************************
*  \file       driver.c
*
*  \details    Creating Kernel Module with IOCTL
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

#include <linux/slab.h> /*kmalloc and kfree*/
#include <linux/uaccess.h> /*copy_to/from_user*/

#include <linux/err.h>

#include <linux/ioctl.h> /*for ioctls*/

#define mem_size 1024

typedef struct
{
	uint8_t value;
	int8_t buffer[20];
} ioctl_args;
ioctl_args kernel_ioctl_args;

/*create device file*/ 
dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;

/*pointer buffer data*/
uint8_t *kernel_buffer;

/*create ioctl command in the driver*/
#define MAGIC_NUMBER 'k'
#define WRITE_STRUCT _IOW(MAGIC_NUMBER,0,ioctl_args*)
#define READ_STRUCT _IOR(MAGIC_NUMBER,1,ioctl_args*)

/*function prototypes of file operations*/
static int      etx_open(struct inode *inode, struct file *file);
static int      etx_release(struct inode *inode, struct file *file);
static ssize_t  etx_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  etx_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static long etx_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

/*create file operation structure*/
static struct file_operations fops = 
{
	.owner		= THIS_MODULE,
	.read		= etx_read,
	.write		= etx_write,
	.open		= etx_open,
	.release	= etx_release,
	.unlocked_ioctl = etx_ioctl,
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
        size_t ret;
	size_t byte_to_copy;
	//
    	size_t len_data_kernel_buffer = strlen(kernel_buffer) + 1;
    	if (*off >= len_data_kernel_buffer)
    	{
        	return 0; //end of file
    	}
    	byte_to_copy = len;
    	if(byte_to_copy > len_data_kernel_buffer)
    	{
        	byte_to_copy = len_data_kernel_buffer;
    	}

    	//Copy the data from the kernel space to the user-space
    	ret = copy_to_user(buf, kernel_buffer + *off, byte_to_copy);

    	if(ret == 0)
    	{
        	*off += byte_to_copy;
        	// pr_info("Number of byte is read : %ld.\n",byte_to_copy);
        	return byte_to_copy;
    	}
    	else
    	{
        	// pr_info("Fail to copy %ld bytes to user space.\n",ret);
        	return -EFAULT;
    	}
}
/*
** This function will be called when we write the Device file
*/
static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        //Copy the data to kernel space from the user-space
        uint32_t numOfByte = 0;
        numOfByte = copy_from_user(kernel_buffer, buf, len);
        if(numOfByte == 0)
        {
            pr_info("Data : %s",(char*)kernel_buffer);
            return len;
        }
        else
        {
            return -EFAULT;
        }
}

/*
**This is function will be called when we send command by using systemcall ioctl()
*/
static long etx_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    	switch (cmd)
    	{
        	case WRITE_STRUCT:
        	{
            		if (copy_from_user(&kernel_ioctl_args, (ioctl_args __user *)arg, sizeof(kernel_ioctl_args)))/*copy to kernel*/
            		{
                		// Handle error
                		return -EFAULT;
            		}
            		pr_info("Data is setted!\n");
            		pr_info("Value : %d\n",kernel_ioctl_args.value);
            		pr_info("Buffer : %s\n",kernel_ioctl_args.buffer);
            		pr_info("----------------------------------------\n");
            		break;
        	}
        	case READ_STRUCT:
        	{
            		if (copy_to_user((ioctl_args __user *)arg,&kernel_ioctl_args, sizeof(kernel_ioctl_args)))/*copy to user*/
            		{
                		return -EFAULT; // Indicate a bad address
            		}
            		pr_info("Data is read.\n");
            		break;
        	}
        	default:
            		return -ENOTTY; // Inappropriate ioctl for device
    	}
    	return 0;
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

	/*create buffer data kernel*/
	if((kernel_buffer = kmalloc(mem_size,GFP_KERNEL)) == 0)
	{
		pr_err("Cannot allocate memory in kernel.\n");
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
	kfree(kernel_buffer);
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
MODULE_DESCRIPTION("Kernel Module with IOCTL");
MODULE_VERSION("1.2");


