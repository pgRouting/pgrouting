:file: This file is part of the pgRouting project.
:copyright: Copyright (c) 2022-2026 pgRouting developers
:license: Creative Commons Attribution-Share Alike 3.0 https://creativecommons.org/licenses/by-sa/3.0


.. index:: Metrics Family

|

Metrics - Family of functions
===============================================================================

.. official-start

- :doc:`pgr_degree` - Returns a set of vertices and corresponding count of
  incident edges to the vertex.

.. official-end

.. include:: experimental.rst
   :start-after: warning-begin
   :end-before: end-warning

.. experimental-start

* :doc:`pgr_betweennessCentrality` - Calculates relative betweenness centrality
  using Brandes Algorithm

* :doc:`pgr_bandwidth` - Computes the bandwidth of a graph.

.. experimental-end

.. toctree::
    :hidden:

    pgr_degree
    pgr_betweennessCentrality
    pgr_bandwidth

See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
