..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/withPoints-family.html>`__
  (`3.3 <https://docs.pgrouting.org/3.3/en/withPoints-family.html>`__)
  `3.2 <https://docs.pgrouting.org/3.2/en/withPoints-family.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/withPoints-family.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/withPoints-family.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/withPoints-family.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/withPoints-family.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/withPoints-family.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/withPoints/doc/withPoints.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/withPoints/doc/withPoints.html>`__

withPoints - Family of functions
===============================================================================

When points are also given as input:

.. include:: proposed.rst
   :start-after: begin-warning
   :end-before: end-warning

.. index proposed from here

- :doc:`pgr_withPoints` - Route from/to points anywhere on the graph.
- :doc:`pgr_withPointsCost` - Costs of the shortest paths.
- :doc:`pgr_withPointsCostMatrix` - Costs of the shortest paths.
- :doc:`pgr_withPointsKSP` - K shortest paths.
- :doc:`pgr_withPointsDD` - Driving distance.
- :doc:`pgr_withPointsVia` - Via routing

.. index proposed to here

.. toctree::
    :hidden:

    pgr_withPoints
    pgr_withPointsCost
    pgr_withPointsCostMatrix
    pgr_withPointsKSP
    pgr_withPointsDD
    pgr_withPointsVia

.. contents:: Contents
   :local:

Introduction
-------------------------------------------------------------------------------

This family of functions belongs to the :doc:`withPoints-category` and the
functions that compose them are based one way or another on dijkstra algorithm.

Depending on the name:

- pgr_withPoints is pgr_dijkstra **with points**
- pgr_withPointsCost is pgr_dijkstraCost **with points**
- pgr_withPointsCostMatrix is pgr_dijkstraCostMatrix **with points**
- pgr_withPointsKSP is pgr_ksp **with points**
- pgr_withPointsDD is pgr_drivingDistance **with points**
- pgr_withPointsvia is pgr_dijkstraVia **with points**

.. include:: withPoints-category.rst
   :start-after: not involving vehicles
   :end-before: See Also

See Also
-------------------------------------------------------------------------------

* :doc:`withPoints-category`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
