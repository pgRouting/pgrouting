.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_dijkstra_sql_version:

pgr_dijkstra - Shortest Path Dijkstra
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

Signatures
===============================================================================



.. index:: 
	single: dijkstra(edges_sql, start_vids_sql, end_vid, directed)

.. rubric:: Dijkstra many to 1:

.. code-block:: sql

      pgr_dijkstra(TEXT edges_sql, TEXT start_vids_sql, BIGINT end_vid,
			 BOOLEAN directed:=true);
	 RETURNS SET OF (seq, path_seq, start_vid, node, edge, cost, agg_cost) or EMPTY SET

This signature performs a Dijkstra from each ``start_vid`` in  ``start_vids_sql`` to one ``end_vid``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

Using this signature, will load once the graph and perform several one to one Dijkstra
where the ending vertex is fixed.
The result is the union of the results of the one to one dijkstra.

The extra ``start_vid`` in the result is used to distinguish to which path it belongs.

For example:

.. literalinclude:: ../test/doc_dijkstra_sql_version.doc.result
   :start-after: -- q2
   :end-before: -- q3





.. index:: 
	single: dijkstra(edges_sql, start_vid, end_vids_sql, directed)

.. rubric:: Dijkstra 1 to many:

.. code-block:: sql

       pgr_dijkstra(TEXT edges_sql, BIGINT start_vid, TEXT end_vids_sql,
			 BOOLEAN directed:=true);
	 RETURNS SET OF (seq, path_seq, end_vid, node, edge, cost, agg_cost) or EMPTY SET

This signature performs a Dijkstra from one ``start_vid`` to each ``end_vid`` in ``end_vids_sql``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

Using this signature, will load once the graph and perform several 1 to 1 Dijkstra
where the starting vertex is fixed.
The result is the union of the results of the one to one dijkstra.

The extra ``end_vid`` in the result is used to distinguish to which path it belongs.

For example:

.. literalinclude:: ../test/doc_dijkstra_sql_version.doc.result
   :start-after: -- q1
   :end-before: -- q2


.. index:: 
	single: dijkstra(edges_sql, start_vids_sql, end_vids_sql, directed)

.. rubric:: Dijkstra many to many:

.. code-block:: sql

       pgr_dijkstra(TEXT edges_sql, TEXT start_vids_sql, TEXT end_vids_sql,
			 BOOLEAN directed:=true);
	 RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost) or EMPTY SET

This signature performs a Dijkstra from each ``start_vid`` in  ``start_vids_sql`` to each ``end_vid`` in ``end_vids_sql``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.


Using this signature, will load once the graph and perform all combinations 
for starting vertices and ending vertices.

The extra ``start_vid`` and ``end_vid`` in the result is used to distinguish to which path it belongs.

For example:

.. literalinclude:: ../test/doc_dijkstra_sql_version.doc.result
   :start-after: -- q3



Description of the Signatures
=============================

Description of the edges_sql query
-------------------------------------------------------------------------------

:edges_sql: an SQL query, which should return a set of rows with the following columns:


============== ================= ======================
COLUMN         TYPE              DESCRIPTION
============== ================= ======================
:id:           ``ANY-INTEGER``   identifier of the edge.
:source:       ``ANY-INTEGER``   identifier of the first end point vertex of the edge.
:target:       ``ANY-INTEGER``   identifier of the second end pont vertex of the edge.
:cost:         ``ANY-NUMERICAL`` weight of the edge `(source, target)`, if negative: edge `(source, target)` does not exist, therefore it's not part of the graph.
:reverse_cost: ``ANY-NUMERICAL`` (optional) weight of the edge `(target, source)`, if negative: edge `(target, source)` does not exist, therefore it's not part of the graph.
============== ================= ======================

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, real, float

Description of the start_vids_sql and end_vids_sql  query
-------------------------------------------------------------------------------

:start_vids_sql: is an SQL query.
:end_vids_sql: is an SQL query. 
               
Both should return a set of rows with the following column:

============== ================= ======================
COLUMN         TYPE              DESCRIPTION
============== ================= ======================
:first_column: ``ANY-INTEGER``   identifier of a vertex. 
============== ================= ======================

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT


Description of the parameters of the signatures
-------------------------------------------------------------------------------

================ =========== ===========================
COLUMN           TYPE              DESCRIPTION
================ =========== ===========================
:edges_sql:      ``TEXT``    SQL query as decribed above.
:start_vid:      ``BIGINT``  identifier of the starting vertex of the path.
:start_vids_sql: ``TEXT``    SQL query as described above.
:end_vid:        ``BIGINT``  identifier of the ending vertex of the path.
:start_vids_sql: ``TEXT``    SQL query as described above.
:directed:       ``BOOLEAN`` (optional). When ``false`` the graph is considered as Undirected. Default is ``true`` which considers the graph as Directed.
================ =========== ===========================


Description of the return values
-------------------------------------------------------------------------------

Returns set of ``(seq [, start_vid] [, end_vid] , node, edge, cost, agg_cost)``

=========== ========== ===========================
COLUMN         TYPE              DESCRIPTION
=========== ========== ===========================
:seq:       ``INT``    Sequential value starting from **1**.
:path_seq:  ``INT``    Relative position in the path. Has value **1** for the begining of a path.
:start_vid: ``BIGINT`` Identifier of the starting vertex. Used when multiple starting vetrices are in the query.
:end_vid:   ``BIGINT`` Identifier of the ending vertex. Used when multiple ending vertices are in the query.
:node:      ``BIGINT`` Identifier of the node in the path from start_vid to end_v.
:edge:      ``BIGINT`` Identifier of the edge used to go from ``node`` to the next node in the path sequence. ``-1`` for the last node of the path. 
:cost:      ``FLOAT``  Cost to traverse from ``node`` using ``edge`` to the next node in the path sequence.
:agg_cost:  ``FLOAT``  Total cost from ``start_v`` to ``node``.
=========== ========== ===========================


.. rubric:: History

* Proposed in version 2.2


See Also
-------------------------------------------------------------------------------

* http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

