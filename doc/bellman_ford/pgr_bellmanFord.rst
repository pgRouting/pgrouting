..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_bellmanFord - Experimental
===============================================================================

``pgr_bellmanFord`` — Returns the shortest path(s) using Bellman-Ford algorithm.
In particular, the Bellman-Ford algorithm implemented by Boost.Graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/bellman_ford_shortest.html

   Boost Graph Inside

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 3.0.0

  * New **experimental** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_bellmanFord.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_bellmanFord.html>`__

Description
-------------------------------------------------------------------------------

Bellman-Ford's algorithm, is named after Richard Bellman and Lester Ford, who first published it in 1958 and 1956, respectively.
It is a graph search algorithm that computes shortest paths from
a starting vertex (``start_vid``) to an ending vertex (``end_vid``) in a graph where some of the edge weights may be negative number. Though it is more versatile, it is slower than Dijkstra's algorithm/
This implementation can be used with a directed graph and an undirected graph.

The main characteristics are:
  - Process is valid for edges with both positive and negative edge weights.
  - Values are returned when there is a path.

    - When the start vertex and the end vertex are the same, there is no path. The agg_cost would be 0.

    - When the start vertex and the end vertex are different, and there exists a path between them without having a *negative cycle*. The agg_cost would be some finite value denoting the shortest distance between them.
    - When the start vertex and the end vertex are different, and there exists a path between them, but it contains a *negative cycle*. In such case, agg_cost for those vertices keep on decreasing furthermore, Hence agg_cost can’t be defined for them.

    - When the start vertex and the end vertex are different, and there is no path. The agg_cost is :math:`\infty`.

  - For optimization purposes, any duplicated value in the `start_vids` or `end_vids` are ignored.

  - The returned values are ordered:

    - `start_vid` ascending
    - `end_vid` ascending

  - Running time: :math:`O(| start\_vids | * ( V * E))`


Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_bellmanFord(edges_sql, from_vid,  to_vid  [, directed])
    pgr_bellmanFord(edges_sql, from_vid,  to_vids [, directed])
    pgr_bellmanFord(edges_sql, from_vids, to_vid  [, directed])
    pgr_bellmanFord(edges_sql, from_vids, to_vids [, directed])

    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)
    OR EMPTY SET

.. rubric:: Using defaults

.. code-block:: none

    pgr_bellmanFord(TEXT edges_sql, BIGINT start_vid, BIGINT end_vid)
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertex :math:`3` on a **directed** graph

.. literalinclude:: doc-pgr_bellmanFord.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: bellman_ford(One to One) - Experimental

One to One
...............................................................................

.. code-block:: none

    pgr_bellmanFord(edges_sql, from_vid,  to_vid  [, directed])
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertex :math:`3` on an **undirected** graph

.. literalinclude:: doc-pgr_bellmanFord.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: bellmanFord(One to Many) - Experimental

One to many
...............................................................................

.. code-block:: none

    pgr_bellmanFord(edges_sql, from_vid,  to_vids [, directed])
    RETURNS SET OF (seq, path_seq, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertices :math:`\{ 3, 5\}` on an **undirected** graph

.. literalinclude:: doc-pgr_bellmanFord.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: bellman_ford(Many to One) - Experimental

Many to One
...............................................................................

.. code-block:: none

    pgr_bellmanFord(edges_sql, from_vids, to_vid  [, directed])
    RETURNS SET OF (seq, path_seq, start_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertices :math:`\{2, 11\}` to vertex :math:`5` on a **directed** graph

.. literalinclude:: doc-pgr_bellmanFord.queries
   :start-after: -- q4
   :end-before: -- q5

.. index::
    single: bellmanFord(Many to Many) - Experimental

Many to Many
...............................................................................

.. code-block:: none

    pgr_bellmanFord(edges_sql, from_vids, to_vids [, directed])
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertices :math:`\{2, 11\}` to vertices :math:`\{3, 5\}` on an **undirected** graph

.. literalinclude:: doc-pgr_bellmanFord.queries
   :start-after: -- q5
   :end-before: -- q6

Parameters
-------------------------------------------------------------------------------

.. pgr_bellmanFord_parameters_start

.. rubric:: Description of the parameters of the signatures

============== ================== ======== =================================================
Parameter      Type               Default     Description
============== ================== ======== =================================================
**edges_sql**        ``TEXT``                    SQL query as described above.
**start_vid**  ``BIGINT``                  Identifier of the starting vertex of the path.
**start_vids** ``ARRAY[BIGINT]``           Array of identifiers of starting vertices.
**end_vid**    ``BIGINT``                  Identifier of the ending vertex of the path.
**end_vids**   ``ARRAY[BIGINT]``           Array of identifiers of ending vertices.
**directed**   ``BOOLEAN``        ``true`` - When ``true`` Graph is considered `Directed`
                                           - When ``false`` the graph is considered as `Undirected`.
============== ================== ======== =================================================

.. pgr_bellmanFord_parameters_end

Inner Query
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Results Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_path_short_start
    :end-before: return_path_short_end

See Also
-------------------------------------------------------------------------------

* https://en.wikipedia.org/wiki/Bellman%E2%80%93Ford_algorithm
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

