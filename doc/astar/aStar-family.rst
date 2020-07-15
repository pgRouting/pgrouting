..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

aStar - Family of functions
===============================================================================

The A* (pronounced "A Star") algorithm is based on Dijkstra's algorithm with a heuristic that allow it to solve most shortest path problems by evaluation only a sub-set of the overall graph.

.. index from here

- :doc:`pgr_aStar` - A* algorithm for the shortest path.
- :doc:`pgr_aStarCost` - Get the aggregate cost of the shortest paths.
- :doc:`pgr_aStarCostMatrix` - Get the cost matrix of the shortest paths.

.. index to here

.. toctree::
    :hidden:

    pgr_aStar
    pgr_aStarCost
    pgr_aStarCostMatrix

.. rubric:: Previous versions of this page

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/aStar-family.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/aStar-family.html>`__
  `2.6 <https://docs.pgrouting.org/2.6/en/aStar-family.html>`__

* **Unsupported versions:**
  `2.5 <https://docs.pgrouting.org/2.5/en/aStar-family.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/aStar-family.html>`__

General Information
--------------------------------------------------------------------------------

The main Characteristics are:

.. astar general info start

* Default kind of graph is **directed**  when

  * ``directed`` flag is missing.
  * ``directed`` flag is set to true

* Unless specified otherwise, ordering is:

  *  first by ``start_vid`` (if exists)
  *  then by ``end_vid``

* Values are returned when there is a path
* Let :math:`v` and :math:`u` be nodes on the graph:

  * If there is no path from :math:`v` to :math:`u`:

    * no corresponding row is returned
    * ``agg_cost`` from :math:`v` to :math:`u` is :math:`\infty`

  * There is no path when :math:`v = u` therefore

    * no corresponding row is returned
    * ``agg_cost`` from `v` to `u` is :math:`0`

* Edges with negative costs are not included in the graph.
* When (x,y) coordinates for the same vertex identifier differ:

  * A random selection of the vertex's (x,y) coordinates is used.

* Running time: :math:`O((E + V) * \log V)`

.. astar general info end

Advanced documentation
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
Latitude  Conversion                        Factor
======== ================================= ==========
45       1 longitude degree is  78846.81 m   78846
 0       1 longitude degree is 111319.46 m  111319
======== ================================= ==========

.. rubric:: Analysis 3

Working with cost/reverse_cost as time in seconds, x/y in lat/lon:
Factor: would depend on the location of the points and on the average speed
say 25m/s is the speed.

======== =========================================== ==========
Latitude  Conversion                                  Factor
======== =========================================== ==========
45       1 longitude degree is (78846.81m)/(25m/s)   3153 s
 0       1 longitude degree is (111319.46 m)/(25m/s) 4452 s
======== =========================================== ==========



See Also
-------------------------------------------------------------------------------

* :doc:`pgr_aStar`
* :doc:`pgr_aStarCost`
* :doc:`pgr_aStarCostMatrix`
* https://www.boost.org/libs/graph/doc/astar_search.html
* https://en.wikipedia.org/wiki/A*_search_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
