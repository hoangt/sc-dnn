OBJECTS = feedforward backprop 
OPT_OBJECTS = feedforward-zero_opt feedforward-simd_opt feedforward-simd_zero_opt backprop-zero_opt 
CC = g++
CFLAGS = -g -O3 -static -march=x86-64 ../m5threads/pthread.o -msse4.1

all: $(OBJECTS) $(OPT_OBJECTS)

$(OBJECTS):%:%.cc utils.cc 
	$(CC) $(CFLAGS) $< utils.cc -o $@ 

feedforward-zero_opt: feedforward.cc utils.cc 
	$(CC) $(CFLAGS) -DZERO_OPT feedforward.cc utils.cc -o $@

feedforward-simd_opt: feedforward.cc utils.cc 
	$(CC) $(CFLAGS) -DSIMD_OPT feedforward.cc utils.cc -o $@

feedforward-simd_zero_opt: feedforward.cc utils.cc 
	$(CC) $(CFLAGS) -DSIMD_OPT -DZERO_OPT feedforward.cc utils.cc -o $@

backprop-zero_opt: backprop.cc utils.cc
	$(CC) $(CFLAGS) -DZERO_OPT backprop.cc utils.cc -o $@

clean:
	rm -f *.o *~ $(OBJECTS) $(OPT_OBJECTS)
