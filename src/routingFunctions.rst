.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _routing_functions:


Routing Functions
=================

..
    from-here

:ref:`routing_functions`

  - :ref:`All pairs<all_pairs>` - All pair of vertices.

    - :ref:`pgr_floydWarshall<pgr_floydWarshall>` - Floyd-Warshall's Algorithm
    - :ref:`pgr_johnson<pgr_johnson>`- Johnson’s Algorithm

  - :ref:`pgr_astar<pgr_astar>` - Shortest Path A*
  - :ref:`pgr_bdAstar<bd_astar>` - Bi-directional A* Shortest Path
  - :ref:`pgr_bdDijkstra<bd_dijkstra>` - Bi-directional Dijkstra Shortest Path
  - :ref:`dijkstra<dijkstra>` - Dijkstra family functions

    - :ref:`pgr_dijkstra` - Dijkstra's shortest path algorithm.
    - :ref:`pgr_dijkstraCost` - Use pgr_dijkstra to calculate the costs of the shortest paths.
    - :ref:`pgr_dijkstraVia` - Use pgr_dijkstra to make a route via vertices.
        
  - :ref:`Driving Distance<drivingDistance>` - Driving Distance

    - :ref:`pgr_drivingDistance<pgr_drivingDistance>` - Driving Distance

    - Post processing

      - :ref:`pgr_alphaShape` - Alpha shape computation
      - :ref:`pgr_points_as_polygon` - Polygon around set of points

  - :ref:`pgr_ksp<pgr_ksp>` - K-Shortest Path
  - :ref:`pgr_trsp<trsp>` - Turn Restriction Shortest Path (TRSP)
  - :ref:`tsp`

    - :ref:`pgr_TSP<pgr_TSP>` - When input is a cost matrix.
    - :ref:`pgr_eucledianTSP<pgr_eucledianTSP>` - When input are coordinates.


  - :ref:`pgr_withPoints<withPoints>` - withPoints family functions

    - :ref:`pgr_withPoints` - Route from/to points anywhere on the graph.
    - :ref:`pgr_withPointsCost` - Costs of the shortest paths.
    - :ref:`pgr_withPointsKSP<pgr_withPointsKSP>` - K shortest paths with points.
    - :ref:`pgr_withPointsDD<pgr_withPointsDD>` - Driving distance.

..
    to-here

.. toctree::
    :hidden: 

    allpairs/doc/allpairs
    astar/doc/pgr_astar
    bd_astar/doc/pgr_bdAstar
    bd_dijkstra/doc/pgr_bdDijkstra
    dijkstra/doc/dijkstra
    driving_distance/doc/drivingDistance
    ksp/doc/pgr_ksp
    tsp/doc/tsp
    trsp/doc/pgr_trsp
    withPoints/doc/withPoints


