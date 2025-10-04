CFLAGS=-c -Wall -O2 -D__LINUX__ -I../src
LIBS = -lm -lpthread

all: libbb_scd41.a

libbb_scd41.a: bb_scd41.o
	ar -rc libbb_scd41.a bb_scd41.o ;\
	sudo cp libbb_scd41.a /usr/local/lib ;\
	sudo cp ../src/bb_scd41.h /usr/local/include

bb_scd41.o: ../src/linux_io.inl ../src/bb_scd41.cpp ../src/bb_scd41.h
	$(CXX) $(CFLAGS) ../src/bb_scd41.cpp

clean:
	rm *.o libbb_scd41.a
