CC=g++

CFLAGS=-std=c++11

main: main.cpp scheduler.o
	$(CC) $(CFLAGS) main.cpp scheduler.o -o main


scheduler.o: scheduler.cpp
	$(CC) $(CFLAGS)  -c scheduler.cpp

clean:
	rm *.o main

