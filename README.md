1) Introduce

One the root, there is a folder titled "proc". This folder is a mount point 
for the procfs (Process Filesystem) which is a filesystem in a memory.

Many processes store information about themselves on this virtual filesystem. 
ProcFS also stores other system information.

Procfs can act as a bridge connecting the user space and the kernel space. 
User space can use proc files to read the information exported by the 
kernel. Every entry in the proc file system provides some infomation from 
kernel.

The proc file system is also very useful when we want to debug a kernel 
module. While debugging we might want to know the values of various 
variable in the module or maybe the data that the module is handling. 


The proc entry can also be used to pass data to the kernel by writing into 
the kernel, so there can be kinds of proc entries :
- An entry that only reads only data from the kernel space.
- An entry that reads as well as writes data into and from kernel space.

2) Creating procfs directory

You can create the directory under /proc using below API:

struct proc_dir_entry *proc_mkdir(const char *name, struct proc_dir_entry 
*parent)

where :

name : The name of the directory that will be created under /proc
parent : 

3) Creating procfs entry

include header file proc_fs.h 

struct proc_dir_entry *proc_create ( const char *name, umode_t mode
, struct proc_dir_entry *parent, const struct file_operations *proc_fops )

3) Procfs file

We need to create file_operations structure proc_fops in which we can map 
the read and write functions for the proc entry.

static struct file_operations proc_fops = {
    .open = open_proc,
    .read = read_proc,
    .write = write_proc,
    .release = release_proc
};
