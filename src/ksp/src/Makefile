CC= g++
CCFLAGS=  -g -O3 -std=c++0x
LDFLAGS= -lboost_program_options
INCFLAGS= -I /usr/include/postgresql/9.4/server/ 
SOURCES= mainP.cpp signalhandler.cpp vrp_assert.cpp
# Graph.cpp DijkstraShortestPathAlg.cpp YenTopKShortestPathsAlg.cpp vrp_assert.cpp signalhandler.cpp

OBJECTS= $(SOURCES:.cpp=.o)
TARGET= mainP
 
all: $(OBJECTS) $(TARGET) 
 
$(TARGET): $(OBJECTS) 
	$(CC)  -o $@ $^ $(LDFLAGS)
 
$(TARGET).o: $(TARGET).cpp baseGraph.hpp pgr_types.h pgr_dijkstra.hpp YenTopKShortestPathsAlg.h YenTopKShortestPathsAlg.cpp
	$(CC)  $(CCFLAGS) $(INCFLAGS) -c $<
 
%.o: %.cpp %.h 
	$(CC)  $(CCFLAGS) $(INCFLAGS) -c $<

valgrind: mainP
	valgrind -v --track-origins=yes --undef-value-errors=yes --leak-check=full --show-leak-kinds=all ./mainP &> ./valgrindlog.txt
clean:
	rm -f *.o $(TARGET) 

#GraphElements.h Graph.h DijkstraShortestPathAlg.h YenTopKShortestPathsAlg.h BaseEdge.h BaseVertex.h BasePath.h signalHandler.h
