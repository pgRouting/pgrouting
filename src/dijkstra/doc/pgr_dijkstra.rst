..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_dijkstra:

pgr_dijkstra
===============================================================================

``pgr_dijkstra`` — Returns the shortest path(s) using Dijkstra algorithm.
In particular, the Dijkstra algorithm implemented by Boost.Graph.

.. figure:: ../../../doc/src/introduction/images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/dijkstra_shortest_paths.html

   Boost Graph Inside


Synopsis
-------------------------------------------------------------------------------

Dijkstra's algorithm, conceived by Dutch computer scientist Edsger Dijkstra in 1956.
It is a graph search algorithm that solves the shortest path problem for
a graph with non-negative edge path costs, producing a shortest path from
a starting vertex (``start_vid``) to an ending vertex (``end_vid``).
This implementation can be used with a directed graph and an undirected graph.

Characteristics:
----------------

The main Characteristics are:
  - Process is done only on edges with positive costs.
  - Values are returned when there is a path.

    - When the starting vertex and ending vertex are the same, there is no path.

      - The `agg_cost` the non included values `(v, v)` is `0`

    - When the starting vertex and ending vertex are the different and there is no path:

      - The `agg_cost` the non included values `(u, v)` is :math:`\infty`

  - For optimization purposes, any duplicated value in the `start_vids` or `end_vids` are ignored.

  - The returned values are ordered:

    - `start_vid` ascending
    - `end_vid` ascending

  - Runing time: :math:`O(| start\_vids | * (V \log V + E))`


Signature Summary
-----------------

.. code-block:: none

    pgr_dijkstra(edges_sql, start_vid,  end_vid)
    pgr_dijkstra(edges_sql, start_vid,  end_vid,  directed:=true)
    pgr_dijkstra(edges_sql, start_vid,  end_vids, directed:=true)
    pgr_dijkstra(edges_sql, start_vids, end_vid,  directed:=true)
    pgr_dijkstra(edges_sql, start_vids, end_vids, directed:=true)

    RETURNS SET OF (seq, path_seq [, start_vid] [, end_vid], node, edge, cost, agg_cost)
        OR EMPTY SET


Signatures
===============================================================================

.. index::
    single: dijkstra(edges_sql, start_vid, end_vid)

Minimal signature
-----------------

.. code-block:: none

    pgr_dijkstra(TEXT edges_sql, BIGINT start_vid, BIGINT end_vid)
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost) or EMPTY SET

The minimal signature is for a **directed** graph from one ``start_vid`` to one ``end_vid``:

:Example:

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q1
   :end-before: -- q2


.. index::
    single: dijkstra(edges_sql, start_vid, end_vid, directed:=TRUE)

Dijkstra One to One
-------------------

.. code-block:: none

    pgr_dijkstra(TEXT edges_sql, BIGINT start_vid, BIGINT end_vid,
        BOOLEAN directed:=true);
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost) or EMPTY SET

This signature finds the shortest path from one ``start_vid`` to one ``end_vid``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

:Example:

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q2
   :end-before: -- q3


.. index::
    single: dijkstra(edges_sql, start_vid, end_vids, directed:=TRUE)

Dijkstra One to many
--------------------

.. code-block:: none

    pgr_dijkstra(TEXT edges_sql, BIGINT start_vid, ARRAY[ANY_INTEGER] end_vids,
        BOOLEAN directed:=true);
    RETURNS SET OF (seq, path_seq, end_vid, node, edge, cost, agg_cost) or EMPTY SET

This signature finds the shortest path from one ``start_vid`` to each ``end_vid`` in ``end_vids``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

Using this signature, will load once the graph and perform a one to one `pgr_dijkstra`
where the starting vertex is fixed, and stop when all ``end_vids`` are reached.

  - The result is equivalent to the union of the results of the one to one `pgr_dijkstra`.
  - The extra ``end_vid`` in the result is used to distinguish to which path it belongs.

:Example:

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q3
   :end-before: -- q4




.. index::
    single: dijkstra(edges_sql, start_vids, end_vid, directed)

Dijkstra Many to One
--------------------

.. code-block:: none

    pgr_dijkstra(TEXT edges_sql, ARRAY[ANY_INTEGER] start_vids, BIGINT end_vid,
        BOOLEAN directed:=true);
    RETURNS SET OF (seq, path_seq, start_vid, node, edge, cost, agg_cost) or EMPTY SET

This signature finds the shortest path from each ``start_vid`` in  ``start_vids`` to one ``end_vid``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

Using this signature, will load once the graph and perform several one to one `pgr_dijkstra`
where the ending vertex is fixed.

  - The result is the union of the results of the one to one `pgr_dijkstra`.
  - The extra ``start_vid`` in the result is used to distinguish to which path it belongs.

:Example:

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q4
   :end-before: -- q5



.. index::
    single: dijkstra(edges_sql, start_vids, end_vids, directed)

Dijkstra Many to Many
---------------------

