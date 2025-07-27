REAL OPERATIONS

1) kmalloc() function

kmalloc function is used to allocate the memory in kernel space. This is like a 
malloc() function in userspace.

The allocated region still holds its previous content. The allocated region is 
also contiguous in physical memory.

Include : linux/slab.h

Syntax :
	void *kmalloc(size_t size, gfp_t flags);

where :

size - how many bytes of memory are required.
flags - the type of memory to allocate.

The flags argument may be one of:
+ GFP_USER
+ GFP_KERNEL
+ ..........


2) kfree() function

This is like a free() function in the userspace. This is used to free the 
previously allocated memory.

Syntax :
	void kfree(const void *objp);
where :

objp - pointer returned by kmalloc


3) copy_from_user()

This function is used to Copy a block of data frome user space (Copy data from 
user space to kernel space).

unsigned long copy_from_user(void *to, const void __user *from, unsigned long n);

where :
to - Destination address, in the kernel space.
from - The source address in the user space.
n - Number of bytes to copy.

Returns number of bytes that could not be copied. On success, this will be zero.

4) copy_to_user()

This function is used to Copy a block of data into user space (Copy data from 
kernel space to user space).

unsigned long copy_to_user(const void __user *to, const void *from, unsigned long n);

where :
to - Destination address, in the user space.
from - The source address in the the kernel space.
n - Number of bytes to copy

Return number of bytes that could not be copied. On success, this will be zero.


