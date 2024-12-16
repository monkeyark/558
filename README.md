# FreeRTOS EDF Scheduling

FreeRTOS with EDF algorithm scheduling

Each real time kernel port consists of three files that contain the core kernel
components and are common to every port, and one or more files that are 
specific to a particular microcontroller and or compiler.

+ The FreeRTOS_core directory contains the three files that are common to 
every port - list.c, queue.c and tasks.c.  The kernel is contained within these 
three files.  croutine.c implements the optional co-routine functionality - which
is normally only used on very memory limited systems.

+ The FreeRTOS_core/Portable directory contains the files that are specific to 
a particular microcontroller and or compiler.

+ The FreeRTOS_core/include directory contains the real time kernel header 
files.

See the readme file in the FreeRTOS_core/Portable directory for more 
information.

+ The FreeRTOS is customized using a configuration file called FreeRTOSConfig.h. 


# Requirements
The 32-bit glibc package contains the object files necessary for developing programs that use the standard C libraries.
  - ```$ sudo dnf install glibc-devel.i686```
  
# Compile And Run
  - ```$ make```
  - ```$ ./edf_sim```

# Run without EDF
  - ```$ make clean```
  - set ```configUSE_EDF_SCHEDULER = 0``` in FreeRTOSConfig.h
  - ```$ make```
  - ```$ ./edf_sim```

