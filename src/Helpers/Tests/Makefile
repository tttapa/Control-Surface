CXX := g++
CXXFLAGS := -Wall -std=c++11 -I/home/pieter/lib/Catch2/single_include

test: tests-linked-list.o tests-bit-array.o tests-main.o
	g++ *.o -o tests && ./tests

tests-linked-list.o: tests-linked-list.cpp
	g++ -c $(CXXFLAGS) tests-linked-list.cpp

tests-bit-array.o: tests-bit-array.cpp
	g++ -c $(CXXFLAGS) tests-bit-array.cpp

tests-main.o: tests-main.cpp
	g++ -c $(CXXFLAGS) tests-main.cpp

.PHONY: clean
clean:
	$(RM) *.o