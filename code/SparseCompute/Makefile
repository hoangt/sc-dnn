OBJECTS = SparseCompute DebugSparseCompute M5SparseCompute DebugM5SparseCompute ZSIMSparseCompute DebugZSIMSparseCompute
CC = g++

ZSIM_HOOK_DIR=../zsim/misc/hooks

CFLAGS = -O3 -static -Os -DLINUX_BUILD -march=x86-64 -std=gnu++11 
CPU_CFLAGS=$(CFLAGS) -pthread
M5_CFLAGS = $(CFLAGS) Linux/m5pthread.o -DM5_BUILD 
ZSIM_CFLAGS = $(CPU_CFLAGS) -I$(ZSIM_HOOK_DIR) -DZSIM_BUILD 

DEBUG_CFLAGS = -g3 -static -DLINUX_BUILD -march=x86-64 -std=gnu++11
DEBUG_CPU_CFLAGS=$(DEBUG_CFLAGS) -pthread
DEBUG_M5_CFLAGS =$(DEBUG_CFLAGS) Linux/m5pthread.o -DM5_BUILD
DEBUG_ZSIM_CFLAGS = $(DEBUG_CPU_CFLAGS) -I$(ZSIM_HOOK_DIR) -DZSIM_BUILD

#SOURCE = Main.cpp LinuxUtils.cpp Utils.cpp Kernels.cpp DataProvider.cpp Linux/*.s Linux/*.S  
SOURCE = Main.cpp LinuxUtils.cpp Utils.cpp BaselineKernels.cpp DataProvider.cpp Linux/baseline.s 

all: $(OBJECTS) 

SparseCompute: $(SOURCE)
	$(CC) $(CPU_CFLAGS) $(SOURCE) -o $@ 

ZSIMSparseCompute: $(SOURCE)
	$(CC) $(ZSIM_CFLAGS) $(SOURCE) -o $@ 

M5SparseCompute: $(SOURCE)
	$(CC) $(M5_CFLAGS) $(SOURCE) -o $@ 

DebugSparseCompute: $(SOURCE)
	$(CC) $(DEBUG_CPU_CFLAGS) $(SOURCE) -o $@ 

DebugZSIMSparseCompute: $(SOURCE)
	$(CC) $(DEBUG_ZSIM_CFLAGS) $(SOURCE) -o $@ 

DebugM5SparseCompute: $(SOURCE)
	$(CC) $(DEBUG_M5_CFLAGS) $(SOURCE) -o $@ 

clean:
	rm -f *.o *~ $(OBJECTS)
