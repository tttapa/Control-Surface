CXX := g++
CXXFLAGS := -Wall -std=c++11
SRCS=$(wildcard ../*.cpp) $(wildcard ../*.h)

test: tests.o source
	g++ *.o -o tests && ./tests

tests.o: tests.cpp /home/pieter/lib/Catch2/single_include/catch.hpp
	g++ -c -Wall -std=c++11 -I/home/pieter/lib/Catch2/single_include tests.cpp
    
source: $(SRCS)
	g++ -c -Wall -std=c++11 ../*.cpp

.PHONY: clean
clean:
	$(RM) *.o