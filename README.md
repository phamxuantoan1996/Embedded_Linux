IOCTL

There are many ways to communicate between the User space and Kernel space, 
they are:

+ IOCTL
+ Procfs
+ Sysfs
+ Configfs
+ Debugfs
+ Sysctl
+ UDB Sockets
+ Netlink Sockets

1) IOCTL

IOCTL is referred to as Input and Output Control, which is used to talk to 
device driver. This system call is available in most driver categories. 

The major use of this is in case of handling some specific operations of a 
device for which the kernel does not have a system call by default.

2) Step Involved in IOCTL


+ Create IOCTL command in the driver
+ Write the IOCTL function in the driver
+ Create IOCTL command in a Userspace application
+ Use the IOCTL system call in a Userspace

2.1) Create IOCTL command in the driver

- Define the IOCTL command:

#define "ioctl_name" __IOX("magic_number","command_number","argument_type");

where:

IOX can be : IO,IOW,IOR,IOWR

magic_number : is a unique number or character that will differntiate our set 
of ioctl calls from the other ioctl calls. Some times the major number for 
the device is used here.

command_number : is the number that is assigned to the ioctl. This is used 
to differentiate the commands from one another.

- Include header file linux/ioctl


2.2) Write IOCTL Function in the Driver

- We need to add the ioctl function to our driver. 

Declare prototype of ioctl function:

int  ioctl(struct inode *inode,struct file *file,unsigned int cmd,unsigned long arg)

- Define ioctl function


- Add ioctl to driver
static struct file_operations fops = 
{
	.owner		= THIS_MODULE
	.............................
	.............................
	.unlocked_ioctl	= etx_ioctl,
}

2.3) Create IOCTL Command in a User space Applicaton

Just define the ioctl command like how we define it in the driver.

2.4) Use IOCTL System Call in User space.

- Include header file sys/ioctl.h

- call ioctl system call

Syntax:
	long ioctl("file descriptor","ioctl_command","argument");

Example
