.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_withPoints:

pgr_withPoints
===============================================================================

.. index:: 
    single: pgr_pgr_withPointsVia(text, ARRAY[BIGINT], ARRAY[FLOAT], boolean)

Name
-------------------------------------------------------------------------------

.. warning::
    This is a proposed function, signature can change at any time without notice. 

``pgr_withPointsVia`` — Using dijkstra algorithm, it finds the route that goes through
a list of points in edges.


.. figure:: ../../../doc/src/introduction/images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph

   Boost Graph Inside


Synopsis
-------------------------------------------------------------------------------

Given a graph and a list of positional vertices given as list of edges and the corresponding
relative position of the vertex on the edge given as a fraction, this function finds the route that goes
Thru all the positional vertices.
This function is equivalent to finding the shortest path between temporary vertices.


Summary of signatures
-------------------------------------------------------------------------------

.. code-block:: none

    pgr_withPointsVia(edges_sql, eids, positions)
    pgr_withPointsVia(edges_sql, eids, positions, directed)
    RETURNS SET OF (seq, path_pid, path_seq, start_vid, end_vid,
        node, edge, cost, agg_cost, route_agg_cost) or EMPTY SET


Characteristics:
----------------

The main Characteristics are:
  - For the moment code is in plpsql.
  - Uses the also proposed function:

    - pgr_dijstraViaVertex

  - Process is done only on edges with positive costs.
  
  - Temporary Vertices are created as follows:

    - The Temporary vertex is assigned an identifier:

      - For the row ``i`` of the array, the identifier will be ``-i``.
      - The reason for this is to make sure it doesn't get the same internal
        idetifier of a vertex that already exists.
      - In the results any negative value representing a vertex identifier means
        that its a temporary vertex.
      - Except the case where the vertex position is 0 or 1 then no new vertex is
        added and the original vertex's id is used

  - For processing temporary edges are inserted in the graph as follows

    - substituing the original edge by two edges.
    - That is done for the "going" and "coming" edges if they exist.
           



Signatures
===============================================================================

.. rubric:: pgr_withPointsVia Minimal signature

The minimal signature is for a **directed** graph.

.. code-block:: none

    pgr_withPointsVia(edges_sql text, eid array[ANY_INTEGER], position array[FLOAT8])
    RETURNS SET OF (seq, path_pid, path_seq, start_vid, end_vid,
        node, edge, cost, agg_cost, route_agg_cost) or EMPTY SET


.. rubric:: pgr_withPointsVia complete signature

This signature works: 
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.


.. code-block:: none

    pgr_withPointsVia(edges_sql text, eid array[ANY_INTEGER], position array[FLOAT8],
        directed BOOLEAN)
    RETURNS SET OF (seq, path_pid, path_seq, start_vid, end_vid,
        node, edge, cost, agg_cost, route_agg_cost) or EMPTY SET


Description of the Signatures
=============================

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

Recives  ``(edges_sql, eid, position, directed)``

============== ====================== =================================================
Parameter      Type                   Description
============== ====================== =================================================
**edges_sql**  ``TEXT``               SQL query as decribed above.
**eid**        ``ARRAY[ANY-INTEGER]`` Array of edges identifiers. Where the temprary verties are going to be placed
**position**   ``ARRAY[FLOAT]``       Array relative position on the corresponding edge ``eid`` of the temporary vertex
**directed**   ``BOOLEAN``            (optional) Default is true (is directed). When set to false the graph is considered as Undirected
============== ====================== =================================================



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

Edges: 4, 5, and 14 are used in this examples.

.. literalinclude:: doc-withPointsVia.result
   :start-after: -- q0
   :end-before: -- q1

With reverse_cost
-----------------

:Example 1: Directed graph with and without flag.

.. literalinclude:: doc-withPointsVia.result
   :start-after: -- q1
   :end-before: -- q2


:Example 2: Undirected graph has a flag.

.. literalinclude:: doc-withPointsVia.result
   :start-after: -- q2
   :end-before: -- q3

Only cost is used
-----------------

:Example 3: Directed graph.

.. literalinclude:: doc-withPointsVia.result
   :start-after: -- q3
   :end-before: -- q4


:Example 4: Undirected graph.

.. literalinclude:: doc-withPointsVia.result
   :start-after: -- q4
   :end-before: -- q5


The queries use the :ref:`sampledata` network.

.. rubric:: History

* Renamed in version 2.0.0 
* Added functionality for version 3.0.0 in version 2.1


See Also
-------------------------------------------------------------------------------

* http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
