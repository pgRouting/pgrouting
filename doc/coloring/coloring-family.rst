..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|



Coloring - Family of functions
===============================================================================

.. rubric:: Proposed

.. include:: proposed.rst
   :start-after: stable-begin-warning
   :end-before: stable-end-warning

.. index proposed from here

* :doc:`pgr_sequentialVertexColoring` - Vertex coloring algorithm using greedy
  approach.

.. index proposed to here

.. rubric:: Experimental

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. index from here

* :doc:`pgr_bipartite` - Bipartite graph algorithm using a DFS-based coloring
  approach.
* :doc:`pgr_edgeColoring` - Edge Coloring algorithm using Vizing's theorem.

.. index to here


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
