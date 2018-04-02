CXX := g++
CXXFLAGS := -Wall -std=c++11

test: tests.o tests-main.o
	g++ *.o -o tests && ./tests

tests.o: tests.cpp /home/pieter/lib/Catch2/single_include/catch.hpp
	g++ -c -Wall -std=c++11 -I/home/pieter/lib/Catch2/single_include tests.cpp

tests-main.o: tests-main.cpp /home/pieter/lib/Catch2/single_include/catch.hpp
	g++ -c -Wall -std=c++11 -I/home/pieter/lib/Catch2/single_include tests-main.cpp

.PHONY: clean
clean:
	$(RM) *.o