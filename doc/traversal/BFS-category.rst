..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


BFS - Category
===============================================================================

.. index from here

* :doc:`pgr_kruskalBFS`
* :doc:`pgr_primBFS`

.. index to here


Traversal using breadth first search.

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

.. mst-bfs-dfs-params_start

.. list-table::
   :width: 81
   :widths: 10,20,53
   :header-rows: 1

   * - Parameter
     - Type
     - Description
   * - `Edges SQL`_
     - ``TEXT``
     - `Edges SQL`_ as described below.
   * - **root vid**
     - ``BIGINT``
     - Identifier of the root vertex of the tree.

       * When value is :math:`0` then gets the spanning forest starting in
         aleatory nodes for each tree in the forest.

   * - **root vids**
     - ``ARRAY`` [ **ANY-INTEGER** ]
     -  Array of identifiers of the root vertices.

        * :math:`0` values are ignored
        * For optimization purposes, any duplicated value is ignored.

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERIC: SMALLINT, INTEGER, BIGINT, REAL, FLOAT, NUMERIC

.. mst-bfs-dfs-params_end

BFS optional parameters
...............................................................................

.. max-depth-optional-start

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Parameter
     - Type
     - Default
     - Description
   * - ``max_depth``
     - ``BIGINT``
     - :math:`9223372036854775807`
     - Upper limit of the depth of the tree.

       * When negative throws an error.

.. max-depth-optional-end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

.. mst-bfs-dfs-dd-result-columns-start

Returns SET OF ``(seq, depth, start_vid, node, edge, cost, agg_cost)``

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Parameter
     - Type
     - Description
   * - ``seq``
     - ``BIGINT``
     - Sequential value starting from :math:`1`.
   * - ``depth``
     - ``BIGINT``
     - Depth of the ``node``.

       - :math:`0` when ``node`` = ``start_vid``.

   * - ``start_vid``
     - ``BIGINT``
     - Identifier of the root vertex.

   * - ``node``
     - ``BIGINT``
     - Identifier of ``node`` reached using ``edge``.
   * - ``edge``
     - ``BIGINT``
     - Identifier of the ``edge`` used to arrive to ``node``.

       - :math:`-1` when ``node`` = ``start_vid``.

   * - ``cost``
     - ``FLOAT``
     - Cost to traverse ``edge``.
   * - ``agg_cost``
     - ``FLOAT``
     - Aggregate cost from ``start_vid`` to ``node``.

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERIC: SMALLINT, INTEGER, BIGINT, REAL, FLOAT, NUMERIC

.. mst-bfs-dfs-dd-result-columns-end

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
