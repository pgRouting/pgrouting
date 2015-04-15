CC= g++
CCFLAGS=  -g -O3 -std=c++0x
LDFLAGS= -lboost_program_options
INCFLAGS= -I /usr/include/postgresql/9.4/server/ 
SOURCES= mainP.cpp ./../../common/src/signalhandler.cpp ./../../common/src/pgr_assert.cpp
# Graph.cpp DijkstraShortestPathAlg.cpp YenTopKShortestPathsAlg.cpp vrp_assert.cpp signalhandler.cpp

OBJECTS= $(SOURCES:.cpp=.o)
TARGET= mainP
 
all: $(OBJECTS) $(TARGET) 
 
$(TARGET): signalhandler.o pgr_assert.o mainP.o 
	$(CC)  -o $@ $^ $(LDFLAGS)
 
$(TARGET).o: $(TARGET).cpp ./../../common/src/baseGraph.hpp ./../../common/src/pgr_types.h ./../../dijkstra/src/pgr_dijkstra.hpp pgr_ksp.cpp ./../../common/src/basePath_SSEC.hpp
	$(CC)  $(CCFLAGS) $(INCFLAGS) -c $<
 
%.o: %.cpp %.h 
	$(CC)  $(CCFLAGS) $(INCFLAGS) -c $<

valgrind: mainP
	valgrind -v --track-origins=yes --undef-value-errors=yes --leak-check=full --show-leak-kinds=all ./mainP &> ./valgrindlog.txt
clean:
	rm -f *.o $(TARGET) 

#GraphElements.h Graph.h DijkstraShortestPathAlg.h YenTopKShortestPathsAlg.h BaseEdge.h BaseVertex.h BasePath.h signalHandler.h
