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

.. _pgr_maxFlowEdmondsKarp:

pgr_maxFlowEdmondsKarp
======================


Name
----

``pgr_maxFlowEdmondsKarp`` — Calculates the maximum flow in a directed graph given a source and a destination. Implemented by Boost Graph Library.

.. warning::  This is a proposed function.

     - Is not officially in the current release

..
   keep if uses boost (this is a comment)

.. figure:: ../../../doc/src/introduction/images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/graph_theory_review.html#sec:network-flow-algorithms

   Boost Graph Inside


Synopsis
-------------------------------------------------------------------------------

Calculates the maximum flow in a directed graph from a source node to a sink node.
Edges must be weighted with non-negative capacities.
Developed by Edmonds and Karp.
Implementation details_.

.. _details: http://www.boost.org/libs/graph/doc/edmonds_karp_max_flow.html

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

..
   If the function has more than one signature
   Remove the unnecessary parts of the signature, just leaving the name of the parameters
   Like in these examples

.. code-block:: none

    pgr_maxflowedmondskarp(edges_sql, source\_vertex,  sink\_vertex)
    pgr_maxflowedmondskarp(edges_sql, source\_vertices,  sink\_vertex)
    pgr_maxflowedmondskarp(edges_sql, source\_vertex,  sink\_vertices)
    pgr_maxflowedmondskarp(edges_sql, source\_vertices,  sink\_vertices)
    RETURNS SET OF (id, source, target, flow, residual_capacity)
      OR EMPTY SET


..
  This is a reminder of how your query looks like
        pgr_maxFlowEdmondsKarp(
            edges_sql TEXT,
            source_vertex BIGINT,
            sink_vertex BIGINT,
            OUT id BIGINT,
            OUT source BIGINT,
            OUT target BIGINT,
            OUT flow BIGINT,
            OUT residual_capacity BIGINT
        )


Signatures
==========

Edmonds-Karp One to One
-----------------------

The available signature calculates the maximum flow from one source vertex to one sink vertex.

.. code-block:: none

    pgr_maxflowedmondskarp(edges_sql, source\_vertex,  sink\_vertex)
    RETURNS SET OF (id, source, target, flow, residual_capacity)
      OR EMPTY SET

:Example:

.. literalinclude:: doc-maxFlowEdmondsKarp.queries
   :start-after: -- q1
   :end-before: -- q2

Edmonds-Karp One to Many
------------------------

The available signature calculates the maximum flow from one source vertex to many sink vertices.

.. code-block:: none

    pgr_maxflowedmondskarp(edges_sql, source\_vertex,  sink\_vertices)
    RETURNS SET OF (id, source, target, flow, residual_capacity)
      OR EMPTY SET

:Example:

.. literalinclude:: doc-maxFlowEdmondsKarp.queries
   :start-after: -- q2
   :end-before: -- q3

Edmonds-Karp Many to One
------------------------

The available signature calculates the maximum flow from many source vertices to one sink vertex.

.. code-block:: none

    pgr_maxflowedmondskarp(edges_sql, source\_vertices,  sink\_vertex)
    RETURNS SET OF (id, source, target, flow, residual_capacity)
      OR EMPTY SET

:Example:

.. literalinclude:: doc-maxFlowEdmondsKarp.queries
   :start-after: -- q3
   :end-before: -- q4

Edmonds-Karp Many to Many
-------------------------

The available signature calculates the maximum flow from many sources to many sinks.

.. code-block:: none

    pgr_maxflowedmondskarp(edges_sql, source\_vertices,  sink\_vertices)
    RETURNS SET OF (id, source, target, flow, residual_capacity)
      OR EMPTY SET

:Example:

.. literalinclude:: doc-maxFlowEdmondsKarp.queries
   :start-after: -- q4
   :end-before: -- q5

Description of the Signatures
=============================

..
   DELETE / ADD DEPENDING ON YOUR REQUIREMENTS

Description of the SQL query
----------------------------

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
-----------------------------------------------

================= ====================== =================================================
Column            Type                   Description
================= ====================== =================================================
**edges_sql**     ``TEXT``               SQL query as described above.
**source_vertex** ``BIGINT``             Identifier of the source vertex(or vertices).
**sink_vertex**   ``BIGINT``             Identifier of the sink vertex(or vertices).
================= ====================== =================================================


Examples
========

The examples of this section are based on the :ref:`sampledata` network extended with the roads categorization.

:Example:

.. literalinclude:: doc-maxFlowEdmondsKarp.queries
   :start-after: -- q1
   :end-before: -- q2

.. literalinclude:: doc-maxFlowEdmondsKarp.queries
   :start-after: -- q2
   :end-before: -- q3


See Also
--------

* https://en.wikipedia.org/wiki/Edmonds%E2%80%93Karp_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

