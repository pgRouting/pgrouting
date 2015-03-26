CC= g++
CCFLAGS= -Wall -g -O3 -std=c++0x
LDFLAGS= 
INCFLAGS= -I /usr/include/postgresql/9.4/server/
SOURCES= mainP.cpp Graph.cpp DijkstraShortestPathAlg.cpp YenTopKShortestPathsAlg.cpp vrp_assert.cpp signalhandler.cpp
OBJECTS= $(SOURCES:.cpp=.o)
TARGET= mainP
 
all: $(OBJECTS) $(TARGET) 
 
$(TARGET): $(OBJECTS) 
	$(CC) $(LDFLAGS) -o $@ $^
 
%.o: %.cpp %.h 
	$(CC)  $(CCFLAGS) $(INCFLAGS) -c $<
 
$(TARGET).o: $(TARGET).cpp GraphElements.h Graph.h DijkstraShortestPathAlg.h YenTopKShortestPathsAlg.h
	$(CC)  $(CCFLAGS) $(INCFLAGS) -c $<
 
valgrind: mainP
	valgrind -v --track-origins=yes --undef-value-errors=yes --leak-check=full --show-leak-kinds=all ./mainP &> ./valgrindlog.txt
clean:
	rm -f *.o $(TARGET) 

#GraphElements.h Graph.h DijkstraShortestPathAlg.h YenTopKShortestPathsAlg.h BaseEdge.h BaseVertex.h BasePath.h signalHandler.h
