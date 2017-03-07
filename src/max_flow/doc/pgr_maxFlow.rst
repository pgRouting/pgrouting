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


.. include:: proposedNext.rst
   :start-after: begin-warning
   :end-before: end-warning


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

    pgr_maxFlow(edges_sql, source,  target)
    pgr_maxFlow(edges_sql, sources,  target)
    pgr_maxFlow(edges_sql, source,  targets)
    pgr_maxFlow(edges_sql, sources,  targets)
    RETURNS BIGINT


Signatures
-----------------------

.. index::
    single: maxFlow(One to One) - Proposed

One to One
.....................................................................

Calculates the maximum flow from one source vertex to one sink vertex in a directed graph.

.. code-block:: none

    pgr_maxFlow(edges_sql, source,  target)
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

    pgr_maxFlow(edges_sql, source,  targets)
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

    pgr_maxFlow(edges_sql, sources,  target)
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

    pgr_maxFlow(edges_sql, sources,  targets)
    RETURNS BIGINT

:Example:

.. literalinclude:: doc-pgr_maxFlow.queries
   :start-after: -- q4
   :end-before: -- q5

Description of the Signatures
--------------------------------------------------------


.. include:: custom_query.rst
    :start-after: flow_edges_sql_start
    :end-before: flow_edges_sql_end

.. include:: ../sql_queries/max_flow.sql
    :start-after: pgr_flow_parameters_start
    :end-before: pgr_flow_parameters_end



Description of the return value
.....................................................................

====================== =================================================
Type                   Description
====================== =================================================
``BIGINT``             Maximum flow possible from the source(s) to the target(s)
====================== =================================================

See Also
--------

* :ref:`maxFlow`
* http://www.boost.org/libs/graph/doc/push_relabel_max_flow.html 
* https://en.wikipedia.org/wiki/Push%E2%80%93relabel_maximum_flow_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

