..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _bddijkstra:

Bidirectional Dijkstra - Family of functions
===============================================================================

.. index from here

* :ref:`pgr_bdDijkstra` - Bidirectional Dijkstra algorithm for the shortest paths.
* :ref:`pgr_bdDijkstraCost` - Bidirectional Dijkstra to calculate the cost of the shortest paths
* :ref:`pgr_bdDijkstraCostMatrix` - Bidirectional Dijkstra algorithm  to create a matrix of costs of the shortest paths.

.. index to here

.. toctree::
    :hidden:

    pgr_bdDijkstra
    pgr_bdDijkstraCost
    pgr_bdDijkstraCostMatrix



Synopsis
-------------------------------------------------------------------------------

Based on Dijkstra's algorithm, the bidirectional search finds a shortest path
a starting vertex (``start_vid``) to an ending vertex (``end_vid``).
It runs two simultaneous searches: one forward from the source, and one backward from the target,
stopping when the two meet in the middle.
This implementation can be used with a directed graph and an undirected graph.

Characteristics
-------------------------------------------------------------------------------

The main Characteristics are:

- Process is done only on edges with positive costs.
- Values are returned when there is a path.

- When the starting vertex and ending vertex are the same, there is no path.

  - The `agg_cost` the non included values `(v, v)` is `0`

- When the starting vertex and ending vertex are the different and there is no path:

  - The `agg_cost` the non included values `(u, v)` is :math:`\infty`

- Running time (worse case scenario): :math:`O((V \log V + E))`
- For large graphs where there is a path bewtween the starting vertex and ending vertex:

  - It is expected to terminate faster than pgr_dijkstra

See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
