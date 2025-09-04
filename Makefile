######## Build options ########

verbose = 0

######## Build setup ########

# SRCROOT should always be the current directory
SRCROOT         = $(CURDIR)

# .o directory
ODIR            = obj

# Source VPATHS
VPATH           += $(SRCROOT)/FreeRTOS_core
VPATH	        += $(SRCROOT)/FreeRTOS_core/portable/MemMang
VPATH	        += $(SRCROOT)/FreeRTOS_core/portable/GCC/POSIX
VPATH           += $(SRCROOT)/Demo/Common
VPATH			+= $(SRCROOT)./FileIO
VPATH			+= $(SRCROOT).

# Split source files into two groups
MAIN_FILES = main.c
EDF_FILES = FreeRTOS_core/edf.c

# FreeRTOS Objects (only needed for main executable)
FREERTOS_FILES += croutine.c
FREERTOS_FILES += event_groups.c
FREERTOS_FILES += list.c
FREERTOS_FILES += queue.c
FREERTOS_FILES += tasks.c
FREERTOS_FILES += timers.c

# portable Objects (only needed for main executable)
FREERTOS_FILES += heap_3.c
FREERTOS_FILES += port.c

# Combine files for main executable
MAIN_C_FILES = $(MAIN_FILES) $(FREERTOS_FILES)

# Generate OBJS names for both executables
MAIN_OBJS = $(patsubst %.c,%.o,$(MAIN_C_FILES))
EDF_OBJS = $(patsubst %.c,%.o,$(EDF_FILES))

# Include Paths
INCLUDES        += -I$(SRCROOT)
INCLUDES        += -I$(SRCROOT)/FreeRTOS_core/include
INCLUDES        += -I$(SRCROOT)/FreeRTOS_core/portable/GCC/POSIX/
INCLUDES        += -I$(SRCROOT).
# INCLUDES        += -I/usr/include/x86_64-linux-gnu/  # Linux-specific path, disabled for macOS

######## C Flags ########

# Warnings
CWARNS += -W
CWARNS += -Wall
CWARNS += -Wextra
CWARNS += -Wformat
CWARNS += -Wmissing-braces
CWARNS += -Wno-cast-align
CWARNS += -Wparentheses
CWARNS += -Wshadow
CWARNS += -Wno-sign-compare
CWARNS += -Wswitch
CWARNS += -Wuninitialized
CWARNS += -Wunknown-pragmas
CWARNS += -Wunused-function
CWARNS += -Wunused-label
CWARNS += -Wunused-parameter
CWARNS += -Wunused-value
CWARNS += -Wunused-variable
CWARNS += -Wmissing-prototypes

#CWARNS += -Wno-unused-function

# CFLAGS += -m32  # Disabled for macOS compatibility
CFLAGS += -DDEBUG=1
#CFLAGS += -g -DUSE_STDIO=1 -D__GCC_POSIX__=1
CFLAGS += -g -UUSE_STDIO -D__GCC_POSIX__=1
ifneq ($(shell uname), Darwin)
CFLAGS += -pthread
endif

# MAX_NUMBER_OF_TASKS = max pthreads used in the POSIX port. 
# Default value is 64 (_POSIX_THREAD_THREADS_MAX), the minimum number required by POSIX.
CFLAGS += -DMAX_NUMBER_OF_TASKS=300

CFLAGS += $(INCLUDES) $(CWARNS) -O2

######## Makefile targets ########

# Rules
.PHONY : all
all: main edf

# Fix to place .o files in ODIR
_MAIN_OBJS = $(patsubst %,$(ODIR)/%,$(MAIN_OBJS))
_EDF_OBJS = $(patsubst %,$(ODIR)/%,$(EDF_OBJS))

$(ODIR)/%.o: %.c
	@mkdir -p $(dir $@)
ifeq ($(verbose),1)
	@echo ">> Compiling $<"
	$(CC) $(CFLAGS) -c -o $@ $<
else
	@echo ">> Compiling $(notdir $<)"
	@$(CC) $(CFLAGS) -c -o $@ $<
endif

main: $(_MAIN_OBJS)
	@echo ">> Linking $@..."
ifeq ($(verbose),1)
	$(CC) $(CFLAGS) $^ $(LINKFLAGS) $(LIBS) -o $@
else
	@$(CC) $(CFLAGS) $^ $(LINKFLAGS) $(LIBS) -o $@
endif
	@echo "-------------------------"
	@echo "BUILD COMPLETE: $@"
	@echo "-------------------------"

edf: $(_EDF_OBJS)
	@echo ">> Linking $@..."
ifeq ($(verbose),1)
	$(CC) $(CFLAGS) $^ $(LINKFLAGS) $(LIBS) -o $@
else
	@$(CC) $(CFLAGS) $^ $(LINKFLAGS) $(LIBS) -o $@
endif
	@echo "-------------------------"
	@echo "BUILD COMPLETE: $@"
	@echo "-------------------------"

.PHONY : clean
clean:
	@-rm -rf $(ODIR) main edf
	@echo "CLEAN ALL EXECUTABLES"

.PHONY: valgrind
valgrind: main edf
	valgrind.bin --tool=memcheck --leak-check=full --show-reachable=yes --track-fds=yes ./main
