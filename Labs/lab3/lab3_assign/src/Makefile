CC=g++

CFLAGS=-std=c++11

main: main.cpp process.o frame.o pager_algo.o page_fault_handler.o
	$(CC) $(CFLAGS) main.cpp process.o frame.o pager_algo.o page_fault_handler.o -o main


page_fault_handler.o: page_fault_handler.cpp
	$(CC) $(CFLAGS)  -c page_fault_handler.cpp

pager_algo.o: pager_algo.cpp
	$(CC) $(CFLAGS)  -c pager_algo.cpp

process.o: process.cpp
	$(CC) $(CFLAGS)  -c process.cpp

frame.o: frame.cpp
	$(CC) $(CFLAGS)  -c frame.cpp

clean:
	rm *.o main

