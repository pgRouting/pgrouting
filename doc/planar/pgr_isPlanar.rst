..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_isPlanar - Experimental
===============================================================================

``pgr_isPlanar`` — Returns a boolean depending upon the planarity of the graph. Result
is true if the input graph is planar and false if the input graph is non-planar. In
particular, the boost::boyer_myrvold_planarity_test() is implemented by Boost.Graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/boyer_myrvold.html

   Boost Graph Inside

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability


Description
-------------------------------------------------------------------------------

A graph is planar if it can be drawn in two-dimensional space with no two of its edges crossing. Such a drawing
of a planar graph is called a plane drawing. Every planar graph also admits a straight-line drawing, which is a
plane drawing where each edge is represented by a line segment.

The main characteristics are:
  - It will return a bool value depending upon the planarity of the graph.

  - Applicable only for undirected graphs.

  - The graph can be either weighted or unweighted.

  - **Running time:** Assuming that both the vertex index and edge index supplied take time O(1) to return an index
    and there are O(n) total self-loops and parallel edges in the graph, most combinations of arguments given to
    boyer_myrvold_planarity_test() result in an algorithm that runs in time O(n) for a graph with n vertices and m edges.


Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_isPlanar(Edges SQL)

    RETURNS BOOLEAN

:Example: Query done on :doc:`sampledata` network gives.

.. literalinclude:: doc-pgr_isPlanar.queries
   :start-after: -- q1
   :end-before: -- q2

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

Returns a boolean ``(pgr_isplanar)``

===============  =========== ============================================================
Column           Type        Description
===============  =========== ============================================================
**pgr_isplanar** ``BOOLEAN`` `t` if graph is planar and `f` if graph is non-planar.
===============  =========== ============================================================

Additional Example:
-------------------------------------------------------------------------------

See Also
-------------------------------------------------------------------------------

* https://www.boost.org/libs/graph/doc/boyer_myrvold.html
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
