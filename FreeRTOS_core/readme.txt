Each real time kernel port consists of three files that contain the core kernel
components and are common to every port, and one or more files that are 
specific to a particular microcontroller and or compiler.

+ The FreeRTOS/FreeRTOS_core directory contains the three files that are common to 
every port - list.c, queue.c and tasks.c.  The kernel is contained within these 
three files.  croutine.c implements the optional co-routine functionality - which
is normally only used on very memory limited systems.

+ The FreeRTOS/FreeRTOS_core/Portable directory contains the files that are specific to 
a particular microcontroller and or compiler.

+ The FreeRTOS/FreeRTOS_core/include directory contains the real time kernel header 
files.

See the readme file in the FreeRTOS/FreeRTOS_core/Portable directory for more 
information.