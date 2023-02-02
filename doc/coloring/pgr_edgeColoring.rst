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
  (`3.5 <https://docs.pgrouting.org/3.5/en/pgr_edgeColoring.html>`__)
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_edgeColoring.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_edgeColoring.html>`__

pgr_edgeColoring - Experimental
===============================================================================

``pgr_edgeColoring`` — Returns the edge coloring of undirected and loop-free
graphs

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/edge_coloring.html

   Boost Graph Inside

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-exp

.. rubric:: Availability

* Version 3.3.0

  * New **experimental** signature


Description
-------------------------------------------------------------------------------

Edge Coloring is an algorithm used for coloring of the edges for the vertices
in the graph. It is an assignment of colors to the edges of the graph so that
no two adjacent edges have the same color.

**The main Characteristics are:**

- The implementation is for **undirected** and **loop-free** graphs

  - :loop free: no self-loops and no parallel edges.

- Provides the color to be assigned to all the edges present in the graph.

- At most :math:`\Delta + 1` colors are used, where :math:`\Delta` is the degree
  of the graph.

  - This is optimal for some graphs, and by Vizing's theorem it uses at most one
    color more than the optimal for all others.
  - When the graph is bipartite

    - the chromatic number :math:`x'(G)` (minimum number of
      colors needed for proper edge coloring of graph)  is equal to the degree
      :math:`\Delta + 1` of the graph, (:math:`x'(G) = \Delta`)

- The algorithm tries to assign the least possible color to every edge.

  - Does not always produce optimal coloring.

- The returned rows are ordered in ascending order of the edge identifier.
- Efficient graph coloring is an NP-Hard problem, and therefore:

  - In this implelentation the running time: :math:`O(|E|*|V|)`

    - where :math:`|E|` is the number of edges in the graph,
    - :math:`|V|` is the number of vertices in the graph.

Signatures
------------------------------------------------------------------------------

.. index::
    single: edgeColoring - Experimental on v3.3

.. admonition:: \ \
   :class: signatures

   | pgr_edgeColoring(`Edges SQL`_)

   | RETURNS SET OF |result-edge-color|
   | OR EMPTY SET

:Example: Graph coloring of pgRouting :doc:`sampledata`

.. literalinclude:: doc-edgeColoring.queries
   :start-after: -- q1
   :end-before: -- q2

.. Parameters, Inner Queries & result columns

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

.. include:: coloring-family.rst
    :start-after: result columns start edgeColoring
    :end-before: result columns end edgeColoring


See Also
-------------------------------------------------------------------------------

* The queries use the :doc:`sampledata` network.

.. see also start

* `Boost: Edge Coloring Algorithm documentation
  <https://www.boost.org/libs/graph/doc/edge_coloring.html>`__
* `Wikipedia: Graph Coloring
  <https://en.wikipedia.org/wiki/Graph_coloring>`__

.. see also end

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
