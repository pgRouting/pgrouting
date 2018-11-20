..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

Prim - Family of functions (Experimental)
===============================================================================


.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. index from here

* :doc:`pgr_prim`
* :doc:`pgr_primDD`
* :doc:`pgr_primDFS`
* :doc:`pgr_primBFS`

.. index to here


.. toctree::
    :hidden:

    pgr_prim
    pgr_primDD
    pgr_primDFS
    pgr_primBFS


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

- It's implementation is only on **undirected graph**.
- Process is done only on edges with positive costs.
- When the graph is connected

  - The resulting edges make up a tree

- When the graph is not connected,

  - Finds a minimum spanning tree for each connected component.
  - The resulting edges make up a forest.

- Prim's running time: :math:`O(E*log V)`

.. prim-description-end


.. Note:: From boost Graph:
   "The algorithm as implemented in Boost.Graph does not produce correct results on graphs with parallel edges."

See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
