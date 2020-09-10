# Operating-System
##Lab1 Linker 
Implemented a two-pass linker. In general, a linker takes individually compiled code/object modules and creates a single executable by resolving external symbol references (e.g. variables and functions) and module relative addressing by assigning global addresses after placing the modules’ object code at global addresses.

##Lab2 Scheduler / Dispatcher
Implemented scheduling algorithms on a set of processess/threads executing on a system. The system is to be implemented using Discrete Event Simulation (DES).

The Scheduling Algorithms to be simulated are:
* FCFS
* LCFS
* SRTF
* RR (Round Robin)
* PRIO (Priority Scheduler)
* PREempive PRIO (PREPRIO)

##Lab3 Virtual Memory Management
Implemented/simulated the operation of an Operating System’s Virtual Memory Manager that maps the virtual address spaces of multiple processes onto physical frames using page table translation.

##Lab4 IO Scheduling
Implemented/simulated the scheduling of I/O operations. Applications submit their IO requests to the IO subsystem (potentially via the filesystem and pagecache), where they are maintained in an IO-queue until the disk device is ready for servicing another request. The IO-scheduler then selects a request from the IO-queue and submits it to the disk device.

The Scheduling Algorithms to be implemented are:
* FCFS
* SSTF
* LOOK
* CLOOK
* FLOOK