Register and use device node :

B1) Define struct compatible
#include <linux/of.h
static const struct of_device_id sac58r_wdt_dt_ids[] = {
	{.compatible = "fsl,sac58r-wdt"},
 	{.compatible = "fsl,s32v-wdt"},
  	{.compatible = "fsl,s32gen1-wdt"}
};

In the Linux kernel, the struct of_device_id is used to match a device driver with devices described in the Device Tree.
It's part of the Open Frimware/Device Tree matching mechaism
When the kernel boots, it parses the Device Tree (.dts/.dtb) to find hardware nodes. Each node may have properties like:
	compatible = "vendor,device-name";
 The driver declares an array of_device_id structures that list all the compatible string it supports.

 B2) Define platform drivers

 A platform_driver in the Linux kernel is a type of device driver specifically designed for platform devices - devices that are typically not discoverable via hardware enumeration (like PCI or USB) but are instead described via Device Tree, ACPI, or are registered directly in code.

 Matching happens via:
 + of_device_id (Device Tree compatible property)
 + platform_device_id (name-based matching without DT)

 Handle probe (initializtion) and remove (cleanup)

 Example:

 In Device Tree:
 my_device@0 {
    compatible = "myvendor,mydevice";
    reg = <0x00 0x1000>;
};


In Driver Code:
 #include <linux/platform_device.h>
 #include <linux/module.h>
 #include <linux/of.h>

 static int my_probe(struct platform_device *pdev)
{
    pr_info("my_driver: probe called for %s\n", pdev->name);
    return 0;
}
static int my_remove(struct platform_device *pdev)
{
    pr_info("my_driver: remove called\n");
    return 0;
}

static const struct of_device_id my_of_match[] = {
    { .compatible = "myvendor,mydevice" },
    { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, my_of_match);

static struct platform_driver my_driver = {
    .probe  = my_probe,
    .remove = my_remove,
    .driver = {
        .name           = "my_driver",
		.owner			= "this_module",
        .of_match_table = my_of_match,
    },
};
module_platform_driver(my_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("You");
MODULE_DESCRIPTION("Simple platform driver example");



