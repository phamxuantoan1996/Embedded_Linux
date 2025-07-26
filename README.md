# Embedded_Linux
BeagleBoneBlack

# Passing Argument
1) module_param().
- Include header : linux/moduleparam.h
- Syntax: module_param(name,type,perm);
where :
+ name : name of varible in kernel.
+ type : (bool,invbool), (int,long,short,uint,ulong,ushort),(charp)
+ perm : S_IWUSR, SIRUSR, S_IXUSR, IRGRP, IWGRP, IXGRP

- Example:
	module_param(valueETX,int,S_IWUSR | S_IRUSR);
This will creat a sysfs entry. (/sys/module/[name_of_kernel]/valueETX)

2) module_param_array()
- This macro is used to send the array as an argument to the Linux device 
driver.

- Syntax : 
	module_param_array(name,type,perm);
where:
+ name is the name of array.
+ type is the type of the array elements.
+ num is an integer variable (optional) otherwise NULL.
+ perm is the usual permissions value.

3) module_param_cb()
- This macro is used to register the callback. Whenever the argument (parameter) 
got changed, this callback function will be called. 

- If module want to get notification whenever the value got to change, module 
need to register our handler function to its file operation structure first.


	struct kernel_param_ops 
	{
 		int (*set)(const char *val, const struct kernel_param *kp);
 		int (*get)(char *buffer, const struct kernel_param *kp);
 		void (*free)(void *arg);
	};
