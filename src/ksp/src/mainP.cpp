/************************************************************************/
/* $Id: MainP.cpp 65 2010-09-08 06:48:36Z yan.qi.asu $                                                                 */
/************************************************************************/

#include <limits>
#include <set>
#include <map>
#include <queue>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "GraphElements.h"
#include "Graph.h"
#include "DijkstraShortestPathAlg.h"
#include "YenTopKShortestPathsAlg.h"


#if 0
void testDijkstraGraph()
{
        std::string fileName("../devdata/issue285.data");
	Graph my_graph(fileName);
        my_graph.PrintOut(std::cout);
        //copy constructor test
        Graph other_graph(my_graph);
        other_graph.PrintOut(std::cout);
	BasePath path;
        // test copy consturcotr on dijkstra
        DijkstraShortestPathAlg dijkstra_graph(my_graph);
        dijkstra_graph.PrintOut(std::cout);
        
	// test Dijkstra shortest path from 2 to 3 
	path = dijkstra_graph.Dijkstra(2,3);
        path.PrintOut(std::cout);

	// test Dijkstra shortest path from 1 to 4 
        DijkstraShortestPathAlg test14(my_graph);
	path = test14.Dijkstra(1,4);
        path.PrintOut(std::cout);

	// test Dijkstra shortest path from 1 to 1 
        DijkstraShortestPathAlg test11(my_graph);
	path = test11.Dijkstra(1,1);
        path.PrintOut(std::cout);

}

#endif
void yenTest (int testNumb, int from, int to, int k, const Graph &graph) {
	std::deque<BasePath> paths;
        std::cout<<"TEST "<< testNumb<<": "<< k <<" paths from "<< from <<" to "<< to <<" \n";
        YenTopKShortestPathsAlg test(graph);
//test.PrintOut(std::cout);
        paths = test.Yen(from,to,k);
        std::cout<<"paths found "<<paths.size()<<"\n";
        std::cout << "The PATHS" << std::endl;
        for (UINT i=0; i < paths.size(); i++) { 
          std::cout << "*****" << i <<  "*****" << std::endl;
          paths[i].PrintOut(std::cout);
        }
}

void testYenAlg()
{
        std::string fileName("../devdata/sampledata.data");
	Graph my_graph(fileName);
        yenTest(1, 1,2, 1, my_graph);
        yenTest(2, 1,2, 2, my_graph);
        yenTest(2, 1,2, 100, my_graph);
        yenTest(2, 1,6, 1, my_graph);
        yenTest(2, 1,6, 2, my_graph);
        yenTest(2, 1,6, 3, my_graph);
        yenTest(2, 1,6, 100, my_graph);
        yenTest(3, 2,6, 1, my_graph);
        yenTest(3, 2,6, 2, my_graph);
        yenTest(3, 2,6, 3, my_graph);
        yenTest(4, 2,16, 1, my_graph);
        yenTest(3, 2,6, 3, my_graph);
}

void testYenAlgParallel()
{
        std::string fileName("../devdata/issue285.data");
	Graph my_graph(fileName);
        yenTest(1, 2,3, 2, my_graph);
        yenTest(2, 1,3, 2, my_graph);
        yenTest(3, 1,4, 2, my_graph);
        yenTest(4, 1,1, 2, my_graph);
        yenTest(5, 1,2, 2, my_graph);
        yenTest(6, 2,4, 2, my_graph);
        yenTest(7, 2,4, 3, my_graph);
        yenTest(8, 2,3, 3, my_graph);
        yenTest(9, 2,3, 4, my_graph);
        yenTest(10, 2,4, 4, my_graph);
}

void testYenAlgOptimize()
{
        std::string fileName("../devdata/optimizing.data");
        Graph my_graph(fileName);
        //my_graph.PrintOut(std::cout);
        //yenTest(1, 1,3, 1, my_graph);
        //yenTest(1, 1,3, 3, my_graph);
        //yenTest(1, 1,3, 20, my_graph);
        //yenTest(1, 1,5, 3, my_graph);
        yenTest(1, 1,5, 100, my_graph);
}

void testWorkshop()
{
        std::string fileName("../devdata/workshop.data");
        Graph my_graph(fileName);
        //my_graph.PrintOut(std::cout);
        //yenTest(1, 52836,471, 1, my_graph);
        yenTest(1, 52836,471, 5, my_graph);
}
int main(...)
{
	std::cout << "Welcome to the real world!" << std::endl;

	// testDijkstraGraph();
	//testYenAlg();
	//testYenAlgParallel();
	//testYenAlgOptimize();
	testWorkshop();
}
