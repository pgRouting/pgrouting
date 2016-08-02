..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************


.. _pgr_maxFlowEdmondsKarp:

pgr_maxFlowEdmondsKarp - Proposed
============================================


Name
----

``pgr_maxFlowEdmondsKarp`` — Calculates the maximum flow in a directed graph given a source and a destination. Implemented by Boost Graph Library.


.. include:: ../../proposed.rst
   :start-after: begin-warning
   :end-before: end-warning


.. figure:: ../../../doc/src/introduction/images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/edmonds_karp_max_flow.html

   Boost Graph Inside


Synopsis
-------------------------------------------------------------------------------

Calculates the maximum flow in a directed graph from a source node to a sink node.
Edges must be weighted with non-negative capacities.
Developed by Edmonds and Karp.


Characteristics:
----------------

The main characterics are:
  - The graph must be directed.
  - Calculates the flow/residual capacity for each edge. In the output, edges with zero flow are omitted.
  - The maximum flow through the graph can be calculated by aggregation on source/sink.
  - Returns nothing if source and sink are the same.
  - Allows multiple sources and sinks (See signatures below).
  - Running time: :math:`O(V * E^2)`.

Signature Summary
-----------------

.. code-block:: none

    pgr_maxFlowEdmondsKarp(edges_sql, source_vertex,  sink_vertex)
    pgr_maxFlowEdmondsKarp(edges_sql, source_vertices,  sink_vertex)
    pgr_maxFlowEdmondsKarp(edges_sql, source_vertex,  sink_vertices)
    pgr_maxFlowEdmondsKarp(edges_sql, source_vertices,  sink_vertices)
    RETURNS SET OF (id, edge_id, source, target, flow, residual_capacity)
      OR EMPTY SET

Signatures
-----------------------

.. index::
    single: maxFlowEdmondsKarp(One to One) - Proposed

One to One
.................................................

Calculates the maximum flow from one source vertex to one sink vertex on a `directed` graph.

.. code-block:: none

    pgr_maxFlowEdmondsKarp(edges_sql, source_vertex,  sink_vertex)
    RETURNS SET OF (id, edge_id, source, target, flow, residual_capacity)
      OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_maxFlowEdmondsKarp.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: maxFlowEdmondsKarp(One to Many) - Proposed

One to Many
.................................................

Calculates the maximum flow from one source vertex to many sink vertices on a `directed` graph.

.. code-block:: none

    pgr_maxFlowEdmondsKarp(edges_sql, source_vertex,  sink_vertices)
    RETURNS SET OF (id, edge_id, source, target, flow, residual_capacity)
      OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_maxFlowEdmondsKarp.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: maxFlowEdmondsKarp(Many to One) - Proposed

Many to One
.................................................

Calculates the maximum flow from many source vertices to one sink vertex on a `directed` graph.

.. code-block:: none

    pgr_maxFlowEdmondsKarp(edges_sql, source_vertices,  sink_vertex)
    RETURNS SET OF (id, edge_id, source, target, flow, residual_capacity)
      OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_maxFlowEdmondsKarp.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: maxFlowEdmondsKarp(Many to Many) - Proposed

Many to Many
.................................................

Calculates the maximum flow from many sources to many sinks on a `directed` graph.

.. code-block:: none

    pgr_maxFlowEdmondsKarp(edges_sql, source_vertices,  sink_vertices)
    RETURNS SET OF (id, edge_id, source, target, flow, residual_capacity)
      OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_maxFlowEdmondsKarp.queries
   :start-after: -- q4
   :end-before: -- q5



Description of the Signatures
-----------------------------------------------

Description of the SQL query
.................................................

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
.................................................

================= ====================== =================================================
Column            Type                   Description
================= ====================== =================================================
**edges_sql**     ``TEXT``               SQL query as described above.
**source_vertex** ``BIGINT``             Identifier of the source vertex(or vertices).
**sink_vertex**   ``BIGINT``             Identifier of the sink vertex(or vertices).
================= ====================== =================================================

Description of the return values
.................................................

TODO 


See Also
--------

* :ref:`maxFlow`
* http://www.boost.org/libs/graph/doc/edmonds_karp_max_flow.html
* https://en.wikipedia.org/wiki/Edmonds%E2%80%93Karp_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

