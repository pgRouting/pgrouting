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
  (`3.6 <https://docs.pgrouting.org/3.6/en/withPoints-family.html>`__)
  `3.5 <https://docs.pgrouting.org/3.5/en/withPoints-family.html>`__
  `3.4 <https://docs.pgrouting.org/3.4/en/withPoints-family.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/withPoints-family.html>`__
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

Parameters
-------------------------------------------------------------------------------

.. include:: withPoints-category.rst
    :start-after: withPoints_parameters_start
    :end-before: withPoints_parameters_end

Optional parameters
...............................................................................

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

With points optional parameters
...............................................................................

.. withPoints_optionals_start

.. list-table::
   :width: 81
   :widths: 14 7 7 60
   :header-rows: 1

   * - Parameter
     - Type
     - Default
     - Description
   * - ``driving_side``
     - ``CHAR``
     - ``b``
     - Value in [``r``, ``l``, ``b``] indicating if the driving side is:

       - ``r`` for right driving side.
       - ``l`` for left driving side.
       - ``b`` for both.
   * - ``details``
     - ``BOOLEAN``
     - ``false``
     - - When ``true`` the results will include the points that are in the path.
       - When ``false`` the results will not include the points that are in the
         path.

.. withPoints_optionals_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Points SQL
...............................................................................

.. include:: withPoints-category.rst
    :start-after: points_sql_start
    :end-before: points_sql_end

Combinations SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_combinations_sql_start
    :end-before: basic_combinations_sql_end

Advanced Documentation
-------------------------------------------------------------------------------

.. include:: withPoints-category.rst
   :start-after: advanced_documentation_start
   :end-before: advanced_documentation_end

See Also
-------------------------------------------------------------------------------

* :doc:`withPoints-category`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
