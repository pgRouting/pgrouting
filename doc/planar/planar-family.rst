:file: This file is part of the pgRouting project.
:copyright: Copyright (c) 2026-2026 pgRouting developers
:license: Creative Commons Attribution-Share Alike 3.0 https://creativecommons.org/licenses/by-sa/3.0


.. index:: Planar Family

|

Planar - Family of functions
===============================================================================

.. include:: experimental.rst
   :start-after: warning-begin
   :end-before: end-warning

.. experimental-start

* :doc:`pgr_isPlanar` - Returns a boolean depending upon the planarity of the graph.
* :doc:`pgr_makeBiconnectedPlanar` - Returns edges to add to a planar graph to make each connected component biconnected.
* :doc:`pgr_makeMaximalPlanar` - Returns edges to add to a planar graph to make each connected component maximal.
* :doc:`pgr_planarFaces` - Identifies the faces of a planar embedding and lists
  every edge-face incidence for an undirected graph.

.. experimental-end

.. toctree::
    :hidden:

    pgr_isPlanar
    pgr_makeBiconnectedPlanar
    pgr_makeMaximalPlanar
    pgr_planarFaces

See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
