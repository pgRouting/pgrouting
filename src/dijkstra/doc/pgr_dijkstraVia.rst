.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_dijkstraVia:

pgr_dijkstraVia
===============================================================================


Name
-------------------------------------------------------------------------------

``pgr_dijkstraVia`` — Using dijkstra algorithm, it finds the route that goes through
a list of vertices.


.. figure:: ../../../doc/src/introduction/images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph

   Boost Graph Inside
   

Synopsis
-------------------------------------------------------------------------------

Given a list of vertices and a graph, this function is equivalent to finding the
shortest path between :math:`vertex_i` and :math:`vertex_{i+1}` for all :math:`i < size\_of(vertex_via)`.
The paths represents the sections of the route.

.. note:: This is a proposed function for version 2.3

Signatrue Summary
-----------------

.. code-block:: none

    pgr_dijkstraVia(edges_sql, via_vertices)
    pgr_dijkstraVia(edges_sql, via_vertices, directed:=true, strict:=false, U_turn_on_edge:=true)

    RETURNS SET OF (seq, path_pid, path_seq, start_vid, end_vid,
        node, edge, cost, agg_cost, route_agg_cost) or EMPTY SET

Signatures
===============================================================================

.. index:: 
    single: dijkstraVia(edges_sql, via_vertices) -proposed

Minimal Signature
---------------------------------

.. code-block:: none

    pgr_dijkstraVia(edges_sql, via_vertices)
    RETURNS SET OF (seq, path_pid, path_seq, start_vid, end_vid,
        node, edge, cost, agg_cost, route_agg_cost) or EMPTY SET

:Example: Find the route that visits the vertices 1 3 9  in that order

.. literalinclude:: doc-pgr_dijkstraVia.queries
    :start-after: --q00
    :end-before: -- q0

.. index:: 
    single: dijkstraVia(edges_sql, via_vertices, strict:=false, U_turn_on_edge:=true, directed:=true) -proposed

Complete Signature
------------------

.. code-block:: none

    pgr_dijkstraVia(edges_sql, via_vertices, directed:=true, strict:=false, U_turn_on_edge:=true)
    RETURNS SET OF (seq, path_pid, path_seq, start_vid, end_vid,
        node, edge, cost, agg_cost, route_agg_cost) or EMPTY SET


:Example: Find the route that visits the vertices 1 3 9  in that order on an undirected graph, avoiding U-turns when possible

.. literalinclude:: doc-pgr_dijkstraVia.queries
    :start-after: -- q0
    :end-before: -- q1


Description of the Signature
============================

Description of the SQL query
-------------------------------------------------------------------------------

:edges_sql: is an SQL query, which should return a set of rows with the following columns:

================  ===================   =================================================
Column            Type                      Description
================  ===================   =================================================
**source**        ``ANY-INTEGER``       Identifier of the first end point vertex of the edge.
**target**        ``ANY-INTEGER``       Identifier of the second end point vertex of the edge.
**cost**          ``ANY-NUMERICAL``     Weight of the edge `(source, target)`, if negative: edge `(source, target)` does not exist, therefore it's not part of the graph.
**reverse_cost**  ``ANY-NUMERICAL``     (optional) Weight of the edge `(target, source)`, if negative: edge `(target, source)` does not exist, therefore it's not part of the graph.
================  ===================   =================================================

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, REAL, FLOAT


Description of the parameters of the signatures
-------------------------------------------------------------------------------

Recives  ``(edges_sql, via_vertices, directed:=true, strict:=false, U_turn_on_edge:=true)``

=================== ====================== =================================================
Parameter           Type                   Description
=================== ====================== =================================================
**edges_sql**       ``TEXT``               SQL query as decribed above.
**via_vertices**    ``ARRAY[ANY-INTEGER]`` Array of vertices identifiers
**directed**        ``BOOLEAN``            (optional) Default is true (is directed). When set to false the graph is considered as Undirected
**strict**          ``BOOLEAN``            (optional) ignores if a subsection of the route is missing and returns everything it found Default is true (is directed). When set to false the graph is considered as Undirected
**U_turn_on_edge**  ``BOOLEAN``            (optional) Default is true (is directed). When set to false the graph is considered as Undirected
=================== ====================== =================================================


Description of the return values
-------------------------------------------------------------------------------

Returns set of ``(start_vid, end_vid, agg_cost)``

================== ============= =================================================
Column             Type          Description
================== ============= =================================================
**seq**            ``BIGINT``    Sequential value starting from 1.
**path_pid**       ``BIGINT``    Identifier of the path.
**path_seq**       ``BIGINT``    Sequential value starting from 1 for the path.
**start_vid**      ``BIGINT``    Identifier of the starting vertex of the path.
**end_vid**        ``BIGINT``    Identifier of the ending vertex of the path.
**node**           ``BIGINT``    Identifier of the node in the path from start_vid to end_vid.
**edge**           ``BIGINT``    Identifier of the edge used to go from node to the next node in the path sequence. -1 for the last node of the path. -2 for the last node of the route.
**cost**           ``FLOAT``     Cost to traverse from ``node`` using ``edge`` to the next node in the route sequence.
**agg_cost**       ``FLOAT``     Total cost from ``start_vid`` to ``end_vid`` of the path.
**route_agg_cost** ``FLOAT``     Total cost from ``start_vid`` of ``path_pid = 1`` to ``end_vid`` of the current ``path_pid`` .
================== ============= =================================================


Examples
========

:Example 1: Find the route that visits the vertices 1 5 3 9 4 in that order

.. literalinclude:: doc-pgr_dijkstraVia.queries
    :start-after: -- q1
    :end-before: -- q2

:Example 2: What's the aggregate cost of the third path?

.. literalinclude:: doc-pgr_dijkstraVia.queries
    :start-after: -- q2
    :end-before: -- q3

:Example 3: What's the route's aggregate cost of the route at the end of the third path?

.. literalinclude:: doc-pgr_dijkstraVia.queries
    :start-after: -- q3
    :end-before: -- q4

:Example 4: How are the nodes visited in the route?

.. literalinclude:: doc-pgr_dijkstraVia.queries
    :start-after: -- q4
    :end-before: -- q5

:Example 5: What are the aggregate costs of the route when the visited vertices are reached?

.. literalinclude:: doc-pgr_dijkstraVia.queries
    :start-after: -- q5
    :end-before: -- q6

:Example 6: show the route's seq and aggregate cost and a status of "passes in front" or "visits" node 9

.. literalinclude:: doc-pgr_dijkstraVia.queries
    :start-after: -- q6



The queries use the :ref:`sampledata` network.

.. rubric:: History

* Proposed in version 2.2


See Also
-------------------------------------------------------------------------------

* http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

