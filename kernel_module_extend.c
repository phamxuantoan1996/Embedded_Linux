/******************************************************************************
*  \file       kernel_module_extend.c
*
*  \details    GPIO Interrupt
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

#include <linux/gpio.h>
#include <linux/interrupt.h>

#include <linux/io.h> /*remap, readl, writel function*/
#include <linux/uaccess.h>
#include <linux/slab.h>

/*define address register of GPIO0*/
#define GPIO0_BASE_ADDR     0x44E07000
#define GPIO_OE_OFFSET      0x134
#define GPIO_DATAOUT_OFFSET 0x13C
#define GPIO_SETDATAOUT     0x194
#define GPIO_CLEARDATAOUT   0x190
#define GPIO_DATAIN	    0x138

#define GPIO0_SIZE          0x1000
#define GPIO_PIN_OUT            30
#define GPIO_PIN_IN 		31

#define GPIO_RISINGDETECT   0x148
#define GPIO_FALLINGDETECT  0x14C
#define GPIO_IRQSTATUS_SET_0 0x34
#define GPIO_IRQSTATUS_0     0x2C

static void __iomem *gpio0_base;
uint8_t led_state = 0;
uint8_t *kernel_buffer;

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
*IRQ GPIO Handler
*/
static irqreturn_t gpio_irq_handler(int irq, void *dev_id)
{
	// Clear interrupt
	writel(1 << GPIO_PIN_IN, gpio0_base + GPIO_IRQSTATUS_0);
	printk(KERN_INFO "GPIO interrupt occurred\n");
	if(led_state == 0)
	{
		led_state = 1;
		writel(1 << GPIO_PIN_OUT, gpio0_base + GPIO_SETDATAOUT);
	}
	else
	{
		writel(1 << GPIO_PIN_OUT, gpio0_base + GPIO_CLEARDATAOUT);
		led_state = 0;
	}
    	return IRQ_HANDLED;
}


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
	uint32_t numOfByte = 0;
        pr_info("Driver Write Function Called...!!!\n");
	if((kernel_buffer = kmalloc(1024,GFP_KERNEL)) == 0)
	{
		pr_err("Cannot allocate memory in kernel.\n");
		return -EFAULT;
	}
	numOfByte = copy_from_user(kernel_buffer,buf,len);
	if(numOfByte == 0)
	{
		int ret,led_val;
		kernel_buffer[len - 1] = 0;
		ret = kstrtoint(kernel_buffer,10,&led_val);
		if(ret == 0)
		{
			pr_info("led value : %d\n",led_val);
			if(led_val == 0)
			{
				writel(1 << GPIO_PIN_OUT, gpio0_base + GPIO_CLEARDATAOUT);
				led_state = 0;
			}
			else
			{
				if(led_val == 1)
				{
					led_state = 1;
					writel(1 << GPIO_PIN_OUT, gpio0_base + GPIO_SETDATAOUT);
				}
			}
		}
	}
	else
	{
		kfree(kernel_buffer);
		return -EFAULT;
	}
	kfree(kernel_buffer);
        return len;
}


/*
** Module init function
*/
static int __init kernel_module_extend_init(void)
{
	uint32_t reg_config;
	int irq_number;
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

	/*Config GPIO_30 as a output*/
	gpio0_base = ioremap(GPIO0_BASE_ADDR, GPIO0_SIZE);
	if(!gpio0_base)
	{
		pr_err("Failed to map GPIO0\n");
		return -ENOMEM;
	}
	reg_config = readl(gpio0_base + GPIO_OE_OFFSET);
	reg_config &= ~(1 << GPIO_PIN_OUT);
	writel(reg_config,gpio0_base + GPIO_OE_OFFSET);
	/*Config GPIO_31 as a interrupt*/
	// Enable rising edge
	writel(readl(gpio0_base + GPIO_RISINGDETECT) | (1 << GPIO_PIN_IN),gpio0_base + GPIO_RISINGDETECT);
	writel(readl(gpio0_base + GPIO_IRQSTATUS_SET_0) | (1 << GPIO_PIN_IN),gpio0_base + GPIO_IRQSTATUS_SET_0);
	irq_number = gpio_to_irq(31);  // 49 = GPIO0_31

	if(request_irq(irq_number, gpio_irq_handler,IRQF_TRIGGER_RISING, "gpio_irq_31", NULL))
	{
		pr_err("my_device: cannot register IRQ\n");
		goto irq;
	}

        pr_info("Kernel Module Inserted Successfully...\n");
        return 0;

irq:
	free_irq(irq_number,NULL);

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
	int irq_number = gpio_to_irq(31);
	free_irq(irq_number, NULL);
	iounmap(gpio0_base);

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
MODULE_DESCRIPTION("Kernel Module GPIO Interrupt");
MODULE_VERSION("1.2");


