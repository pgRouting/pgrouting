..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_makeConnected - Experimental
===============================================================================

``pgr_makeConnected`` — Returns the set of edges that will make the graph connected.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/make_connected.html

   Boost Graph Inside

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 3.2.0

  * New **experimental** function

.. rubric:: Support

* **Supported versions:**
  current(`3.2 <https://docs.pgrouting.org/3.2/en/pgr_makeConnected.html>`__)

Description
-------------------------------------------------------------------------------

Adds the minimum number of edges needed to make the input graph connected. The algorithm first identifies
all of the connected components in the graph, then adds edges to connect those components together in a path.
For example, if a graph contains three connected components A, B, and C, make_connected will add two edges.
The two edges added might consist of one connecting a vertex in A with a vertex in B and one connecting a vertex in B with a vertex in C.

The main characteristics are:
  - It will give the minimum list of all edges which are needed in the graph to make the graph connected.

  - Applicable only for **undirected** graphs.

  - The algorithm does not considers traversal costs in the calculations.

  - Running time: :math:`O(V + E)`


Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_makeConnected(Edges SQL)

    RETURNS SET OF (seq, node_from, node_to)
    OR EMPTY SET

:Example: Query done on :doc:`sampledata` network gives the list of edges that are needed in the graph to make it connected.

.. literalinclude:: doc-pgr_makeConnected.queries
   :start-after: -- q1
   :end-before: -- q2

Parameters
-------------------------------------------------------------------------------

=================== ====================== ========= =================================================
Parameter           Type                   Default   Description
=================== ====================== ========= =================================================
**Edges SQL**       ``TEXT``                         SQL query as described below.
=================== ====================== ========= =================================================

Inner query
-------------------------------------------------------------------------------

:Edges SQL: an SQL query, which should return a set of rows with the following columns:

================= =================== ======== =================================================
Column            Type                 Default  Description
================= =================== ======== =================================================
**id**            ``ANY-INTEGER``                Identifier of the edge.
**source**        ``ANY-INTEGER``                Identifier of the first end point vertex of the edge.
**target**        ``ANY-INTEGER``                Identifier of the second end point vertex of the edge.
**cost**          ``ANY-NUMERICAL``              - When positive: edge `(target, source)` is part of the graph.
                                                 - When negative: edge `(target, source)` is not part of the graph.

**reverse_cost**  ``ANY-NUMERICAL``       -1     - When positive: edge `(target, source)` is part of the graph.
                                                 - When negative: edge `(target, source)` is not part of the graph.

================= =================== ======== =================================================

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, REAL, FLOAT

Result Columns
-------------------------------------------------------------------------------

Returns set of ``(seq, node_from, node_to)``

===============  =========== ============================================================
Column           Type        Description
===============  =========== ============================================================
**seq**          ``INT``     Sequential value starting from **1**.
**node_from**    ``BIGINT``  Identifier of the first end point vertex of the edge.
**node_to**      ``BIGINT``  Identifier of the second end point vertex of the edge.
===============  =========== ============================================================

See Also
-------------------------------------------------------------------------------

* https://www.boost.org/libs/graph/doc/make_connected.html
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
