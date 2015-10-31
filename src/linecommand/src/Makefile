CC= g++
CCFLAGS=  -g -O3 -std=c++11 -Wall -pedantic  
LDFLAGS= -lboost_program_options -lpq 
INCFLAGS= -I /usr/include/postgresql/9.4/server/ -I /usr/include/postgresql/ 
SOURCES= pgRouting.cpp  ./../../common/src/basePath_SSEC.cpp 

OBJECTS= $(SOURCES:.cpp=.o)
TARGET= pgRouting
 
all: $(OBJECTS) $(TARGET) 

$(TARGET): pgRouting.o basePath_SSEC.o 
	$(CC)  -o $@ $^ $(LDFLAGS)
 
$(TARGET).o: $(TARGET).cpp ./../../common/src/baseGraph.hpp \
         ./../../common/src/pgr_types.h \
         ./../../common/src/basePath_SSEC.hpp \
         ./../../dijkstra/src/pgr_dijkstra.hpp \
         ./../../ksp/src/pgr_ksp.hpp \
         ./../../warshall/src/pgr_warshall.hpp \
         ./warshall.hpp
	$(CC)  $(CCFLAGS) $(INCFLAGS) -c $<
 
#%.o: ./../../common/src/%.c ./../../common/src/%.h 
#	g++  -c  $(INCFLAGS) $<

%.o: %.cpp %.hpp 
	$(CC)  $(CCFLAGS) $(INCFLAGS) -c $<

valgrind: pgRouting
	valgrind -v --track-origins=yes --undef-value-errors=yes --leak-check=full --show-leak-kinds=all ./mainP &> ./valgrindlog.txt
clean:
	rm -f *.o $(TARGET) 

