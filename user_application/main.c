#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
    u_int8_t value;
    int8_t buffer[20];
} ioctl_args;

#define WR_STRUCT _IOW('k',0,ioctl_args*)
#define RD_STRUCT _IOR('k',1,ioctl_args*)

int main(int argc,char *argv[])
{
    int fd;
    ioctl_args args_set = {20,"12345"};
    ioctl_args args_get;

    printf("Application start.\n");
    fd = open("/dev/etx_device",O_RDWR | O_APPEND);
    if (fd > 0)
    {
        /* code */
        int ret = ioctl(fd,WR_STRUCT,&args_set);
        
        if(ret == 0)
        {
            ret = ioctl(fd,RD_STRUCT,&args_get);
            close(fd);
            if (ret == 0)
            {
                /* code */
                printf("Value : %d\n",args_get.value);
                printf("Buffer : %s\n",args_get.buffer);
                return 0;
            }
            else
            {
                return 1;
            }
            return 0;
        }
        else
        {
            close(fd);
            return 1;
        }
        
    }
    else
    {
        return 1;
    }
    
}
