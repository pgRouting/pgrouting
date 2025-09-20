..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index:: Bidirectional A* Family

|

Bidirectional A* - Family of functions
===============================================================================

The bidirectional A* (pronounced "A Star") algorithm is based on the A*
algorithm.

.. official-start

- :doc:`pgr_bdAstar` - Bidirectional A* algorithm for obtaining paths.
- :doc:`pgr_bdAstarCost` - Bidirectional A* algorithm to calculate the cost of
  the paths.
- :doc:`pgr_bdAstarCostMatrix` - Bidirectional A* algorithm to calculate a cost
  matrix of paths.

.. official-end

.. toctree::
    :hidden:

    pgr_bdAstar
    pgr_bdAstarCost
    pgr_bdAstarCostMatrix


Description
-------------------------------------------------------------------------------

Based on A* algorithm, the bidirectional search finds a shortest path from
a starting vertex (``start_vid``) to an ending vertex (``end_vid``).
It runs two simultaneous searches: one forward from the ``start_vid``, and one
backward from the ``end_vid``, stopping when the two meet in the middle.
This implementation can be used with a directed graph and an undirected graph.

The main Characteristics are:

.. include:: aStar-family.rst
    :start-after: astar general info start
    :end-before: astar general info end

* For large graphs where there is a path between the starting vertex and ending
  vertex:

  * It is expected to terminate faster than :doc:`pgr_aStar`

See :ref:`heuristics <astar_heuristics>` available and :ref:`factor
<astar_factor>` handling.

See Also
-------------------------------------------------------------------------------

* :doc:`aStar-family`
* `Boost: A* search <https://www.boost.org/libs/graph/doc/astar_search.html>`__
* https://en.wikipedia.org/wiki/A*_search_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
