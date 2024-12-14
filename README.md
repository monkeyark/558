# FreeRTOS EDF Scheduling

FreeRTOS with EDF algorithm scheduling

# Requirements
The glibc package contains the object files necessary for developing programs that use the standard C libraries.
  - ```$ sudo dnf install glibc-devel```
  
# Compile And Run
  - ```$ make```
  - ```$ ./edf_sim```

# Run without EDF
  - ```$ make clean```
  - set ```configUSE_EDF_SCHEDULER = 0``` in FreeRTOSConfig.h
  - ```$ make```
  - ```$ ./edf_sim```
  
 # Results
  ![Simulation Result](simulation.png)
