# Embedded_Linux
BeagleBoneBlack


#1) Device Files
- The device file allows transparent communication between userspace
applications and hardware.

- Device File not normal "file", but look like files from the program's 
point of view: it can be read, written by using system call.

- Device File provide a convenient way to access system resource without 
requiring the application programmer to know how the underlying device 
works.

- All device files are stored in /dev directory.


#2) Creating Device File

- We can create a device file in to ways:
+ Manually
+ Automatically


#2.1) Manually Creating Device File

- We can create the device file manually by using mknod.

-Systax:

mknod -m <perm> <name> <device_type> <major> <minor>

where:

- name : device file name.
- device_type : Put c (character device) or b (block device)
- major
- minor
- perm : optional argument that sets the permission bits of the new device 
file to permissions.

Example:
sudo mknod -m 666 /dev/etx_device c 246 0

- Advantages : 
+ Anyone can create the device file using this method.
+ You can create the device file even before loading the driver.


#2.2) Automatically Creating Device File.

- We just follow the bellow steps:
+ Include the header file linux/device.h and linux/kdev_t.h
+ Create the struct Class
+ Create Device with the class which is created by the above step.


#2.2.1) Create the class

- Creat the struct class for device driver:

struct class *class_create(struct module *ower, const char *name)

- Destroy device driver when module exit :

void class_destroy(struct class *cls);

#2.2.2) Create Device

- Create device file fo device driver:

struct device *device_create(struct *class, struct device *parent, dev_t dev
, void * drvdata, const char *fmt, ...)


- Destroy device file when module exit :

void device_destroy (struct class * class, dev_t devt);
