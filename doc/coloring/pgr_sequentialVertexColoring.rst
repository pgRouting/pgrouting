..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: Coloring Family ; pgr_sequentialVertexColoring - Proposed
   single: sequentialVertexColoring - Proposed on v3.3

|

``pgr_sequentialVertexColoring`` - Proposed
===============================================================================

``pgr_sequentialVertexColoring`` — Returns the vertex coloring of an undirected
graph, using greedy approach.

.. include:: proposed.rst
   :start-after: warning-begin
   :end-before: end-warning

.. rubric:: Availability

.. rubric:: Version 4.0.0

* Output columns standardized to |result_node_color|

.. rubric:: Version 3.3.0

* Function promoted to proposed.

.. rubric:: Version 3.2.0

* New experimental function.

Description
-------------------------------------------------------------------------------

Sequential vertex coloring algorithm is a graph coloring algorithm in which
color identifiers are assigned to the vertices of a graph in a sequential
manner, such that no edge connects two identically colored vertices.

**The main Characteristics are:**

- The implementation is applicable only for **undirected** graphs.
- Provides the color to be assigned to all the vertices present in the graph.
- Color identifiers values are in the Range :math:`[1, |V|]`
- The algorithm tries to assign the least possible color to every vertex.
- Efficient graph coloring is an NP-Hard problem, and therefore, this algorithm
  does not always produce optimal coloring. It follows a greedy strategy by
  iterating through all the vertices sequentially, and assigning the smallest
  possible color that is not used by its neighbors, to each vertex.
- The returned rows are ordered in ascending order of the vertex value.
- Sequential Vertex Coloring Running Time: :math:`O(|V|*(d + k))`

  - where :math:`|V|` is the number of vertices,
  - :math:`d` is the maximum degree of the vertices in the graph,
  - :math:`k` is the number of colors used.

|Boost| Boost Graph Inside

Signatures
------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_sequentialVertexColoring(`Edges SQL`_)

   | Returns set of |result_node_color|
   | OR EMPTY SET

:Example: Graph coloring of pgRouting :doc:`sampledata`

.. literalinclude:: sequentialVertexColoring.queries
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

Result columns
-------------------------------------------------------------------------------

.. include:: coloring-family.rst
    :start-after: result-node-color-start
    :end-before: result-node-color-end


See Also
-------------------------------------------------------------------------------

* :doc:`sampledata`
* `Boost: Sequential Vertex Coloring
  <https://www.boost.org/libs/graph/doc/sequential_vertex_coloring.html>`__
* `Wikipedia: Graph coloring <https://en.wikipedia.org/wiki/Graph_coloring>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
