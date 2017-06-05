..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _astar:

aStar - Family of functions
===============================================================================

The A* (pronounced "A Star") algorithm is based on Dijkstra's algorithm with a heuristic that allow it to solve most shortest path problems by evaluation only a sub-set of the overall graph.

.. index from here

- :ref:`pgr_aStar` - A* algorithm for the shortest path.
- :ref:`pgr_aStarCost` - Get the aggregate cost of the shortest paths.
- :ref:`pgr_aStarCostMatrix` - Get the cost matrix of the shortest paths.

.. index to here

.. toctree::
    :hidden:

    pgr_aStar
    pgr_aStarCost
    pgr_aStarCostMatrix


The problem definition (Advanced documentation)
-----------------------------------------------

The A* (pronounced "A Star") algorithm is based on Dijkstra's algorithm with a heuristic, that is an estimation of the remaining cost from the vertex to the goal,
that allows to solve most shortest path problems by evaluation only a sub-set of the overall graph.
Running time: :math:`O((E + V) * \log V)`




.. _astar_heuristics:

Heuristic
..........

Currently the heuristic functions available are:

- 0: :math:`h(v) = 0` (Use this value to compare with pgr_dijkstra)
- 1: :math:`h(v) = abs(max(\Delta x, \Delta y))`
- 2: :math:`h(v) = abs(min(\Delta x, \Delta y))`
- 3: :math:`h(v) = \Delta x * \Delta x + \Delta y * \Delta y`
- 4: :math:`h(v) = sqrt(\Delta x * \Delta x + \Delta y * \Delta y)`
- 5: :math:`h(v) = abs(\Delta x) + abs(\Delta y)`

where :math:`\Delta x = x_1 - x_0` and :math:`\Delta y = y_1 - y_0`


.. _astar_factor:

Factor
-------------------------------------------------------------------------------

.. rubric:: Analysis 1

Working with cost/reverse_cost as length in degrees, x/y in lat/lon:
Factor = 1   (no need to change units)

.. rubric:: Analysis 2

Working with cost/reverse_cost as length in meters, x/y in lat/lon:
Factor =  would depend on the location of the points:

======== ================================= ==========
latitude  conversion                        Factor
======== ================================= ==========
45       1 longitude degree is  78846.81 m   78846
 0       1 longitude degree is 111319.46 m  111319
======== ================================= ==========

.. rubric:: Analysis 3

Working with cost/reverse_cost as time in seconds, x/y in lat/lon:
Factor: would depend on the location of the points and on the average speed
say 25m/s is the speed.

======== =========================================== ==========
latitude  conversion                                  Factor
======== =========================================== ==========
45       1 longitude degree is (78846.81m)/(25m/s)   3153 s
 0       1 longitude degree is (111319.46 m)/(25m/s) 4452 s
======== =========================================== ==========



See Also
-------------------------------------------------------------------------------

* :ref:`pgr_aStar`
* :ref:`pgr_aStarCost`
* :ref:`pgr_aStarCostMatrix`
* http://www.boost.org/libs/graph/doc/astar_search.html
* http://en.wikipedia.org/wiki/A*_search_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
