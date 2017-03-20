..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_maxFlow:

pgr_maxFlow Proposed
============================================


Name
----

``pgr_maxFlow`` — Calculates the maximum flow in a directed graph given source(s) and sink(s).


.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr


.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/push_relabel_max_flow.html

   Boost Graph Inside


Synopsis
-------------------------------------------------------------------------------

Calculates the maximum flow in a directed graph from a source node to a sink node.

Characteristics:
----------------

The main characterics are:
  - Calculates the flow/residual capacity for each edge. In the output, edges with zero flow are omitted.
  - The maximum flow through the graph can be calculated by aggregation on source/sink.
  - Edges must be weighted with non-negative capacities.
  - Returns 0 if source and sink are the same.
  - Allows multiple sources and sinks.
  - Running time: :math:`O( V ^ 3)`

Signature Summary
-----------------

.. code-block:: none

    pgr_maxFlow(edges_sql, source_vertex,  sink_vertex)
    pgr_maxFlow(edges_sql, source_vertices,  sink_vertex)
    pgr_maxFlow(edges_sql, source_vertex,  sink_vertices)
    pgr_maxFlow(edges_sql, source_vertices,  sink_vertices)
    RETURNS BIGINT


Signatures
-----------------------

.. index::
    single: maxFlow(One to One) - Proposed

One to One
.....................................................................

Calculates the maximum flow from one source vertex to one sink vertex in a directed graph.

.. code-block:: none

    pgr_maxFlow(edges_sql, source_vertex,  sink_vertex)
    RETURNS BIGINT

:Example:

.. literalinclude:: doc-pgr_maxFlow.queries
   :start-after: -- q1
   :end-before: -- q2


.. index::
    single: maxFlow(One to Many) - Proposed

One to Many
.....................................................................

Ccalculates the maximum flow from one source vertex to many sink vertices in a directed graph.

.. code-block:: none

    pgr_maxFlow(edges_sql, source_vertex,  sink_vertices)
    RETURNS BIGINT

:Example:

.. literalinclude:: doc-pgr_maxFlow.queries
   :start-after: -- q2
   :end-before: -- q3


.. index::
    single: maxFlow(Many to One) - Proposed

Many to One
.....................................................................

Calculates the maximum flow from many source vertices to one sink vertex in a directed graph.

.. code-block:: none

    pgr_maxFlow(edges_sql, source_vertices,  sink_vertex)
    RETURNS BIGINT

:Example:

.. literalinclude:: doc-pgr_maxFlow.queries
   :start-after: -- q3
   :end-before: -- q4


.. index::
    single: maxFlow(Many to Many) - Proposed

Many to Many
.....................................................................

Calculates the maximum flow from many sources to many sinks in a directed graph.

.. code-block:: none

    pgr_maxFlow(edges_sql, source_vertices,  sink_vertices)
    RETURNS BIGINT

:Example:

.. literalinclude:: doc-pgr_maxFlow.queries
   :start-after: -- q4
   :end-before: -- q5

Description of the Signatures
--------------------------------------------------------


Description of the SQL query
.....................................................................

:edges_sql: an SQL query, which should return a set of rows with the following columns:

====================  ===================   =================================================
Column                Type                  Description
====================  ===================   =================================================
**id**                ``ANY-INTEGER``       Identifier of the edge.
**source**            ``ANY-INTEGER``       Identifier of the first end point vertex of the edge.
**target**            ``ANY-INTEGER``       Identifier of the second end point vertex of the edge.
**capacity**          ``ANY-INTEGER``       Capacity of the edge `(source, target)`. Must be positive.
**reverse_capacity**  ``ANY-INTEGER``       (optional) Weight of the edge `(target, source)`. Must be positive or null.
====================  ===================   =================================================

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT


Description of the parameters of the signatures
.....................................................................

================= ====================== =================================================
Column            Type                   Description
================= ====================== =================================================
**edges_sql**     ``TEXT``               SQL query as described above.
**source_vertex** ``BIGINT``             Identifier of the source vertex(or vertices).
**sink_vertex**   ``BIGINT``             Identifier of the sink vertex(or vertices).
================= ====================== =================================================


See Also
--------

* :ref:`maxFlow`
* http://www.boost.org/libs/graph/doc/push_relabel_max_flow.html
* https://en.wikipedia.org/wiki/Push%E2%80%93relabel_maximum_flow_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

