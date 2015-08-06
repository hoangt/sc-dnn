OBJECTS = CanonicalCompute
CC = g++
CFLAGS = -O3 -static -Os 
SOURCE = Main.cpp LinuxUtils.cpp Utils.cpp Linux/*.s 

all: $(OBJECTS) 

$(OBJECTS): $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $@ 

clean:
	rm -f *.o *~ $(OBJECTS)
