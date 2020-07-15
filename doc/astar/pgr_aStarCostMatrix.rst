..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_aStarCostMatrix
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

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_aStarCostMatrix.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_aStarCostMatrix.html>`__
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_aStarCostMatrix.html>`__

* **Unsupported versions:**
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_aStarCostMatrix.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_aStarCostMatrix.html>`__

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

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_aStarCostMatrix(edges_sql, vids [, directed] [, heuristic] [, factor] [, epsilon])
    RETURNS SET OF (start_vid, end_vid, agg_cost)

.. rubric:: Using defaults

.. code-block:: none

    pgr_aStarCostMatrix(edges_sql, vids)
    RETURNS SET OF (start_vid, end_vid, agg_cost)

:Example: Cost matrix for vertices :math:`\{1, 2, 3, 4\}` on a **directed** graph

.. literalinclude:: doc-aStarCostMatrix.queries
   :start-after: -- astar q1
   :end-before: -- astar q2

.. index::
    single: aStarCostMatrix

Complete Signature
...............................................................................

.. code-block:: none

    pgr_aStarCostMatrix(edges_sql, vids, [, directed] [, heuristic] [, factor] [, epsilon])
    RETURNS SET OF (start_vid, end_vid, agg_cost)

:Example: Symmetric cost matrix for vertices :math:`\{1, 2, 3, 4\}` on an **undirected** graph using heuristic :math:`2`

.. literalinclude:: doc-aStarCostMatrix.queries
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
--------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_cost_start
    :end-before: return_cost_end

Additional Examples
-------------------------------------------------------------------------------

:Example: Use with :doc:`pgr_TSP`

.. literalinclude:: doc-aStarCostMatrix.queries
   :start-after: -- astar q3
   :end-before: -- astar q4

See Also
-------------------------------------------------------------------------------

* :doc:`aStar-family`
* :doc:`cost-category`
* :doc:`costMatrix-category`
* :doc:`TSP-family`
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

