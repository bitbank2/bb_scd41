CFLAGS= -D__LINUX__ -c -Wall -O2
LIBS = -lm -lOneBitDisplay -lbb_scd41 -lpthread -lgpiod

all: shared_bus

shared_bus: main.o
	g++ main.o $(LIBS) -o shared_bus

main.o: main.cpp
	g++ $(CFLAGS) main.cpp

clean:
	rm *.o shared_bus
