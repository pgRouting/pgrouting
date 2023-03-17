..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************



pgr_stoerWagner - Experimental
===============================================================================

``pgr_stoerWagner`` — The min-cut of graph using stoerWagner algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/stoer_wagner_min_cut.html

   Boost Graph Inside

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 3.0

   * New **Experimental** function


Description
-------------------------------------------------------------------------------

In graph theory, the Stoer–Wagner algorithm is a recursive algorithm to solve
the minimum cut problem in undirected weighted graphs with non-negative weights.
The essential idea of this algorithm is to shrink the graph by merging the most
intensive vertices, until the graph only contains two combined vertex sets.
At each phase, the algorithm finds the minimum s-t cut for two vertices s and t
chosen as its will. Then the algorithm shrinks the edge between s and t to
search for non s-t cuts. The minimum cut found in all phases will be the
minimum weighted cut of the graph.

A cut is a partition of the vertices of a graph into two disjoint subsets. A
minimum cut is a cut for which the size or weight of the cut is not larger than
the size of any other cut. For an unweighted graph, the minimum cut would simply
be the cut with the least edges. For a weighted graph, the sum of all edges'
weight on the cut determines whether it is a minimum cut.

**The main characteristics are:**

- Process is done only on edges with positive costs.
- It's implementation is only on **undirected** graph.
- Sum of the weights of all edges between the two sets is mincut.

  - A **mincut** is a cut having the least weight.

- Values are returned when graph is connected.

  - When there is no edge in graph then EMPTY SET is return.
  - When the graph is unconnected then EMPTY SET is return.

- Sometimes a graph has multiple min-cuts, but all have the same weight. The
  this function determines exactly one of the min-cuts as well as its weight.

* Running time: :math:`O(V*E + V^2*log V)`.

Signatures
-------------------------------------------------------------------------------

.. index::
    single: stoerWagner - Experimental on v3.0

.. admonition:: \ \
   :class: signatures

   | pgr_stoerWagner(`Edges SQL`_)

   | RETURNS SET OF |result-mincut|
   | OR EMPTY SET

:Example: min cut of the main subgraph

.. literalinclude:: doc-stoerWagner.queries
   :start-after: -- q1
   :end-before: -- q2

Parameters
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
   :start-after: only_edge_param_start
   :end-before: only_edge_param_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

Returns set of ``(seq, edge, cost, mincut)``

===============  =========== ==================================================
Column           Type        Description
===============  =========== ==================================================
**seq**          ``INT``     Sequential value starting from **1**.
**edge**         ``BIGINT``  Edges which divides the set of vertices into two.
**cost**         ``FLOAT``   Cost to traverse of edge.
**mincut**       ``FLOAT``   Min-cut weight of a undirected graph.
===============  =========== ==================================================

Additional Example:
-------------------------------------------------------------------------------

:Example: min cut of an edge

.. literalinclude:: doc-stoerWagner.queries
   :start-after: -- q2
   :end-before: -- q3

:Example: Using :doc:`pgr_connectedComponents`

.. literalinclude:: doc-stoerWagner.queries
   :start-after: -- q3
   :end-before: -- q4

See Also
-------------------------------------------------------------------------------

* :doc:`sampledata`
* https://en.wikipedia.org/wiki/Stoer%E2%80%93Wagner_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

