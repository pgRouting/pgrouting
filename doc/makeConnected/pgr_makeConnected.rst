..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_makeConnected - Experimental
===============================================================================

``pgr_makeConnected`` — Returns the set of node_from and node_to of edges. This set of edges are those edges
which will make the graph connected. In particular, the boost::make_connected( ) is implemented by Boost.Graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/make_connected.html

   Boost Graph Inside

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability


Description
-------------------------------------------------------------------------------

Adds the minimum number of edges needed to make the input graph connected. The algorithm first identifies
all of the connected components in the graph, then adds edges to connect those components together in a path.
For example, if a graph contains three connected components A, B, and C, make_connected will add two edges.
The two edges added might consist of one connecting a vertex in A with a vertex in B and one connecting a vertex in B with a vertex in C.

The main characteristics are:
  - It will give the minimum list of all edges which are needed in the graph to make the graph connected.

  - Applicable only for undirected graphs.

  - The graph can be either weighted or unweighted.

  - **Running time:** On a graph with n vertices and m edges, make_connected runs in time O(n + m).


Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_makeConnected(Edges SQL)

    RETURNS SET OF (seq, node_from, node_to)
    OR EMPTY SET

Using pgr_connectedComponents() to check the number of connected components in the :doc:`sampledata` graph.
There are 3 connected components namely **1**, **14**, **16**.

.. literalinclude:: doc-pgr_makeConnected.queries
   :start-after: -- q1
   :end-before: -- q2

:Example: Query done on :doc:`sampledata` network gives the list of edges that are needed in the graph to make it connected.

.. literalinclude:: doc-pgr_makeConnected.queries
   :start-after: -- q2
   :end-before: -- q3

Inserting the above list of edges in the edge_table.

.. literalinclude:: doc-pgr_makeConnected.queries
   :start-after: -- q3
   :end-before: -- q4

Using pgr_connectedComponents() to check the number of connected components. So, Now the graph has
only one connected component.

.. literalinclude:: doc-pgr_makeConnected.queries
      :start-after: -- q4
      :end-before: -- q5

pgr_makeConnected() can also be used to verify it. Since, the graph is already connected now, it will
return an empty set of row.

.. literalinclude:: doc-pgr_makeConnected.queries
      :start-after: -- q5
      :end-before: -- q6

Parameters
-------------------------------------------------------------------------------

=================== ====================== ========= =================================================
Parameter           Type                   Default   Description
=================== ====================== ========= =================================================
**Edges SQL**       ``TEXT``                         SQL query as described above.
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
**cost**          ``ANY-NUMERICAL``              Weight of the edge  `(source, target)`

                                                 - When negative: edge `(source, target)` does not exist, therefore it's not part of the graph.

**reverse_cost**  ``ANY-NUMERICAL``       -1     Weight of the edge `(target, source)`,

                                                 - When negative: edge `(target, source)` does not exist, therefore it's not part of the graph.

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
