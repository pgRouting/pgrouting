..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************


.. _pgr_maxFlowPushRelabel:

pgr_maxFlowPushRelabel Proposed
============================================


Name
----

``pgr_maxFlowPushRelabel`` — Calculates the maximum flow in a directed graph for the given source(s) and targets(s).


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
  - Process is done only on edges with positive capacities.
  - When the maximum flow is 0 then there is no flow and **EMPTY SET** is returned.

    - There is no flow when a **source** is the same as a **target**.

  - Any duplicated value in the source(s) or target(s) are ignored.
  - Calculates the flow/residual capacity for each edge. In the output, edges with zero flow are omitted.
  - Creates a **super source** and edges to all the source(s), and a **super target** and the edges from all the targets(s).
  - The maximum flow through the graph can be calculated by aggregation on source/sink which is garanteed to be the value returned by :ref:`pgr_maxFlow <pgr_maxFlow>` when executed with the same parameters.
  - Running time: :math:`O( V ^ 3)`

Signature Summary
-----------------

.. code-block:: none

    pgr_maxFlowPushRelabel(edges_sql, source,  target)
    pgr_maxFlowPushRelabel(edges_sql, sources, target)
    pgr_maxFlowPushRelabel(edges_sql, source,  targets)
    pgr_maxFlowPushRelabel(edges_sql, sources, targets)
    RETURNS SET OF (seq, edge, start_vid, end_vid, flow, residual_capacity)
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
    RETURNS SET OF (seq, edge, start_vid, end_vid, flow, residual_capacity)
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
    RETURNS SET OF (seq, edge, start_vid, end_vid, flow, residual_capacity)
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
    RETURNS SET OF (seq, edge, start_vid, end_vid, flow, residual_capacity)
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
    RETURNS SET OF (seq, edge, start_vid, end_vid, flow, residual_capacity)
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


.. include:: ../../../doc/src/tutorial/custom_query.rst
    :start-after: result_flow_start
    :end-before: result_flow_end


See Also
--------

* :ref:`maxFlow`
* http://www.boost.org/libs/graph/doc/push_relabel_max_flow.html 
* https://en.wikipedia.org/wiki/Push%E2%80%93relabel_maximum_flow_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

