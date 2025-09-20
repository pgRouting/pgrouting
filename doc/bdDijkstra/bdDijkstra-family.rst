..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************


.. index:: Bidirectional Dijkstra Family

|

Bidirectional Dijkstra - Family of functions
===============================================================================

.. official-start

* :doc:`pgr_bdDijkstra` - Bidirectional Dijkstra algorithm for the shortest
  paths.
* :doc:`pgr_bdDijkstraCost` - Bidirectional Dijkstra to calculate the cost of
  the shortest paths
* :doc:`pgr_bdDijkstraCostMatrix` - Bidirectional Dijkstra algorithm to create
  a matrix of costs of the shortest paths.

.. official-end

.. toctree::
    :hidden:

    pgr_bdDijkstra
    pgr_bdDijkstraCost
    pgr_bdDijkstraCostMatrix


Synopsis
-------------------------------------------------------------------------------

Based on Dijkstra's algorithm, the bidirectional search finds a shortest path
a starting vertex to an ending vertex.

It runs two simultaneous searches: one forward from the source, and one backward
from the target, stopping when the two meet in the middle.

This implementation can be used with a directed graph and an undirected graph.

Characteristics
-------------------------------------------------------------------------------


The main Characteristics are:

.. description start

- Process is done only on edges with positive costs.

  - A negative value on a cost column is interpreted as the edge does not exist.

- Values are returned when there is a path.
- When there is no path:

  - When the starting vertex and ending vertex are the same.

    - The **aggregate cost** of the non included values :math:`(v, v)` is
      :math:`0`

  - When the starting vertex and ending vertex are the different and there is
    no path:

    - The **aggregate cost** the non included values :math:`(u, v)` is
      :math:`\infty`

- For optimization purposes, any duplicated value in the starting vertices or on
  the ending vertices are ignored.
- Running time (worse case scenario): :math:`O((V \log V + E))`
- For large graphs where there is a path bewtween the starting vertex and ending
  vertex:

  - It is expected to terminate faster than :doc:`pgr_dijkstra`

.. description end

See Also
-------------------------------------------------------------------------------

* `Boost  <https://www.boost.org/libs/graph/doc>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
