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


.. include:: proposedNext.rst
   :start-after: begin-warning
   :end-before: end-warning


.. figure:: images/boost-inside.jpeg
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

    pgr_maxFlowEdmondsKarp(edges_sql, source,  target)
    pgr_maxFlowEdmondsKarp(edges_sql, sources, target)
    pgr_maxFlowEdmondsKarp(edges_sql, source,  targets)
    pgr_maxFlowEdmondsKarp(edges_sql, sources, targets)
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

    pgr_maxFlowEdmondsKarp(edges_sql, source,  target)
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

    pgr_maxFlowEdmondsKarp(edges_sql, source,  targets)
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

    pgr_maxFlowEdmondsKarp(edges_sql, sources,  target)
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

    pgr_maxFlowEdmondsKarp(edges_sql, sources,  targets)
    RETURNS SET OF (id, edge_id, source, target, flow, residual_capacity)
      OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_maxFlowEdmondsKarp.queries
   :start-after: -- q4
   :end-before: -- q5



Description of the Signatures
-----------------------------------------------


.. include:: custom_query.rst
    :start-after: flow_edges_sql_start
    :end-before: flow_edges_sql_end

.. include:: ../sql_queries/max_flow.sql
    :start-after: pgr_flow_parameters_start
    :end-before: pgr_flow_parameters_end

.. include:: pgr_maxFlowPushRelabel.rst
    :start-after: result_start
    :end-before: result_end


See Also
--------

* :ref:`maxFlow`
* http://www.boost.org/libs/graph/doc/edmonds_karp_max_flow.html
* https://en.wikipedia.org/wiki/Edmonds%E2%80%93Karp_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

