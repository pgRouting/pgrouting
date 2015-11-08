CC=cc
CPP=c++

all: clean test1 test2 test clean2

test1: test1.c ../src/pgr_logger.h
	$(CC) -o test1 test1.c

test2: test2.cpp ../src/pgr_logger.h
	$(CPP) -o test2 test2.cpp

test: clean
	+./test1
	+./test2
	+cat test-log.log

clean:
	+rm -f test1 test2 test-log.log

clean2:
	+rm -f test1 test2 test-log.log
