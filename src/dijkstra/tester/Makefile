
all: testit

testit: testit.o boost_wrapper.o
	g++ -g -O0 -o testit testit.o boost_wrapper.o -L/usr/lib -lm

testit.o: testit.c
	gcc -g -O0 -c testit.c

boost_wrapper.o: boost_wrapper.cpp
	g++ -g -O0 -I/usr/include -c boost_wrapper.cpp

clean:
	rm -f *.o testit
