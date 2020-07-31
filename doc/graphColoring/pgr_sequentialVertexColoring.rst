..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_sequentialVertexColoring - Experimental
===============================================================================

``pgr_sequentialVertexColoring`` — Assigns labels to the vertices of graph
such that no edge connects two identically labeled vertices. In particular,
the Sequential Vertex Coloring algorithm implemented by Boost.Graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/sequential_vertex_coloring.html

   Boost Graph Inside

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-exp

.. rubric:: Availability

* Version 3.2.0

  * New **experimental** function

.. rubric:: Support

* **Supported versions:**
  current(`3.2 <https://docs.pgrouting.org/3.2/en/pgr_depthFirstSearch.html>`__)

Description
-------------------------------------------------------------------------------

Sequential Vertex Coloring algorithm is a graph coloring or labeling algorithm
in which colors or labels are assigned to the vertices of a graph sequentially,
such that no edge connects two identically labeled or colored vertices.

Efficient graph coloring is an NP-Hard problem, and therefore, the Sequential
Vertex Coloring algorithm does not always produce optimal coloring. This algorithm
follows a greedy strategy by iterating through all the vertices sequentially,
and assigning the smallest possible color that is not used by its neighbors, to
each vertex.

**The main Characteristics are:**

- The implementation is applicable only for **undirected** graphs.
- Provides the color to be assigned to all the nodes present in the graph.
- The maximum value of color will not be greater than the number of vertices
  in the graph.
- Higher color values are used only if required, i.e. the algorithm tries to
  assign the least possible color to every vertex.
- The returned rows are ordered in ascending order of the node value.
- Sequential Vertex Coloring Running Time: :math:`O(|V|*(|d| + |k|))`, where
  :math:`|V|` is the number of vertices, :math:`|d|` is the maximum degree of
  the vertices in the graph, and :math:`|k|` is the number of colors used.

Signatures
------------------------------------------------------------------------------

.. code-block:: none

    pgr_sequentialVertexColoring(Edges SQL)

    RETURNS SET OF (node, color)
    OR EMPTY SET

:Example: Graph coloring of pgRouting :doc:`sampledata`

.. literalinclude:: doc-pgr_sequentialVertexColoring.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: sequentialVertexColoring - Experimental

.. Parameters, Inner query & result columns

.. sequentialVertexColoring-information-start

Parameters
-------------------------------------------------------------------------------

=================== ====================== =================================================
Parameter           Type                   Description
=================== ====================== =================================================
**Edges SQL**       ``TEXT``               SQL query described in `Inner query`_.
=================== ====================== =================================================

Inner query
-------------------------------------------------------------------------------

.. rubric:: Edges SQL

.. include:: pgRouting-concepts.rst
   :start-after: basic_edges_sql_start
   :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

.. result columns start

Returns SET OF ``(node, color)``

===============  =========== ====================================================
Column           Type        Description
===============  =========== ====================================================
**node**         ``BIGINT``  Identifier of all the ``nodes`` in the graph.
**color**        ``BIGINT``  Identifier of the ``color`` of the ``node``.

                             - The maximum value will not be greater than the
                               number of vertices in the graph


===============  =========== ====================================================

.. result columns end


See Also
-------------------------------------------------------------------------------

* The queries use the :doc:`sampledata` network.
* `Boost: Sequential Vertex Coloring algorithm documentation <https://www.boost.org/libs/graph/doc/sequential_vertex_coloring.html>`__
* `Wikipedia: Graph coloring <https://en.wikipedia.org/wiki/Graph_coloring>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

