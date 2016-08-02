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

.. _pgr_maxFlowBoykovKolmogorov:

pgr_maxFlowBoykovKolmogorov
===========================


Name
----

``pgr_maxFlowBoykovKolmogorov`` — Calculates the maximum flow in a directed graph given a source and a destination. Implemented by Boost Graph Library.


.. warning::  These are proposed functions.

    - Are not officially in the release.
    - Names could change.
    - Signatures could change.
    - Needs testing.
    - Functionality could change.
    - Documentation could be incomplete.


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
  - The graph must be directed.
  - Calculates the flow/residual capacity for each edge. In the output, edges with zero flow are omitted.
  - The maximum flow through the graph can be calculated by aggregation on source/sink.
  - Returns nothing if source and sink are the same.
  - Allows multiple sources and sinks (See signatures below).
  - Running time: in general polynomial complexity, performs well on graphs that represent 2D grids (eg.: roads).

Signature Summary
-----------------

..
   If the function has more than one signature
   Remove the unnecessary parts of the signature, just leaving the name of the parameters
   Like in these examples

.. code-block:: none

    pgr_maxflowboykovkolmogorov(edges_sql, source\_vertex,  sink\_vertex)
    pgr_maxflowboykovkolmogorov(edges_sql, source\_vertices,  sink\_vertex)
    pgr_maxflowboykovkolmogorov(edges_sql, source\_vertex,  sink\_vertices)
    pgr_maxflowboykovkolmogorov(edges_sql, source\_vertices,  sink\_vertices)
    RETURNS SET OF (id, edge_id, source, target, flow, residual_capacity)
      OR EMPTY SET


Signatures
==========

Boykov-Kolmogorov One to One
----------------------------

The available signature calculates the maximum flow from one source vertex to one sink vertex.

.. code-block:: none

    pgr_maxflowboykovkolmogorov(edges_sql, source\_vertex,  sink\_vertex)
    RETURNS SET OF (id, edge_id, source, target, flow, residual_capacity)
      OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_maxFlowBoykovKolmogorov.queries
   :start-after: -- q1
   :end-before: -- q2

Boykov-Kolmogorov One to Many
-----------------------------

The available signature calculates the maximum flow from one source vertex to many sink vertices.

.. code-block:: none

    pgr_maxflowboykovkolmogorov(edges_sql, source\_vertex,  sink\_vertices)
    RETURNS SET OF (id, edge_id, source, target, flow, residual_capacity)
      OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_maxFlowBoykovKolmogorov.queries
   :start-after: -- q2
   :end-before: -- q3

Boykov-Kolmogorov Many to One
-----------------------------

The available signature calculates the maximum flow from many source vertices to one sink vertex.

.. code-block:: none

    pgr_maxflowboykovkolmogorov(edges_sql, source\_vertices,  sink\_vertex)
    RETURNS SET OF (id, edge_id, source, target, flow, residual_capacity)
      OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_maxFlowBoykovKolmogorov.queries
   :start-after: -- q3
   :end-before: -- q4

Boykov-Kolmogorov Many to Many
------------------------------

The available signature calculates the maximum flow from many sources to many sinks.

.. code-block:: none

    pgr_maxflowboykovkolmogorov(edges_sql, source\_vertices,  sink\_vertices)
    RETURNS SET OF (id, edge_id, source, target, flow, residual_capacity)
      OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_maxFlowBoykovKolmogorov.queries
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

See Also
--------

* http://www.boost.org/libs/graph/doc/boykov_kolmogorov_max_flow.html
* http://www.csd.uwo.ca/~yuri/Papers/pami04.pdf

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

