# FreeRTOS EDF Scheduling

FreeRTOS with EDF algorithm scheduling

# Requirements
  - ```$ sudo dnf install glibc-devel.i686```
  
# Compile And Run
  - ```$ make```
  - ```$ ./FreeRTOS-Sim```

# Run without EDF
  - ```$ make clean```
  - set ```configUSE_EDF_SCHEDULER = 0``` in FreeRTOSConfig.h
  - ```$ make```
  - ```$ ./FreeRTOS-Sim```
  
 # Results
  ![Result](results.png)
