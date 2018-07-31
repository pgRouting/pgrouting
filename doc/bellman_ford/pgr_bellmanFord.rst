..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_bellmanFord:

pgr_bellmanFord - Experimental
===============================================================================

``pgr_bellmanFord`` — Returns the shortest path(s) using Bellman-Ford algorithm.
In particular, the Bellman-Ford algorithm implemented by Boost.Graph.

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/bellman_ford_shortest.html

   Boost Graph Inside

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr


Synopsis
-------------------------------------------------------------------------------

Bellman-Ford's algorithm, is named after Richard Bellman and Lester Ford, who first published it in 1958 and 1956, respectively.
It is a graph search algorithm that computes shortest paths from
a starting vertex (``start_vid``) to an ending vertex (``end_vid``) in a graph where some of the edge weights may be negative number. Though it is more versatile, it is slower than Dijkstra's algorithm/
This implementation can be used with a directed graph and an undirected graph.

Characteristics
-------------------------------------------------------------------------------

The main Characteristics are:
  - Process is valid for edges with both positive and negative edge weights.
  - Values are returned when there is a path.

    - When the start vertex and the end vertex are the same, there is no path. The agg_cost would be 0.

    - When the start vertex and the end vertex are different, and there exists a path between them without having a ‘negative cycle’. The agg_cost would be some finite value denoting the shortest distance between them.
    - When the start vertex and the end vertex are different, and there exists a path between them, but it contains a ‘negative cycle’. In such case, agg_cost for those vertices keep on decreasing furthermore, Hence agg_cost can’t be defined for them.

    - When the start vertex and the end vertex are different, and there is no path. The agg_cost is :math:`\infty`.

  - For optimization purposes, any duplicated value in the `start_vids` or `end_vids` are ignored.

  - The returned values are ordered:

    - `start_vid` ascending
    - `end_vid` ascending

  - Running time: :math:`O(| start\_vids | * ( V * E))`


Signature Summary
-----------------

.. code-block:: none

    pgr_bellmanFord(edges_sql, start_vid,  end_vid)
    pgr_bellmanFord(edges_sql, start_vid,  end_vid,  directed:=true)
    pgr_bellmanFord(edges_sql, start_vid,  end_vids, directed:=true)
    pgr_bellmanFord(edges_sql, start_vids, end_vid,  directed:=true)
    pgr_bellmanFord(edges_sql, start_vids, end_vids, directed:=true)

    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)
        OR EMPTY SET


Signatures
-------------------------------------------------------------------------------

.. index::
    single: bellman_ford(Minimal Use) - Experimental

Minimal signature
.......................................

.. code-block:: none

    pgr_bellmanFord(TEXT edges_sql, BIGINT start_vid, BIGINT end_vid)
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost) or EMPTY SET

The minimal signature is for a **directed** graph from one ``start_vid`` to one ``end_vid``.

:Example:

.. literalinclude:: doc-pgr_bellman_ford.queries
   :start-after: -- q1
   :end-before: -- q2


.. index::
    single: bellman_ford(One to One) - Experimental

pgr_bellmanFord One to One
.......................................

.. code-block:: none

    pgr_bellmanFord(TEXT edges_sql, BIGINT start_vid, BIGINT end_vid,
        BOOLEAN directed:=true);
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost) or EMPTY SET

This signature finds the shortest path from one ``start_vid`` to one ``end_vid``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

:Example:

.. literalinclude:: doc-pgr_bellman_ford.queries
   :start-after: -- q2
   :end-before: -- q3


.. index::
    single: bellman_ford(One to Many) - Experimental

pgr_bellmanFord One to many
.......................................

.. code-block:: none

    pgr_bellmanFord(TEXT edges_sql, BIGINT start_vid, ARRAY[ANY_INTEGER] end_vids,
        BOOLEAN directed:=true);
    RETURNS SET OF (seq, path_seq, end_vid, node, edge, cost, agg_cost) or EMPTY SET

This signature finds the shortest path from one ``start_vid`` to each ``end_vid`` in ``end_vids``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

Using this signature, will load once the graph and perform a one to one `pgr_bellmanFord`
where the starting vertex is fixed, and stop when all ``end_vids`` are reached.

  - The result is equivalent to the union of the results of the one to one `pgr_bellmanFord`.
  - The extra ``end_vid`` in the result is used to distinguish to which path it belongs.

:Example:

.. literalinclude:: doc-pgr_bellman_ford.queries
   :start-after: -- q3
   :end-before: -- q4




.. index::
    single: bellman_ford(Many to One) - Experimental

pgr_bellmanFord Many to One
.......................................

.. code-block:: none

    pgr_bellmanFord(TEXT edges_sql, ARRAY[ANY_INTEGER] start_vids, BIGINT end_vid,
        BOOLEAN directed:=true);
    RETURNS SET OF (seq, path_seq, start_vid, node, edge, cost, agg_cost) or EMPTY SET

This signature finds the shortest path from each ``start_vid`` in  ``start_vids`` to one ``end_vid``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

Using this signature, will load once the graph and perform several one to one `pgr_bellmanFord`
where the ending vertex is fixed.

  - The result is the union of the results of the one to one `pgr_bellmanFord`.
  - The extra ``start_vid`` in the result is used to distinguish to which path it belongs.

:Example:

.. literalinclude:: doc-pgr_bellman_ford.queries
   :start-after: -- q4
   :end-before: -- q5



.. index::
    single: bellman_ford(Many to Many) - Experimental

pgr_bellmanFord Many to Many
.......................................

.. code-block:: none

    pgr_bellmanFord(TEXT edges_sql, ARRAY[ANY_INTEGER] start_vids, ARRAY[ANY_INTEGER] end_vids,
        BOOLEAN directed:=true);
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost) or EMPTY SET

This signature finds the shortest path from each ``start_vid`` in  ``start_vids`` to each ``end_vid`` in ``end_vids``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

Using this signature, will load once the graph and perform several one to Many `pgr_bellmanFord`
for all ``start_vids``.

  - The result is the union of the results of the one to one `pgr_bellmanFord`.
  - The extra ``start_vid`` in the result is used to distinguish to which path it belongs.

The extra ``start_vid`` and ``end_vid`` in the result is used to distinguish to which path it belongs.

:Example:

.. literalinclude:: doc-pgr_bellman_ford.queries
   :start-after: -- q5
   :end-before: -- q6

Parameters
-------------------------------------------------------------------------------

.. pgr_bellmanFord_parameters_start

Description of the parameters of the signatures
...............................................................................

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

