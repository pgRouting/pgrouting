..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_bdAstarCostMatrix
===============================================================================

``pgr_bdAstarCostMatrix`` - Calculates the a cost matrix using :doc:`pgr_aStar`.

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph

   Boost Graph Inside

.. rubric:: Availability

* Official on v3.0.0
* Proposed on v2.5.0

Description
-------------------------------------------------------------------------------

The main Characteristics are:

* Using internaly the :doc:`pgr_bdAstar` algorithm
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

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_bdAstarCostMatrix(edges_sql, vids)
    pgr_bdAstarCostMatrix(edges_sql, vids [, directed, heuristic, factor, epsilon])
    RETURNS SET OF (start_vid, end_vid, agg_cost)

.. rubric::  Minimal Signature

The minimal signature:

* Is for a **directed** graph.

.. code-block:: none

    pgr_bdAstarCostMatrix(edges_sql, vids)
    RETURNS SET OF (start_vid, end_vid, agg_cost)

:Example: Cost matrix for vertices 1, 2, 3, and 4.

.. literalinclude:: doc-pgr_fooDmatrix.queries
   :start-after: -- astar q1
   :end-before: -- astar q2

.. rubric::  Complete Signature

.. code-block:: none

    pgr_bdAstarCostMatrix(edges_sql, vids [, directed, heuristic, factor, epsilon])
    RETURNS SET OF (start_vid, end_vid, agg_cost)


:Example: Symmetric cost matrix for vertices 1, 2, 3, and 4 using heuristic 2.

.. literalinclude:: doc-pgr_fooDmatrix.queries
   :start-after: -- astar q2
   :end-before: -- astar q3

Parameters
-------------------------------------------------------------------------------

================ ====================== =================================================
Parameter        Type                   Description
================ ====================== =================================================
**edges_sql**    ``TEXT``               `edges_sql`_  inner query.
**vids**         ``ARRAY[ANY-INTEGER]`` Array of vertices identifiers.
================ ====================== =================================================

Optional Parameters
...............................................................................

.. include:: pgr_aStar.rst
   :start-after: aStar optional parameters start
   :end-before: aStar optional parameters end

Inner query
-------------------------------------------------------------------------------

edges_sql
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

.. literalinclude:: doc-pgr_fooDmatrix.queries
   :start-after: -- astar q3
   :end-before: -- astar q4

See Also
-------------------------------------------------------------------------------

* :doc:`aStar-family`
* :doc:`bdAstar-family`
* :doc:`cost-category`
* :doc:`costMatrix-category`
* :doc:`TSP-family`
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

