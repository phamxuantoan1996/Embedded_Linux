#include <linux/module.h>

#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>


#include <linux/of_address.h>
#include <linux/platform_device.h>
#include <linux/of.h>

#include <linux/io.h>

#include <linux/err.h>

#include <linux/delay.h>

#define GPIO_OE_OFFSET      0x134
#define GPIO_DATAOUT_OFFSET 0x13C
#define GPIO_SETDATAOUT     0x194
#define GPIO_CLEARDATAOUT   0x190
#define GPIO_DATAIN	    0x138

static void __iomem *gpio0_base;
u32 pin;
u32 base_address_gpio;
u32 size_gpio;


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
	return 0;
}

static int led_external_driver_probe(struct platform_device *pdev)
{
	uint32_t reg_config;
	struct resource res;

	if (of_address_to_resource(pdev->dev.of_node, 0, &res) == 0)
	{
		base_address_gpio =  res.start;
		size_gpio = res.end - res.start + 1;
		pr_info("Base address : 0x%x and Size : 0x%x\n",base_address_gpio,size_gpio);
	}

	of_property_read_u32(pdev->dev.of_node, "pin", &pin);
	pr_info( "Pin : 0x%x\n", pin);


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

	/*Config GPIO0_30 as a output*/
	gpio0_base = ioremap(base_address_gpio, size_gpio);
	if(!gpio0_base)
	{
		pr_err("Failed to map GPIO0\n");
		return -ENOMEM;
	}
	reg_config = readl(gpio0_base + GPIO_OE_OFFSET);
	reg_config &= ~(1 << pin);
	writel(reg_config,gpio0_base + GPIO_OE_OFFSET);

	writel(1 << pin, gpio0_base + GPIO_SETDATAOUT);
	mdelay(1000);
	writel(1 << pin, gpio0_base + GPIO_CLEARDATAOUT);
	mdelay(1000);
	writel(1 << pin, gpio0_base + GPIO_SETDATAOUT);
        mdelay(1000);
        writel(1 << pin, gpio0_base + GPIO_CLEARDATAOUT);
        mdelay(1000);

	return 0;

r_device:
        class_destroy(dev_class);

r_class:
        unregister_chrdev_region(dev,1);
        return -1;
}

static int led_external_driver_remove(struct platform_device *pdev)
{
    	pr_info("led driver is  removed\n");

	iounmap(gpio0_base);
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
	cdev_del(&etx_cdev);
        unregister_chrdev_region(dev, 1);
	return 0;
}

static const struct of_device_id my_of_match[] = {
    { .compatible = "user,led_external" },
    { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, my_of_match);

static struct platform_driver my_platform_driver = {
    .probe = led_external_driver_probe,
    .remove = led_external_driver_remove,
    .driver = {
        .name = "led_external_driver",
        .of_match_table = my_of_match,
    },
};

module_platform_driver(my_platform_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pham Toan");
MODULE_DESCRIPTION("Example Platform Driver");

