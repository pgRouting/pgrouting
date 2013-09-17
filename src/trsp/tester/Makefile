all: testit

testit: testit.cpp ../src/GraphDefinition.h ../src/utils.h ../src/GraphDefinition.cpp
	g++ -g -O0 -I. -I../src testit.cpp ../src/GraphDefinition.cpp -o testit

clean:
	rm -f testit *.txt
