..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_edgeColoring.html>`__
  (`3.3 <https://docs.pgrouting.org/3.3/en/pgr_edgeColoring.html>`__)

pgr_edgeColoring - Experimental
===============================================================================

``pgr_edgeColoring`` — Returns the edge coloring of an undirected and loop-free (i.e *no self-loops and no parallel edges*) graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/edge_coloring.html

   Boost Graph Inside

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-exp

.. rubric:: Availability

* Version 3.3.0

  * New **experimental** function


Description
-------------------------------------------------------------------------------

Edge Coloring is an algorithm used for coloring of the edges for the vertices in the graph. It
is an assignment of colors to the edges of the graph so that no two adjacent edges have the same
color.

**The main Characteristics are:**

- The implementation is applicable only for **undirected** and **loop-free** (i.e *no self-loops and no parallel edges*) graphs.
- Provides the color to be assigned to all the edges present in the graph.
- At most **Δ + 1** colors are used, where **Δ** is the degree of the graph. This is optimal for some graphs, and by Vizing's theorem it uses at most one color more than the optimal for all others.
- It can tell us whether a graph is **Bipartite**. If in a graph, the chromatic number **χ′(G)** i.e. minimum number of colors needed for proper edge coloring of graph is equal to degree **Δ** of the graph, (i.e. **χ′(G) = Δ**) then graph is said to be Bipartite. But, the vice-versa is not always true.
- The algorithm tries to assign the least possible color to every edge.
- Efficient graph coloring is an NP-Hard problem, and therefore, this algorithm
  does not always produce optimal coloring.
- The returned rows are ordered in ascending order of the edge value.
- This algorithm is the fastest known almost-optimal algorithm for edge coloring.
- Edge Coloring Running Time: :math:`O(|E||V|)`

  - where :math:`|E|` is the number of edges in the graph,
  - :math:`|V|` is the number of vertices in the graph.

Signatures
------------------------------------------------------------------------------

.. code-block:: sql

    pgr_edgeColoring(Edges SQL) -- Experimental on v3.3

    RETURNS SET OF (edge_id, color_id)
    OR EMPTY SET

:Example: Graph coloring of pgRouting :doc:`sampledata`

.. literalinclude:: doc-edgeColoring.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: edgeColoring -- Experimental on v3.3

.. Parameters, Inner query & result columns

Parameters
-------------------------------------------------------------------------------

.. include:: coloring-family.rst
    :start-after: parameters start
    :end-before: parameters end

Inner query
-------------------------------------------------------------------------------

:Edges SQL: an SQL query of an **undirected** graph, which should return
            a set of rows with the following columns:

.. include:: traversal-family.rst
   :start-after: edges_sql_start
   :end-before: edges_sql_end

Result Columns
-------------------------------------------------------------------------------

.. include:: coloring-family.rst
    :start-after: result columns start edgeColoring
    :end-before: result columns end edgeColoring


See Also
-------------------------------------------------------------------------------

* The queries use the :doc:`sampledata` network.

.. see also start

* `Boost: Edge Coloring Algorithm documentation <https://www.boost.org/libs/graph/doc/edge_coloring.html>`__
* `Wikipedia: Graph Coloring <https://en.wikipedia.org/wiki/Graph_coloring>`__

.. see also end

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
