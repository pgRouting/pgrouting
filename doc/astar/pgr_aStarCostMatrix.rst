..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


``pgr_aStarCostMatrix``
===============================================================================

``pgr_aStarCostMatrix`` - Calculates the a cost matrix using :doc:`pgr_aStar`.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org//libs/graph/doc/astar_search.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.0.0

  * **Official** function

* Version 2.4.0

  * New **proposed** function

Description
-------------------------------------------------------------------------------

**The main characteristics are:**

* Using internaly the :doc:`pgr_aStar` algorithm
* Returns a cost matrix.
* No ordering is performed
* let `v` and `u` are nodes on the graph:

  * when there is no path from `v` to `u`:

    * no corresponding row is returned
    * cost from `v` to `u` is :math:`\inf`

  * when :math:`v = u` then

    * no corresponding row is returned
    * cost from `v` to `u` is :math:`0`

* When the graph is **undirected** the cost matrix is symmetric

.. index::
    single: aStarCostMatrix

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_aStarCostMatrix(`Edges SQL`_, **start vids**, [**options**])
   | **options:** ``[directed, heuristic, factor, epsilon]``

   | RETURNS SET OF |matrix-result|
   | OR EMPTY SET

:Example: Symmetric cost matrix for vertices :math:`\{5, 6, 10, 15\}` on an
          **undirected** graph using heuristic :math:`2`

.. literalinclude:: doc-aStarCostMatrix.queries
   :start-after: -- q2
   :end-before: -- q3

Parameters
-------------------------------------------------------------------------------

.. include:: costMatrix-category.rst
    :start-after: costMatrix_parameters_start
    :end-before: costMatrix_parameters_end

Optional parameters
...............................................................................

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

aStar optional Parameters
...............................................................................

.. include:: aStar-family.rst
    :start-after: astar_optionals_start
    :end-before: astar_optionals_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: xy_edges_sql_start
    :end-before: xy_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_cost_start
    :end-before: return_cost_end

Additional Examples
-------------------------------------------------------------------------------

:Example: Use with :doc:`pgr_TSP`

.. literalinclude:: doc-aStarCostMatrix.queries
   :start-after: -- q3
   :end-before: -- q4

See Also
-------------------------------------------------------------------------------

* :doc:`aStar-family`
* :doc:`costMatrix-category`
* :doc:`TSP-family`
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