.. code-block:: none

    pgr_dijkstra(TEXT edges_sql, ARRAY[ANY_INTEGER] start_vids, ARRAY[ANY_INTEGER] end_vids,
        BOOLEAN directed:=true);
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost) or EMPTY SET

This signature finds the shortest path from each ``start_vid`` in  ``start_vids`` to each ``end_vid`` in ``end_vids``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

Using this signature, will load once the graph and perform several one to Many `pgr_dijkstra`
for all ``start_vids``.

  - The result is the union of the results of the one to one `pgr_dijkstra`.
  - The extra ``start_vid`` in the result is used to distinguish to which path it belongs.

The extra ``start_vid`` and ``end_vid`` in the result is used to distinguish to which path it belongs.

:Example:

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q5
   :end-before: -- q6

Description of the Signatures
=============================

Description of the SQL query
-------------------------------------------------------------------------------

:edges_sql: an SQL query, which should return a set of rows with the following columns:

================  ===================   =================================================
Column            Type                  Description
================  ===================   =================================================
**id**            ``ANY-INTEGER``       Identifier of the edge.
**source**        ``ANY-INTEGER``       Identifier of the first end point vertex of the edge.
**target**        ``ANY-INTEGER``       Identifier of the second end point vertex of the edge.
**cost**          ``ANY-NUMERICAL``     Weight of the edge `(source, target)`, If negative: edge `(source, target)` does not exist, therefore it's not part of the graph.
**reverse_cost**  ``ANY-NUMERICAL``     (optional) Weight of the edge `(target, source)`, If negative: edge `(target, source)` does not exist, therefore it's not part of the graph.
================  ===================   =================================================


Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, REAL, FLOAT


Description of the parameters of the signatures
-------------------------------------------------------------------------------

============== ====================== =================================================
Column         Type                   Description
============== ====================== =================================================
**sql**        ``TEXT``               SQL query as decribed above.
**start_vid**  ``BIGINT``             Identifier of the starting vertex of the path.
**start_vids** ``ARRAY[ANY-INTEGER]`` Array of identifiers of starting vertices.
**end_vid**    ``BIGINT``             Identifier of the ending vertex of the path.
**end_vids**   ``ARRAY[ANY-INTEGER]`` Array of identifiers of ending vertices.
**directed**   ``BOOLEAN``            (optional). When ``false`` the graph is considered as Undirected. Default is ``true`` which considers the graph as Directed.
============== ====================== =================================================


Description of the return values
-------------------------------------------------------------------------------

Returns set of ``(seq, path_seq [, start_vid] [, end_vid], node, edge, cost, agg_cost)``

============== ========== =================================================
Column         Type       Description
============== ========== =================================================
**seq**        ``INT``    Sequential value starting from **1**.
**path_seq**   ``INT``    Relative position in the path. Has value **1** for the begining of a path.
**start_vid**  ``BIGINT`` Identifier of the starting vertex. Used when multiple starting vetrices are in the query.
**end_vid**    ``BIGINT`` Identifier of the ending vertex. Used when multiple ending vertices are in the query.
**node**       ``BIGINT`` Identifier of the node in the path from ``start_vid`` to ``end_vid``.
**edge**       ``BIGINT`` Identifier of the edge used to go from ``node`` to the next node in the path sequence. ``-1`` for the last node of the path.
**cost**       ``FLOAT``  Cost to traverse from ``node`` using ``edge`` to the next node in the path sequence.
**agg_cost**   ``FLOAT``  Aggregate cost from ``start_v`` to ``node``.
============== ========== =================================================


Examples
========

The examples of this section are based on the :ref:`sampledata` network.

The examples include combinations from starting vertices 2 and 11 to ending vertices 3 and 5 in a directed and
undirected graph with and with out reverse_cost.

Examples for queries marked as ``directed`` with ``cost`` and ``reverse_cost`` columns
--------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig1`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q7
   :end-before: -- q8



Examples for queries marked as ``undirected`` with ``cost`` and ``reverse_cost`` columns
----------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig2`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q9
   :end-before: -- q10


Examples for queries marked as ``directed`` with ``cost`` column
----------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig3`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q11
   :end-before: -- q12


Examples for queries marked as ``undirected`` with ``cost`` column
----------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig4`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q13
   :end-before: -- q14


Equvalences between signatures
------------------------------

Examples for queries marked as ``directed`` with ``cost`` and ``reverse_cost`` columns
The examples in this section use the following :ref:`fig1`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q15
   :end-before: -- q16


Equvalences between signatures
------------------------------

Examples for queries marked as ``undirected`` with ``cost`` and ``reverse_cost`` columns
The examples in this section use the following :ref:`fig2`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q17
   :end-before: -- q18


The queries use the :ref:`sampledata` network.

.. rubric:: History

* Renamed in version 2.0.0
* Added functionality in version 2.1.0


See Also
-------------------------------------------------------------------------------

* http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

