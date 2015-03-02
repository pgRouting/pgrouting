CC=g++
CCFLAGS=-Wall
LDFLAGS= 
INCFLAGS= -I /usr/include/postgresql/9.3/server/
SOURCES= mainP.cpp Graph.cpp DijkstraShortestPathAlg.cpp YenTopKShortestPathsAlg.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
TARGET= mainP
 
all: $(TARGET)
 
$(TARGET): $(OBJECTS) 
	$(CC) $(LDFLAGS) -o $@ $^
 
%.o: %.cpp %.h GraphElements.h Graph.h DijkstraShortestPathAlg.h YenTopKShortestPathsAlg.h BaseEdge.h BaseVertex.h BasePath.h
	$(CC) $(INCFLAGS) $(CCFLAGS) -c $<
 
$(TARGET).o: $(TARGET).cpp GraphElements.h Graph.h DijkstraShortestPathAlg.h YenTopKShortestPathsAlg.h
	$(CC) $(INCFLAGS) $(CCFLAGS) -c $<
 
clean:
	rm -f *.o $(TARGET) 
