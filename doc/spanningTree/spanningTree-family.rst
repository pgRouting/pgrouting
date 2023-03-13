..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/spanningTree-family.html>`__
  (`3.6 <https://docs.pgrouting.org/3.6/en/spanningTree-family.html>`__)
  `3.5 <https://docs.pgrouting.org/3.5/en/spanningTree-family.html>`__
  `3.4 <https://docs.pgrouting.org/3.4/en/spanningTree-family.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/spanningTree-family.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/spanningTree-family.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/spanningTree-family.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/spanningTree-family.html>`__

Spanning Tree - Category
===============================================================================

.. index from here

* :doc:`kruskal-family`
* :doc:`prim-family`

.. index to here

A spanning tree of an undirected graph is a tree that includes all the vertices
of G with the minimum possible number of edges.

For a disconnected graph, there there is no single tree, but a spanning forest,
consisting of a spanning tree of each connected component.

.. toctree::
    :hidden:

    kruskal-family
    prim-family

Characteristics:

.. spanntree_traits_start

* It's implementation is only on **undirected** graph.
* Process is done only on edges with positive costs.
* When the graph is connected

  * The resulting edges make up a tree

* When the graph is not connected,

  * Finds a minimum spanning tree for each connected component.
  * The resulting edges make up a forest.

.. spanntree_traits_end

See Also
-------------------------------------------------------------------------------

* `Boost: Prim's algorithm
  <https://www.boost.org/libs/graph/doc/prim_minimum_spanning_tree.html>`__
* `Boost: Kruskal's algorithm
  <https://www.boost.org/libs/graph/doc/kruskal_min_spanning_tree.html>`__
* `Wikipedia: Prim's algorithm
  <https://en.wikipedia.org/wiki/Prim%27s_algorithm>`__
* `Wikipedia: Kruskal's algorithm
  <https://en.wikipedia.org/wiki/Kruskal's_algorithm>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
