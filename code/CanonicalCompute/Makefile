OBJECTS = CanonicalCompute DebugCanonicalCompute M5CanonicalCompute
CC = g++
CFLAGS = -O3 -static -Os -DLINUX_BUILD -pthread -march=x86-64
DEBUG_CFLAGS = -g3 -static -Os -DLINUX_BUILD -pthread -march=x86-64
M5_CFLAGS = -O3 -static -Os -DLINUX_BUILD Linux/m5pthread.o -march=x86-64
SOURCE = Main.cpp LinuxUtils.cpp Utils.cpp Linux/*.s 

all: $(OBJECTS) 

CanonicalCompute: $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $@ 

M5CanonicalCompute: $(SOURCE)
	$(CC) $(M5_CFLAGS) $(SOURCE) -o $@ 

DebugCanonicalCompute: $(SOURCE)
	$(CC) $(DEBUG_CFLAGS) $(SOURCE) -o $@ 


clean:
	rm -f *.o *~ $(OBJECTS)
