..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.1 License: http://creativecommons.org/licenses/by-sa/3.1/
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

* Version 3.1.0

  * New **experimental** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/dev/en/pgr_makeConnected.html>`__) and above

* **TBD**

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

    pgr_makeConnected(edges_sql)

    RETURNS SET OF (seq, source, target, cost)
    OR EMPTY SET

:Example: Query done on :doc:`sampledata` network gives.

.. literalinclude:: doc-pgr_makeConnected.queries
   :start-after: -- q1
   :end-before: -- q2

Parameters
-------------------------------------------------------------------------------

=================== ====================== ========= =================================================
Parameter           Type                   Default   Description
=================== ====================== ========= =================================================
**edges_sql**       ``TEXT``                         SQL query as described above.
=================== ====================== ========= =================================================

Inner query
-------------------------------------------------------------------------------

:edges_sql: an SQL query, which should return a set of rows with the following columns:

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

Returns set of ``(seq, source, target, cost)``

===============  =========== ============================================================
Column           Type        Description
===============  =========== ============================================================
**seq**          ``INT``     Sequential value starting from **1**.
**source**       ``BIGINT``  Identifier of the first end point vertex of the edge.
**target**       ``BIGINT``  Identifier of the second end point vertex of the edge.
**cost**         ``FLOAT``   Weight of the edge  `(source, target)`

                             - When negative: edge `(source, target)` does not exist, therefore it's not part of the graph.
===============  =========== ============================================================

Additional Example:
-------------------------------------------------------------------------------


Now, let's add some edges to make the graph non-planar. We will be adding edges between every pair in this list of vertices **1**, **2**, **3**, **4**, **5**.


.. literalinclude:: doc-pgr_makeConnected.queries
   :start-after: -- q2
   :end-before: -- q3


Now, let's check our graph is planar or not. If it is non-planar then it will return an empty set of rows.


.. literalinclude:: doc-pgr_makeConnected.queries
   :start-after: -- q3
   :end-before: -- q4

There can be some cases where we only want to check whether a particular connected component of a graph is planar or not. So the below example
will illustrate the way to do it.

  - Use pgr_connectedComponents( ) function in query:


.. literalinclude:: doc-pgr_makeConnected.queries
   :start-after: -- q4
   :end-before: -- q5

See Also
-------------------------------------------------------------------------------

* https://en.wikipedia.org/wiki/Planarity_testing
* https://www.boost.org/libs/graph/doc/boyer_myrvold.html
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
