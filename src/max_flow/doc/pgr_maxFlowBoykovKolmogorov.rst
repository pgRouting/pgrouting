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

.. _pgr_maxFlowBoykovKolmogorov:

pgr_maxFlowBoykovKolmogorov - Proposed
======================================================


Name
----

``pgr_maxFlowBoykovKolmogorov`` — Calculates the maximum flow in a directed graph given a source and a destination. Implemented by Boost Graph Library.



.. include:: ../../proposedNext.rst
   :start-after: begin-warning
   :end-before: end-warning



.. figure:: ../../../doc/src/introduction/images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/boykov_kolmogorov_max_flow.html

   Boost Graph Inside


Synopsis
-------------------------------------------------------------------------------

Calculates the maximum flow in a directed graph from a source node to a sink node.
Edges must be weighted with non-negative capacities.
Developed by Boykov and Kolmogorov.


Characteristics:
----------------

The main characterics are:
  - The graph must be **directed**.
  - Calculates the flow/residual capacity for each edge. In the output, edges with zero flow are omitted.
  - The maximum flow through the graph can be calculated by aggregation on source/sink.
  - Returns nothing if source and sink are the same.
  - Allows multiple sources and sinks (See signatures below).
  - Running time: in general polynomial complexity, performs well on graphs that represent 2D grids (eg.: roads).

Signature Summary
-----------------

.. code-block:: none

    pgr_maxFlowBoykovKolmogorov(edges_sql, start_vid,  end_vid)
    pgr_maxFlowBoykovKolmogorov(edges_sql, start_vids, end_vid)
    pgr_maxFlowBoykovKolmogorov(edges_sql, start_vid,  end_vids)
    pgr_maxFlowBoykovKolmogorov(edges_sql, start_vids, end_vids)
    RETURNS SET OF (id, edge_id, start_vid, end_vid, flow, residual_capacity)
      OR EMPTY SET


Signatures
----------------------------

.. index::
    single: maxFlowBoykovKolmogorov(One to One) - Proposed

One to One
..............................................

The available signature calculates the maximum flow from one source vertex to one sink vertex.

.. code-block:: none

    pgr_maxFlowBoykovKolmogorov(edges_sql, start_vid, end_vid)
    RETURNS SET OF (id, edge_id, start_vid, end_vid, flow, residual_capacity)
      OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_maxFlowBoykovKolmogorov.queries
   :start-after: -- q1
   :end-before: -- q2


.. index::
    single: maxFlowBoykovKolmogorov(One to Many) - Proposed

One to Many
..............................................

The available signature calculates the maximum flow from one source vertex to many sink vertices.

.. code-block:: none

    pgr_maxFlowBoykovKolmogorov(edges_sql, start_vid,  end_vids)
    RETURNS SET OF (id, edge_id, start_vid, end_vid, flow, residual_capacity)
    OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_maxFlowBoykovKolmogorov.queries
   :start-after: -- q2
   :end-before: -- q3


.. index::
    single: maxFlowBoykovKolmogorov(Many to One) - Proposed

Many to One
..............................................

The available signature calculates the maximum flow from many source vertices to one sink vertex.

.. code-block:: none

    pgr_maxFlowBoykovKolmogorov(edges_sql, start_vids,  end_vid)
    RETURNS SET OF (id, edge_id, start_vid, end_vid, flow, residual_capacity)
    OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_maxFlowBoykovKolmogorov.queries
   :start-after: -- q3
   :end-before: -- q4


.. index::
    single: maxFlowBoykovKolmogorov(Many to Many) - Proposed

Many to Many
..............................................

The available signature calculates the maximum flow from many sources to many sinks.

.. code-block:: none

    pgr_maxFlowBoykovKolmogorov(edges_sql, start_vids,  end_vids)
    RETURNS SET OF (id, edge_id, start_vid, end_vid, flow, residual_capacity)
    OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_maxFlowBoykovKolmogorov.queries
   :start-after: -- q4
   :end-before: -- q5




Description of the Signatures
--------------------------------------------------------


.. include:: ../../../doc/src/tutorial/custom_query.rst
    :start-after: flow_edges_sql_start
    :end-before: flow_edges_sql_end

.. include:: ../sql/max_flow.sql
    :start-after: pgr_flow_parameters_start
    :end-before: pgr_flow_parameters_end

.. include:: ./pgr_maxFlowPushRelabel.rst
    :start-after: result_start
    :end-before: result_end


See Also
--------

* http://www.boost.org/libs/graph/doc/boykov_kolmogorov_max_flow.html
* http://www.csd.uwo.ca/~yuri/Papers/pami04.pdf

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

