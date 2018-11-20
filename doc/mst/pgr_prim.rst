..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_prim - Experimental
===============================================================================

``pgr_prim`` — Returns the minimum spanning tree of graph using Prim algorithm.
In particular, the Prim algorithm implemented by Boost.Graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/doc/libs/1_64_0/libs/graph/doc/prim_minimum_spanning_tree.html

   Boost Graph Inside

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Experimental:

  * v3.0.0

Description
-------------------------------------------------------------------------------

The prim algorithm was developed in 1930 by Czech mathematician Vojtěch Jarník.
It is a greedy algorithm that finds a minimum spanning tree for a weighted
undirected graph. This means it finds a subset of the edges that forms a tree
that includes every vertex, where the total weight of all the edges in the tree
is minimized. The algorithm operates by building this tree one vertex at a time,
from an arbitrary starting vertex, at each step adding the cheapest possible
connection from the tree to another vertex.

This algorithms find the minimum spanning forest in a possibly disconnected
graph; in contrast, the most basic form of Prim's algorithm only finds minimum
spanning trees in connected graphs. However, running Prim's algorithm separately
for each connected component of the graph, then it is called minimum spanning
forest.

**The main characteristics are:**

- It's implementation is only on **undirected graph**.
- Process is done only on edges with positive costs.
- Span start from chosen root_vertex resulting subgraph.

  - When root vertex is not given then result is minimun spanning tree of disconnected graph.

- Values are returned when there is a minimum spanning tree.

  - When there is no edge in graph then EMPTY SET is return.

- The minimum spanning tree is same for any root vertex.
- Running time: :math:`O(E*log V)`

.. Note: From boost Graph:
   "The algorithm as implemented in Boost.Graph does not produce correct results on graphs with parallel edges."



Signatures
-------------------------------------------------------------------------------

.. index::
    single: prim - Experimental

.. code-block:: none

    pgr_prim(edges_sql)
    pgr_prim(edges_sql, root_vertex)

    RETURNS SET OF (seq, root_vertex, node, edge, cost, agg_cost, tree_cost)
       or EMPTY SET

The signature is for a **undirected** graph.

.. rubric:: pgr_prim without root vertex

:Example:

.. code-block:: none

    pgr_prim(TEXT edges_sql) - Experimental
    RETURNS SET OF (seq, root_vertex, node, edge, cost, agg_cost, tree_cost) or EMPTY SET

When root vertex is not given then result is MST of disconnected graph.

.. literalinclude:: doc-pgr_prim.queries
   :start-after: -- q1
   :end-before: -- q2

.. rubric:: pgr_prim with root vertex

:Example:

.. code-block:: none

    pgr_prim(TEXT edges_sql, BIGINT root_vertex);
    RETURNS SET OF (seq, root_vertex, node, edge, cost, agg_cost, tree_cost) or EMPTY SET

Results is subgraph which contain root_vertex.

.. literalinclude:: doc-pgr_prim.queries
   :start-after: -- q2
   :end-before: -- q3

.. literalinclude:: doc-pgr_prim.queries
   :start-after: -- q3
   :end-before: -- q4

Parameters
-------------------------------------------------------------------------------

=================== ====================== ========= =================================================
Parameter           Type                   Default   Description
=================== ====================== ========= =================================================
**edges_sql**       ``TEXT``                         SQL query as described above.
**root_vertex**     ``BIGINT``                       Root vertex from where spanning of tree start and
                                                     resulting in subgraph.

                                                     - In the absence of root vertex the result is
                                                       MST of disconnected graph.
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

.. rubric:: For prim algorithms

Returns set of ``(seq, root_vertex, node, edge, cost, agg_cost, tree_cost)``

===============  =========== ====================================================
Column           Type        Description
===============  =========== ====================================================
**seq**          ``BIGINT``  Sequential value starting from **1**.
**root_vertex**  ``BIGINT``  Root vertex from where spanning of tree start.
**node**         ``BIGINT``  Identifier of the node in spanning tree.
**edge**         ``BIGINT``  Identifier of the edge used to go to node.
**cost**         ``FLOAT``   Cost to traverse of edge to the node.
**agg_cost**     ``FLOAT``   Aggregate cost from node to root_vertex.
**tree_cost**    ``FLOAT``   Aggregate cost of edges that is covered in spanning.
===============  =========== ====================================================

See Also
-------------------------------------------------------------------------------

* https://en.wikipedia.org/wiki/Prim%27s_algorithm
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

