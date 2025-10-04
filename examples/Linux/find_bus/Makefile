CFLAGS= -D__LINUX__ -c -Wall -O2
LIBS = -lm -lbb_scd41 -lpthread

all: find_bus

find_bus: main.o
	g++ main.o $(LIBS) -o find_bus

main.o: main.cpp
	g++ $(CFLAGS) main.cpp

clean:
	rm *.o find_bus
