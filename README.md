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
 
