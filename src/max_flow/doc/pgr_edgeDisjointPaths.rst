..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************


.. _pgr_edgeDisjointPaths:

pgr_edgeDisjointPaths
=====================

Name
-------------------------------------------------------------------------------

``pgr_edgeDisjointPaths`` — Calculates edge disjoint paths between two groups of vertices.

.. include:: ../../proposed.rst
   :start-after: begin-warning
   :end-before: end-warning


.. figure:: ../../../doc/src/introduction/images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/boykov_kolmogorov_max_flow.html

   Boost Graph Inside



Synopsis
-------------------------------------------------------------------------------

Calculates the edge disjoint paths between two groups of vertices.
Utilizes underlying maximum flow algorithms to calculate the paths.

Characteristics:
----------------

The main characterics are:
  - Calculates the edge disjoint paths between any two groups of vertices.
  - Returns EMPTY SET when source and destination are the same, or cannot be reached.
  - The graph can be directed or undirected.
  - One to many, many to one, many to many versions are also supported.
  - Uses :ref:`pgr_maxFlowBoykovKolmogorov` to calculate the paths.
  - No `cost` or `aggregate cost` of the paths are returned. (Under discussion)

Signature Summary
-----------------

.. code-block:: none

    pgr_edgeDisjointPaths(edges_sql, source_vertex, destination_vertex)
    pgr_edgeDisjointPaths(edges_sql, source_vertex, destination_vertex, directed)
    pgr_edgeDisjointPaths(edges_sql, source_vertices, destination_vertex, directed)
    pgr_edgeDisjointPaths(edges_sql, source_vertex, destination_vertices, directed)
    pgr_edgeDisjointPaths(edges_sql, source_vertices, destination_vertices, directed)

    RETURNS SET OF (seq, path_seq, [start_vid,] [end_vid,] node, edge) OR EMPTY SET


Signatures
----------

.. index::
    single: edgeDisjointPaths(Minimal Use) - Proposed

Minimal signature
.................

.. code-block:: none

    pgr_edgeDisjointPaths(edges_sql, source_vertex, destination_vertex)
    RETURNS SET OF (seq, path_seq, node, edge) OR EMPTY SET

The minimal signature is between `source_vertex` and `destination_vertex` for a `directed` graph.

:Example:

.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: edgeDisjointPaths(One to One) - Proposed

One to One
.......................................

The available signature calculates edge disjoint paths from one source vertex to one destination vertex.
The graph can be directed or undirected.

.. code-block:: none

    pgr_edgeDisjointPaths(edges_sql, source_vertex, destination_vertex, directed)
    RETURNS SET OF (seq, path_seq, node, edge) OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q2
   :end-before: -- q3


.. index::
    single: edgeDisjointPaths(One to Many) - Proposed

One to Many
.......................................

The available signature calculates the maximum flow from one source vertex to many sink vertices.

.. code-block:: none

    pgr_edgeDisjointPaths(edges_sql, source_vertex, destination_vertices, directed)
    RETURNS SET OF (seq, path_seq, end_vid, node, edge) OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q3
   :end-before: -- q4



.. index::
    single: edgeDisjointPaths(Many to One) - Proposed

Many to One
.......................................

The available signature calculates the maximum flow from many source vertices to one sink vertex.

.. code-block:: none

    pgr_edgeDisjointPaths(edges_sql, source_vertices, destination_vertex)
    RETURNS SET OF (seq, path_seq, start_vid, node, edge)
      OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q4
   :end-before: -- q5



.. index::
    single: edgeDisjointPaths(Many to Many) - Proposed

Many to Many
.......................................

The available signature calculates the maximum flow from many sources to many sinks.

.. code-block:: none

    pgr_edgeDisjointPaths(edges_sql, source_vertices, destination_vertices, directed)
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge) OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q5
   :end-before: -- q6



Description of the Signatures
----------------------------------------------

Description of the SQL query
.......................................

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
........................................................

================= ====================== =================================================
Column            Type                   Description
================= ====================== =================================================
**edges_sql**     ``TEXT``               SQL query as described above.
**source_vertex** ``BIGINT``             Identifier(s) of the source vertex(vertices).
**sink_vertex**   ``BIGINT``             Identifier(s) of the destination vertex(vertices).
**directed**      ``BOOLEAN``            (optional) Determines the type of the graph. Default TRUE.
================= ====================== =================================================

Description of the return values
........................................................

..
   TODO

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

