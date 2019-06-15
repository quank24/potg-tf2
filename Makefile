# Makefile for potg

CPPSTD = -std=c++11
# CXXFLAGS =

main:
	g++ ${CPPSTD} ${CXXFLAGS} main.cpp -o potg

all:
	g++ ${CPPSTD} ${CXXFLAGS} main.cpp -o potg;
	cd tests; make

tests:
	cd tests; make tests

tests-windows:
	cd tests; make tests-windows

clean:
	rm potg;
	cd tests; make clean-test

clean-windows:
	rm potg.exe;
	cd tests; make clean-test-windows
