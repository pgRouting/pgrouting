CC=g++
CCFLAGS=-Wall -g -O2
LDFLAGS= 
INCFLAGS= -I /usr/include/postgresql/9.4/server/
SOURCES= mainP.cpp Graph.cpp DijkstraShortestPathAlg.cpp YenTopKShortestPathsAlg.cpp vrp_assert.cpp signalhandler.cpp
OBJECTS=$(SOURCES:.cpp=.o)
TARGET= mainP
 
all: $(TARGET)
 
$(TARGET): $(OBJECTS) 
	$(CC) $(LDFLAGS) -o $@ $^
 
%.o: %.cpp %.h GraphElements.h Graph.h DijkstraShortestPathAlg.h YenTopKShortestPathsAlg.h BaseEdge.h BaseVertex.h BasePath.h signalHandler.h
	$(CC) $(INCFLAGS) $(CCFLAGS) -c $<
 
$(TARGET).o: $(TARGET).cpp GraphElements.h Graph.h DijkstraShortestPathAlg.h YenTopKShortestPathsAlg.h
	$(CC) $(INCFLAGS) $(CCFLAGS) -c $<
 
valgrind: mainP
	valgrind -v --track-origins=yes --undef-value-errors=yes --leak-check=full --show-leak-kinds=all ./mainP &> ./valgrindlog.txt
clean:
	rm -f *.o $(TARGET) 
