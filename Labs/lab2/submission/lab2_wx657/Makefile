CC=g++

CFLAGS=-std=c++11

main: main.cpp DESLayer.o event.o scheduler.o process.o
	$(CC) $(CFLAGS) main.cpp DESLayer.o event.o scheduler.o process.o -o main

DESLayer.o: DESLayer.cpp event.o
	$(CC) $(CFLAGS)  -c DESLayer.cpp event.o

event.o: event.cpp process.o
	$(CC) $(CFLAGS)  -c event.cpp process.o

scheduler.o: scheduler.cpp process.o
	$(CC) $(CFLAGS)  -c scheduler.cpp process.o

process.o: process.cpp
	$(CC) $(CFLAGS)  -c process.cpp

clean:
	rm *.o main
