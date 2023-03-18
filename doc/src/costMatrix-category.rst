..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


Cost Matrix - Category
===============================================================================

.. index from here

* :doc:`pgr_aStarCostMatrix`
* :doc:`pgr_bdAstarCostMatrix`
* :doc:`pgr_bdDijkstraCostMatrix`
* :doc:`pgr_dijkstraCostMatrix`
* :doc:`pgr_bdDijkstraCostMatrix`

.. index to here


.. rubric:: proposed

.. include:: proposed.rst
    :start-after: begin-warning
    :end-before: end-warning

.. index proposed from here

* :doc:`pgr_withPointsCostMatrix`

.. index proposed to here

General Information
-------------------------------------------------------------------------------

Synopsis
...............................................................................

:doc:`TSP-family` needs as input a symmetric cost matrix and no edge `(u, v)`
must value :math:`\infty`.

This collection of functions will return a cost matrix in form of a table.


Characteristics
...............................................................................

.. costMatrix_details_start

The main Characteristics are:

* Can be used as input to :doc:`pgr_TSP`.

  * Use directly when the resulting matrix is symmetric and there is no
    :math:`\infty` value.
  * It will be the users responsibility to make the matrix symmetric.

    * By using geometric or harmonic average of the non symmetric values.
    * By using max or min the non symmetric values.
    * By setting the upper triangle to be the mirror image of the lower
      triangle.
    * By setting the lower triangle to be the mirror image of the upper
      triangle.

  * It is also the users responsibility to fix an :math:`\infty` value.

* Each function works as part of the family it belongs to.
* It does not return a path.
* Returns the sum of the costs of the shortest path for pair combination of
  nodes in the graph.
* Process is done only on edges with positive costs.
* Values are returned when there is a path.

  * When the starting vertex and ending vertex are the same, there is no path.

    * The aggregate cost in the non included values `(v, v)` is `0`.

  * When the starting vertex and ending vertex are the different and there is no
    path.

    * The aggregate cost in the non included values `(u, v)` is :math:`\infty`.

* Let be the case the values returned are stored in a table:

  * The unique index would be the pair: ``(start_vid, end_vid)``.

* Depending on the function and its parameters, the results can be symmetric.

  * The aggregate cost of `(u, v)` is the same as for `(v, u)`.

* Any duplicated value in the **start vids** are ignored.

* The returned values are ordered:

  * ``start_vid`` ascending
  * ``end_vid`` ascending

.. costMatrix_details_end

Parameters
-------------------------------------------------------------------------------

.. rubric:: Used in:

* :doc:`pgr_aStarCostMatrix`
* :doc:`pgr_dijkstraCostMatrix`

.. costMatrix_parameters_start

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - `Edges SQL`_
     - ``TEXT``
     - `Edges SQL`_ as described below
   * - **start vids**
     - ``ARRAY[BIGINT]``
     -  Array of identifiers of starting vertices.

.. costMatrix_parameters_end

.. rubric:: Used in:

* :doc:`pgr_withPointsCostMatrix`

.. costMatrix_withPoints_parameters_start

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - `Edges SQL`_
     - ``TEXT``
     - `Edges SQL`_ as described below
   * - `Points SQL`_
     - ``TEXT``
     - `Points SQL`_ as described below
   * - **start vids**
     - ``ARRAY[BIGINT]``
     -  Array of identifiers of starting vertices.

.. costMatrix_withPoints_parameters_end

Optional parameters
...............................................................................

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. rubric:: Used in:

* :doc:`pgr_withPointsCostMatrix`
* :doc:`pgr_dijkstraCostMatrix`

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Points SQL
...............................................................................

.. include:: withPoints-category.rst
    :start-after: points_sql_start
    :end-before: points_sql_end

Result Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_cost_start
    :end-before: return_cost_end

See Also
-------------------------------------------------------------------------------

* :doc:`TSP-family`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
