..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/DFS-category.html>`__
  (`3.4 <https://docs.pgrouting.org/3.4/en/DFS-category.html>`__)

DFS - Category
===============================================================================

Traversal using Depth First Search.

.. index from here

* :doc:`pgr_kruskalDFS`
* :doc:`pgr_primDFS`

.. index to here


.. rubric:: Proposed

.. include:: proposed.rst
   :start-after: stable-begin-warning
   :end-before: stable-end-warning

* :doc:`pgr_depthFirstSearch` - Depth first search traversal of the graph.


.. spanntree_traits_start

* It's implementation is only on **undirected** graph.
* Process is done only on edges with positive costs.
* When the graph is connected

  * The resulting edges make up a tree

* When the graph is not connected,

  * Finds a minimum spanning tree for each connected component.
  * The resulting edges make up a forest.

.. spanntree_traits_end

Parameters
-------------------------------------------------------------------------------

.. include:: BFS-category.rst
   :start-after: mst-bfs-dfs-params_start
   :end-before: mst-bfs-dfs-params_end

DFS optional parameters
...............................................................................

.. include:: BFS-category.rst
   :start-after: max-depth-optional-start
   :end-before: max-depth-optional-end

Inner queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

.. include:: BFS-category.rst
   :start-after: mst-bfs-dfs-dd-result-columns-start
   :end-before: mst-bfs-dfs-dd-result-columns-end

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
