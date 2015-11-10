OBJECTS = SparseCompute DebugSparseCompute M5SparseCompute DebugM5SparseCompute
CC = g++
CFLAGS = -O3 -static -Os -DLINUX_BUILD -pthread -march=x86-64 -std=gnu++11
DEBUG_CFLAGS = -g3 -static -DLINUX_BUILD -pthread -march=x86-64 -std=gnu++11
M5_CFLAGS = -O3 -static -Os -DLINUX_BUILD Linux/m5pthread.o -march=x86-64 -std=gnu++11
M5_DEBUG_CFLAGS = -g3 -static -DLINUX_BUILD Linux/m5pthread.o -march=x86-64 -std=gnu++11

SOURCE = Main.cpp LinuxUtils.cpp Utils.cpp Kernels.cpp DataProvider.cpp Linux/*.s 

all: $(OBJECTS) 

SparseCompute: $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $@ 

M5SparseCompute: $(SOURCE)
	$(CC) $(M5_CFLAGS) $(SOURCE) -o $@ 

DebugSparseCompute: $(SOURCE)
	$(CC) $(DEBUG_CFLAGS) $(SOURCE) -o $@ 

DebugM5SparseCompute: $(SOURCE)
	$(CC) $(M5_DEBUG_CFLAGS) $(SOURCE) -o $@ 

clean:
	rm -f *.o *~ $(OBJECTS)
