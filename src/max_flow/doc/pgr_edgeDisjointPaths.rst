..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

   INSTRUCTIONS
   - if section consists of only one value then use this file as index.rst
   - change [...] (including the square braquets) to appropiate values
   - one file / function,  may signatures of the same function go in the same file

.. _pgr_edgeDisjointPaths:

pgr_edgeDisjointPaths
=====================

Name
----

``pgr_edgeDisjointPaths`` — Calculates edge disjoint paths between two groups of vertices.

.. warning::  This is a proposed function.

     - Is not officially in the current release


Synopsis
-------------------------------------------------------------------------------

Calculates the number of edge disjoint paths between two groups of vertices.
Utilizes underlying maximum flow algorithms to calculate the number.

Characteristics:
----------------

The main characterics are:
  - Calculates the number of edge disjoint paths between any two groups of vertices.
  - Returns nothing if source and destination are the same, or cannot be reached.
  - The graph can be directed or undirected.
  - One to many, many to one, many to many versions are also supported.
  - Uses :ref:`pgr_maxFlowBoykovKolmogorov`.

Signature Summary
-----------------

..
   If the function has more than one signature
   Remove the unnecessary parts of the signature, just leaving the name of the parameters
   Like in these examples

.. code-block:: none

    pgr_edgedisjointpaths(edges_sql, source_vertex, destination_vertex)
    pgr_edgedisjointpaths(edges_sql, source_vertices, destination_vertex)
    pgr_edgedisjointpaths(edges_sql, source_vertex, destination_vertices)
    pgr_edgedisjointpaths(edges_sql, source_vertices, destination_vertices)
    pgr_edgedisjointpaths(edges_sql, source_vertex, destination_vertex, directed:=true)
    pgr_edgedisjointpaths(edges_sql, source_vertices, destination_vertex, directed:=true)
    pgr_edgedisjointpaths(edges_sql, source_vertex, destination_vertices, directed:=true)
    pgr_edgedisjointpaths(edges_sql, source_vertices, destination_vertices, directed:=true)

    RETURNS BIGINT

Signatures
----------

Minimal signature
.................

.. code-block:: none

    pgr_edgedisjointpaths(edges_sql, source_vertex, destination_vertex)
    RETURNS SET OF (seq, path_seq, node, edge)

The minimal signature the number of edge disjoint paths between `source_vertex` and `destination_vertex`.
If the directed parameter is not specified, it is assumed that the graph is directed.

:Example:

.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q1
   :end-before: -- q2

Edge Disjoint Paths One to One
------------------------------

The available signature calculates edge disjoint paths from one source vertex to one destination vertex.
The graph can be directed or undirected.

.. code-block:: none

    pgr_edgedisjointpaths(edges_sql, source_vertex, destination_vertex)
    RETURNS SET OF (seq, path_seq, node, edge)
          OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q1
   :end-before: -- q2

.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q2
   :end-before: -- q3

Edge Disjoint Paths One to Many
-------------------------------

The available signature calculates the maximum flow from one source vertex to many sink vertices.

.. code-block:: none

    pgr_edgedisjointpaths(edges_sql, source_vertex, destination_vertices)
    RETURNS SET OF (seq, path_seq, end_vid, node, edge)
          OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q5
   :end-before: -- q6

.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q6
   :end-before: -- q7

Edge Disjoint Paths Many to One
-------------------------------

The available signature calculates the maximum flow from many source vertices to one sink vertex.

.. code-block:: none

    pgr_edgedisjointpaths(edges_sql, source_vertices, destination_vertex)
    RETURNS SET OF (seq, path_seq, start_vid, node, edge)
      OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q3
   :end-before: -- q4

.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q4
   :end-before: -- q5

Edge Disjoint Paths Many to Many
--------------------------------

The available signature calculates the maximum flow from many sources to many sinks.

.. code-block:: none

    pgr_edgedisjointpaths(edges_sql, source_vertices, destination_vertices)
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
      OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q7
   :end-before: -- q8

.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q8
   :end-before: -- q9

Description of the Signatures
=============================

Description of the SQL query
----------------------------

:edges_sql: an SQL query, which should return a set of rows with the following columns:

====================  ===================   =================================================
Column                Type                  Description
====================  ===================   =================================================
**id**                ``ANY-INTEGER``       Identifier of the edge.
**source**            ``ANY-INTEGER``       Identifier of the first end point vertex of the edge.
**target**            ``ANY-INTEGER``       Identifier of the second end point vertex of the edge.
**going**             ``ANY-NUMERIC``       A positive value represents the existence of the edge (source, target).
**coming**            ``ANY-NUMERIC``       A positive value represents the existence of the edge (target, source).
====================  ===================   =================================================

Where:
    - :ANY-INTEGER: SMALLINT, INTEGER, BIGINT
    - :ANY-NUMERIC: SMALLINT, INTEGER, BIGINT, REAL, DOUBLE PRECISION

Description of the parameters of the signatures
-----------------------------------------------

================= ====================== =================================================
Column            Type                   Description
================= ====================== =================================================
**edges_sql**     ``TEXT``               SQL query as described above.
**source_vertex** ``BIGINT``             Identifier(s) of the source vertex(vertices).
**sink_vertex**   ``BIGINT``             Identifier(s) of the destination vertex(vertices).
**directed**      ``BOOLEAN``            (optional) Determines the type of the graph. Default TRUE.
================= ====================== =================================================


.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

