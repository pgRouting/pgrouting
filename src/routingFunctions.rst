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
        
  - :ref:`Driving Distance<drivingDistance>` - Driving Distance

    - :ref:`pgr_drivingDistance<pgr_drivingDistance>` - Driving Distance

    - Post processing

      - :ref:`pgr_alphaShape` - Alpha shape computation
      - :ref:`pgr_points_as_polygon` - Polygon around set of points

  - :ref:`pgr_ksp<pgr_ksp>` - K-Shortest Path
  - :ref:`pgr_trsp<trsp>` - Turn Restriction Shortest Path (TRSP)
  - :ref:`pgr_tsp<pgr_tsp>` - Traveling Sales Person

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
	tsp/doc/pgr_tsp
	trsp/doc/pgr_trsp


