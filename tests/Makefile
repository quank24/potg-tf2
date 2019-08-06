# Makefile for potg tests

tests:
	g++ -I/c/googletest-master/googletest/include -pthread potg_functions_tests.cpp /c/googletest-master/libgtest_main.a -o potg_tests

tests-windows:
	g++ -I/cygdrive/c/googletest-master/googletest/include -pthread potg_functions_tests.cpp /cygdrive/c/googletest-master/libgtest_main.a -o potg_tests
# there is cygdrive before C:/ bc cygwin I guess?
	
clean-test:
	rm potg_tests
	
clean-test-windows:
	rm potg_tests.exe
