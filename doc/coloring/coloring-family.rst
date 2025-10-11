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

.. experimental-start

* :doc:`pgr_bipartite` - Bipartite graph algorithm using a DFS-based coloring
  approach.
* :doc:`pgr_edgeColoring` - Edge Coloring algorithm using Vizing's theorem.

.. experimental-end


.. toctree::
    :hidden:

    pgr_sequentialVertexColoring
    pgr_bipartite
    pgr_edgeColoring


Result columns
-------------------------------------------------------------------------------

.. result-edge-color-start

Returns set of |result_edge_color|

===============  =========== ======================================
Column           Type        Description
===============  =========== ======================================
``edge``         ``BIGINT``  Identifier of the edge.
``color``        ``BIGINT``  Color of the edge.

                             - The minimum value of color is 1.

===============  =========== ======================================

.. result-edge-color-end

.. result-node-color-start

Returns set of |result_node_color|

============  =========== =====================================
Column           Type        Description
============  =========== =====================================
``node``      ``BIGINT``  Identifier of the node.
``color``     ``BIGINT``  Color of the node.

                          - The minimum value of color is 1.

============  =========== =====================================

.. result-node-color-end

See Also
-------------------------------------------------------------------------------

* `Boost: <https://www.boost.org/libs/graph/doc/table_of_contents.html>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
