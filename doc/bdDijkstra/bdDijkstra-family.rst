..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

.. rubric:: Previous versions of this page

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/bdDijkstra-family.html>`__
  (`3.6 <https://docs.pgrouting.org/3.6/en/bdDijkstra-family.html>`__)
  `3.5 <https://docs.pgrouting.org/3.5/en/bdDijkstra-family.html>`__
  `3.4 <https://docs.pgrouting.org/3.4/en/bdDijkstra-family.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/bdDijkstra-family.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/bdDijkstra-family.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/bdDijkstra-family.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/bdDijkstra-family.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/bdDijkstra-family.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/bdDijkstra-family.html>`__

Bidirectional Dijkstra - Family of functions
===============================================================================

.. index from here

* :doc:`pgr_bdDijkstra` - Bidirectional Dijkstra algorithm for the shortest
  paths.
* :doc:`pgr_bdDijkstraCost` - Bidirectional Dijkstra to calculate the cost of
  the shortest paths
* :doc:`pgr_bdDijkstraCostMatrix` - Bidirectional Dijkstra algorithm  to create
  a matrix of costs of the shortest paths.

.. index to here

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

  - It is expected to terminate faster than pgr_dijkstra

.. description end

See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
