..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index:: Coloring Family

|

Coloring - Family of functions
===============================================================================

.. include:: proposed.rst
   :start-after: warning-begin
   :end-before: end-warning

.. proposed-start

* :doc:`pgr_sequentialVertexColoring` - Vertex coloring algorithm using greedy
  approach.

.. proposed-end

.. include:: experimental.rst
   :start-after: warning-begin
   :end-before: end-warning

.. official-start

* :doc:`pgr_bipartite` - Bipartite graph algorithm using a DFS-based coloring
  approach.
* :doc:`pgr_edgeColoring` - Edge Coloring algorithm using Vizing's theorem.

.. official-end


.. toctree::
    :hidden:

    pgr_sequentialVertexColoring
    pgr_bipartite
    pgr_edgeColoring


Result columns
-------------------------------------------------------------------------------

.. result columns start

Returns set of ``(vertex_id, color_id)``

===============  =========== ======================================
Column           Type        Description
===============  =========== ======================================
``vertex_id``    ``BIGINT``  Identifier of the vertex.
``color_id``     ``BIGINT``  Identifier of the color of the vertex.

                             - The minimum value of color is 1.

===============  =========== ======================================

.. result columns end

.. result columns start edgeColoring

Returns set of ``(edge_id, color_id)``

===============  =========== =====================================
Column           Type        Description
===============  =========== =====================================
``edge_id``      ``BIGINT``  Identifier of the edge.
``color_id``     ``BIGINT``  Identifier of the color of the edge.

                             - The minimum value of color is 1.

===============  =========== =====================================

.. result columns end edgeColoring

See Also
-------------------------------------------------------------------------------

* `Boost: <https://www.boost.org/libs/graph/doc/table_of_contents.html>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
