# Example 1: Parallel Dijkstra (Basic Code for Small Graph)

* Visualize graph with distributed vertices and edges among processors.
* Apply dijkstra algorithm on distributed graph

## Run the program
```
>>> mpic++ -I/path/to/boost/mpi parallel_dijkstra_test_1.cpp -Llibdir -lboost_mpi -lboost_serialization -lboost_system -lboost_graph_parallel -o parallel_dijkstra_test_1
>>> mpirun -np 2 ./parallel_dijkstra_test_1
```
After running thee above commands, a dot file will be generated as ` test_graph_small_1-dijkstra.dot`.

## Visualize the distributed graph
Convert dot file in distributed graph's image  with following command:
```
>>> dot -Tpng test_graph_small_1-dijkstra.dot > output.png
```

![](https://github.com/codeSG/pgrouting/blob/parallelDijkstra/src/parallel/Example_1/output.png)

Here, **value** inside the nodes shows the distance from the source vertex (distance = 0)