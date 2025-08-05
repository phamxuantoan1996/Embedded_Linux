#Interrupt Linux

1) Polling vs Interrupt

In polling the CPU keeps on checking all the hardwares of the availablity of 
any request.

In interrupt the CPU take care of the hardware only when the hardware requests 
for some service.

An interrupt is produced by electronic signal from hardware devices and 
directed into input pins on an interrupt controller.

These are process that will be done by the kernel:
+ Upon receiving a interrupt, the interrupt controller send a signal to 
processor. 

+ The processor detects this signal and interrupts its current execution to 
handle the interrupt.

+ The process can the notify the OS that an interrupt has occurred, and the OS 
can handle the interrupt appropriately.

Different devices are associated with different interrupts using a 
unique value associated with each interrupt. This enables the OS to 
differentiate between interrupts and to know which hardware device 
caused such an interrupt. In turn, the OS can service each interrupt 
with its corresponding.

Interrupt handling is amongst the most sensitive tasks performed by the 
kernel and it must satisfy the following:
+ Hardware devices generate interrupts asynchronously. That mean 
interrupts can come anytime.

+ Because interrupts can come anytime, the kernel might be handling one 
of them while another one occurs.

+ Some critial regions exist inside the kernel code where interrupts 
must be disable. Such critical regions must be limited as much as 
possible.

2) Interrupts and Exceptions

3) Interrupts

Maskable : All Interrupt Requests (IRQs) issued by I/O devices give rise 
to maskable interrupts. A maskable interrupt can be in two states : 
masked and unmasked; a masked interrupt is ignored by the control unit 
as long as remains masked.

Non-maskable : Only a few critical events (such as hardware failures) 
give rise to non-maskable interrupts. Non-maskable interrupts are always 
recognized by the CPU.

4) Exceptions

5) Interrupt Handler

An interrupt handler or interrupt service routine (ISR) is the function 
that the kernel runs in response to a specific interrupt:
+ Each device that generates interrupts has an associated interrupt 
handler.

+ The interrupt handler for a device is part of the device's driver.

What differentiates interrupt handlers from other kernel functions is 
that kernel invokes them in response to interrupts and that they run 
in a special context called interrupt context. 

Because an interrupt can occur at any time, an interrupt handler can 
be executed at any time. It is imperative that the handler runs quickly, 
to resume the execution of the interrupt code as soon as possible. It is 
important that :

+ To the hardware : the OS services the interrupt without delay. 

+ To the rest of the system : the interrupt handler executes in as short 
a period as possible.

An interrupt handler's job is to acknowledge the interrupt's receipt 
to the hardware. However, interrupt handlers can often have a large 
amount of work to perform.


6) Process Context and Interrupt Context
 

Kernel code that services system calls issued by user applications runs on behalf
of the corresponding application processes and is said to excute in the process 
context. 

Interrupt handler, on the other hand, run asynchronously in the interrupt context. 

Process context are not tied to any interrupt context and vice versa.

Kernel code running in process context is preemptible. An interrupt context, 
however, always runs to completion and is not preemptible. Because of this, there are 
retrictions on what can be done from an interrupt context. Code executing from 
interrupt context cannot do the following:

+ Go to sleep or relinquish the processor.
+ Acquire a mutex
+ Perform time-consuming task
+ Access user space virtual memory

If we tak more time to process in a Interrupt Handler, this will happen:
+ While the highest priority is running, it doesn't let other interrupts to run.
+ Interrupts with the same type will be missed.

To eliminate that problem, the processing of interrupts is split into two parts or
halves:
+ Top halves.
+ Bot halves.

7) Top halves and Botton halves
The interrupt handler is the top half. The top half will run immediately upon 
receipt of the interrupt and perform only the work that is time-critical, such 
as acknowledging receipt of the interrupt or resetting the hardware.

The bottom half is used to process data, letting the top half to deal with new 
incoming interrupts. Interrupt are enable when a bottom half runs. 

IF the interrupt handler function could process and acknowledge interrupts within 
a few microseconds consistently, the absolutely there is no need for top half/bottom 
half delegation.

8) Functions Related to Interrupt

request_irq() : Register an IRQ.

free_irq() : Release an IRQ register.

enable_irq() : Re-enable interrupt disabled by disable_irq or disable_irq_nosync

disable_irq() : Disable an IRQ from issuing a interrupt.

disable_irq_nosync() : Disable an IRQ from issuing an interrupt, but wait ultil 
there is an interrupt handler being executed.

in_irq() : returns true when interrupt handler.

in_interrupt() : return true when in interrupt handler or bottom half.
