#Waitqueue in Linux

1) Introduction

When you write a Linux Driver Module or Kernel Program, some processes 
should wait or sleep for some event. There are several ways of handling 
sleeping and waking up in Linux, each suited to differnts need. 

Waitqueue is also one of the method to handle that case.

Whenever a process must wait for an event (such as the arrival of data
 or termination of a process), it should go to sleep. Sleeping causes 
the process to suspend excution, freeing the processor for uses.

After some time, the process will be woken up and will continue with 
its job when the event which are waiting for has arrived.

Waitqueue is the list of processes waiting for an event.

A wait queue is used to wait for someone to wake up when a certain 
condition is true.

Waitqueue must be used carefully to ensure there is no race condition.

There are 3 important step in Waitqueue:

+ Initializing Waitqueue

+ Queuing (Put the task to sleep util the event comes)

+ Waking up Queued Task


2) Initializing Waitqueue

Include header file : #include <linux/wait.h>

There are two ways to initialize the waitqueue:
+ Static method.

DECLARE_WAIT_QUEUE_HEAD(wq);

+ Dynamic method.

wait_queue_head_t wq;
init_waitqueue_head (&wq);

3) Queuing

Once the wait queue is declared are initialized, a process may use it 
to go to sleep. There are several macros are available for different 
use.


+ wait_event(wq,condition)

The process is put to sleep (TASK_UNINTERRUPTIBLE) until the 
condition evaluates to true. The condition is checked each time 
waitqueue wq is woken up.

+ wait_event_timeout(wq,condition,timeout)

The process is put to sleep (TASK_UNINTERRUPTIBLE) until the condition 
evaluates to true or timeout elapses. The condition is checked each 
time the waitqueue is woken up.

It returns 0 if the conditions evaluated to false after the timeout 
elapses, 1 if the condition evaluated to true after the timeout elapses
, or the remaining jiffies (at least 1) if condition evaluated to true 
before the timeout elapsed.


+ wait_event_cmd(wq,condition,cmd1,cmd2)


+ wait_event_interruptible(wq,condition)

The process is put to sleep (TASK_INTERRUPTIBLE) until the condition 
evaluated to true or a signal is received. 

The condition is checked each time the waitqueue wq is woken up.

The function will return -ERESTARTSYS if it was interrupted by a 
signal and 0 if condition evaluate to true. 

+ wait_event_interruptible_timeout(wq,condition,timeout)

The process is put to sleep (TASK_INTERRUPTIBLE) until the condition 
evaluated to true or a signal is received or timeout elaped. The 
condition is checked each time woken up.

It returns, 0 if the condition evaluated to false after timeout 
elapsed, 1 if the condition evaluated to true after timeout elapsed, 
the remaining jiffies (at least 1) the condition evaluated to true 
before the timeout elapsed, or -ERESTARTSYS if it was interrupt by a 
signal.  

+ wait_event_killable(wq,condition)

Sleep until a condition gets true.

The process is put to sleep (TASK_KILLABLE) until the condition 
evaluates to true or a signal is received. The condition is checked 
each time the waitqueue wq woken up.

The function will return -ERESTARTSYS if it was interrupted by a 
signal and 0 if condition evaluated to true.

4) Waking up Queued Task.

+ wake_up(&wp)

wakes up only one process from the wait queue which is in 
non-interruptible sleep. 

+ wake_up_all(&wp);

wakes up all the processes on the wait queue.

+ wake_up_interruptible(&wp)

wakes up only one process from the wait queue that is in 
interruptible sleep

4) wake_up_sync(&wp) and wake_up_interruptible_sync(&wp)


