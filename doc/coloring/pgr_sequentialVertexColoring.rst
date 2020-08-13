..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_sequentialVertexColoring - Experimental
===============================================================================

``pgr_sequentialVertexColoring`` — Returns the vertex coloring of an undirected graph,
using greedy approach.

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
  current(`3.2 <https://docs.pgrouting.org/3.2/en/pgr_sequentialVertexColoring.html>`__)

Description
-------------------------------------------------------------------------------

Sequential Vertex Coloring algorithm is a graph coloring algorithm in which color
identifiers are assigned to the vertices of a graph in a sequential manner,
such that no edge connects two identically colored vertices.

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

Signatures
------------------------------------------------------------------------------

.. code-block:: none

    pgr_sequentialVertexColoring(Edges SQL) -- Experimental on v3.2

    RETURNS SET OF (vertex_id, color_id)
    OR EMPTY SET

:Example: Graph coloring of pgRouting :doc:`sampledata`

.. literalinclude:: doc-pgr_sequentialVertexColoring.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: sequentialVertexColoring -- Experimental on v3.2

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
    :start-after: result columns start
    :end-before: result columns end


See Also
-------------------------------------------------------------------------------

* The queries use the :doc:`sampledata` network.

.. see also start

* `Boost: Sequential Vertex Coloring algorithm documentation <https://www.boost.org/libs/graph/doc/sequential_vertex_coloring.html>`__
* `Wikipedia: Graph coloring <https://en.wikipedia.org/wiki/Graph_coloring>`__

.. see also end

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

