..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/prim-family.html>`__
  (`3.6 <https://docs.pgrouting.org/3.6/en/prim-family.html>`__)
  `3.5 <https://docs.pgrouting.org/3.5/en/prim-family.html>`__
  `3.4 <https://docs.pgrouting.org/3.4/en/prim-family.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/prim-family.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/prim-family.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/prim-family.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/prim-family.html>`__

Prim - Family of functions
===============================================================================

.. index from here

* :doc:`pgr_prim`
* :doc:`pgr_primBFS`
* :doc:`pgr_primDD`
* :doc:`pgr_primDFS`

.. index to here

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/prim_minimum_spanning_tree.html

   Boost Graph Inside

.. toctree::
    :hidden:

    pgr_prim
    pgr_primBFS
    pgr_primDD
    pgr_primDFS


Description
-------------------------------------------------------------------------------

The prim algorithm was developed in 1930 by Czech mathematician Vojtěch Jarník.
It is a greedy algorithm that finds a minimum spanning tree for a weighted
undirected graph. This means it finds a subset of the edges that forms a tree
that includes every vertex, where the total weight of all the edges in the tree
is minimized. The algorithm operates by building this tree one vertex at a time,
from an arbitrary starting vertex, at each step adding the cheapest possible
connection from the tree to another vertex.

This algorithms find the minimum spanning forest in a possibly disconnected
graph; in contrast, the most basic form of Prim's algorithm only finds minimum
spanning trees in connected graphs. However, running Prim's algorithm separately
for each connected component of the graph, then it is called minimum spanning
forest.

**The main characteristics are:**

.. prim-description-start

.. include:: spanningTree-family.rst
   :start-after: spanntree_traits_start
   :end-before: spanntree_traits_end

- Prim's running time: :math:`O(E * log V)`

.. prim-description-end


.. Note:: From boost Graph:
   "The algorithm as implemented in Boost.Graph does not produce correct results
   on graphs with parallel edges."

Inner Queries
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
   :start-after: basic_edges_sql_start
   :end-before: basic_edges_sql_end

See Also
-------------------------------------------------------------------------------

* :doc:`spanningTree-family`
* Boost: `Prim's algorithm
  <https://www.boost.org/libs/graph/doc/prim_minimum_spanning_tree.html>`__
* Wikipedia: `Prim's algorithm
  <https://en.wikipedia.org/wiki/Prim%27s_algorithm>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
