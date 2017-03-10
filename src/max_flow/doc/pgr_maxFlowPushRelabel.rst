..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

   INSTRUCTIONS
   - if section consists of only one value then use this file as index.rst
   - change [...] (including the square braquets) to appropriate values
   - one file / function,  may signatures of the same function go in the same file

.. _pgr_maxFlowPushRelabel:

pgr_maxFlowPushRelabel Proposed
============================================


Name
----

``pgr_maxFlowPushRelabel`` — Calculates the maximum flow in a directed graph given a source and a destination.


.. include:: ../../proposedNext.rst
   :start-after: begin-warning
   :end-before: end-warning


.. figure:: ../../../doc/src/introduction/images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/push_relabel_max_flow.html 

   Boost Graph Inside


Synopsis
-------------------------------------------------------------------------------

Calculates the maximum flow in a directed graph from a source node to a sink node.
Edges must be weighted with non-negative capacities.

Characteristics:
----------------

The main characterics are:
  - Calculates the flow/residual capacity for each edge. In the output, edges with zero flow are omitted.
  - The maximum flow through the graph can be calculated by aggregation on source/sink.
  - Returns nothing if source and sink are the same.
  - Allows multiple sources and sinks.
  - Running time: :math:`O( V ^ 3)`

Signature Summary
-----------------

.. code-block:: none

    pgr_maxFlowPushRelabel(edges_sql, source,  target)
    pgr_maxFlowPushRelabel(edges_sql, sources, target)
    pgr_maxFlowPushRelabel(edges_sql, source,  targets)
    pgr_maxFlowPushRelabel(edges_sql, sources, targets)
    RETURNS SET OF (seq, edge_id, source, target, flow, residual_capacity)
      OR EMPTY SET


Signatures
-----------------------

.. index::
    single: maxFlowPushRelabel(One to One) - Proposed

One to One
.....................................................................

Calculates the maximum flow from one source vertex to one sink vertex in a directed graph.

.. code-block:: none

    pgr_maxFlowPushRelabel(edges_sql, source,  target)
    RETURNS SET OF (seq, edge_id, source, target, flow, residual_capacity)
      OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_maxFlowPushRelabel.queries
   :start-after: -- q1
   :end-before: -- q2


.. index::
    single: maxFlowPushRelabel(One to Many) - Proposed

One to Many
.....................................................................

Ccalculates the maximum flow from one source vertex to many sink vertices in a directed graph.

.. code-block:: none

    pgr_maxFlowPushRelabel(edges_sql, source,  targets)
    RETURNS SET OF (seq, edge_id, source, target, flow, residual_capacity)
      OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_maxFlowPushRelabel.queries
   :start-after: -- q2
   :end-before: -- q3


.. index::
    single: maxFlowPushRelabel(Many to One) - Proposed

Many to One
.....................................................................

Calculates the maximum flow from many source vertices to one sink vertex in a directed graph.

.. code-block:: none

    pgr_maxFlowPushRelabel(edges_sql, sources,  target)
    RETURNS SET OF (seq, edge_id, source, target, flow, residual_capacity)
      OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_maxFlowPushRelabel.queries
   :start-after: -- q3
   :end-before: -- q4


.. index::
    single: maxFlowPushRelabel(Many to Many) - Proposed

Many to Many
.....................................................................

Calculates the maximum flow from many sources to many sinks in a directed graph.

.. code-block:: none

    pgr_maxFlowPushRelabel(edges_sql, sources,  targets)
    RETURNS SET OF (seq, edge_id, source, target, flow, residual_capacity)
      OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_maxFlowPushRelabel.queries
   :start-after: -- q4
   :end-before: -- q5

Description of the Signatures
--------------------------------------------------------

.. include:: ../../../doc/src/tutorial/custom_query.rst
    :start-after: flow_edges_sql_start
    :end-before: flow_edges_sql_end


.. include::  ./maxFlow.rst
    :start-after: pgr_flow_parameters_start
    :end-before: pgr_flow_parameters_end


.. result_start

Description of the Return Values
.....................................................................

=====================  ====================  =================================================
Column                 Type                  Description
=====================  ====================  =================================================
**seq**                ``INT``               Sequential value starting from **1**.
**edge_id**            ``BIGINT``            Identifier of the edge in the original query(edges_sql).
**source**             ``BIGINT``            Identifier of the first end point vertex of the edge.
**target**             ``BIGINT``            Identifier of the second end point vertex of the edge.
**flow**               ``BIGINT``            Flow through the edge in the direction (source, target).
**residual_capacity**  ``BIGINT``            Residual capacity of the edge in the direction (source, target).
=====================  ====================  =================================================

.. result_end

See Also
--------

* :ref:`maxFlow`
* http://www.boost.org/libs/graph/doc/push_relabel_max_flow.html 
* https://en.wikipedia.org/wiki/Push%E2%80%93relabel_maximum_flow_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

