..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index:: Depth First Search Category

|

DFS - Category
===============================================================================

Traversal using Depth First Search.

.. official-start

* :doc:`pgr_kruskalDFS`
* :doc:`pgr_primDFS`

.. official-end


.. include:: proposed.rst
   :start-after: warning-begin
   :end-before: end-warning

* :doc:`pgr_depthFirstSearch` - Depth first search traversal of the graph.


In general:

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
