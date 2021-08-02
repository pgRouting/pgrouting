..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/coloring-family.html>`__
  (`3.2 <https://docs.pgrouting.org/3.2/en/coloring-family.html>`__)


Coloring - Family of functions (Experimental)
===============================================================================

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. index from here

* :doc:`pgr_sequentialVertexColoring` - Vertex coloring algorithm using greedy approach.
* :doc:`pgr_bipartite` - Bipartite graph algorithm using a DFS-based coloring approach.
* :doc:`pgr_edgeColoring` - Edge Coloring algorithm using Vizing's theorem.

.. index to here


.. toctree::
    :hidden:

    pgr_sequentialVertexColoring
    pgr_bipartite
    pgr_edgeColoring


Parameters
-------------------------------------------------------------------------------

.. parameters start

=================== ====================== =================================================
Parameter           Type                   Description
=================== ====================== =================================================
**Edges SQL**       ``TEXT``               Inner query as described below.
=================== ====================== =================================================

.. parameters end

Inner query
-------------------------------------------------------------------------------

:Edges SQL: an SQL query of an **undirected** graph, which should return
            a set of rows with the following columns:

.. include:: traversal-family.rst
   :start-after: edges_sql_start
   :end-before: edges_sql_end


Result Columns
-------------------------------------------------------------------------------

.. result columns start

Returns SET OF ``(vertex_id, color_id)``

===============  =========== ====================================================
Column           Type        Description
===============  =========== ====================================================
**vertex_id**    ``BIGINT``  Identifier of the vertex.
**color_id**     ``BIGINT``  Identifier of the color of the vertex.

                             - The minimum value of color is 1.

===============  =========== ====================================================

.. result columns end

.. result columns start edgeColoring

Returns SET OF ``(edge_id, color_id)``

===============  =========== ====================================================
Column           Type        Description
===============  =========== ====================================================
**edge_id**      ``BIGINT``  Identifier of the edge.
**color_id**     ``BIGINT``  Identifier of the color of the edge.

                             - The minimum value of color is 1.

===============  =========== ====================================================

.. result columns end edgeColoring




See Also
-------------------------------------------------------------------------------

.. include:: pgr_sequentialVertexColoring.rst
    :start-after: see also start
    :end-before: see also end



.. include:: pgr_bipartite.rst
    :start-after: see also start
    :end-before: see also end



.. include:: pgr_edgeColoring.rst
    :start-after: see also start
    :end-before: see also end


.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
