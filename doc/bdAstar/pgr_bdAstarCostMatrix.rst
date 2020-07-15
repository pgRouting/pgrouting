..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_bdAstarCostMatrix
===============================================================================

``pgr_bdAstarCostMatrix`` - Calculates the a cost matrix using :doc:`pgr_aStar`.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/table_of_contents.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.0.0

  * **Official** function

* Version 2.5.0

  * New **Proposed** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_bdAstarCostMatrix.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_bdAstarCostMatrix.html>`__
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_bdAstarCostMatrix.html>`__

* **Unsupported versions:**
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_bdAstarCostMatrix.html>`__

Description
-------------------------------------------------------------------------------

**The main characteristics are:**

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

    pgr_bdAstarCostMatrix(edges_sql, vids [, directed] [, heuristic] [, factor] [, epsilon])
    RETURNS SET OF (start_vid, end_vid, agg_cost)

.. rubric:: Using defaults

.. code-block:: none

    pgr_bdAstarCostMatrix(edges_sql, vids)
    RETURNS SET OF (start_vid, end_vid, agg_cost)

:Example: Cost matrix for vertices :math:`\{1, 2, 3, 4\}` on a **directed** graph

.. literalinclude:: doc-pgr_bdAstarCostMatrix.queries
   :start-after: -- bdAstar q1
   :end-before: -- bdAstar q2


.. index::
    single: bdaStarCostMatrix

Complete Signature
...............................................................................

.. code-block:: none

    pgr_bdAstarCostMatrix(edges_sql, vids [, directed] [, heuristic] [, factor] [, epsilon])
    RETURNS SET OF (start_vid, end_vid, agg_cost)


:Example: Symmetric cost matrix for vertices :math:`\{1, 2, 3, 4\}` on an **undirected** graph using heuristic :math:`2`

.. literalinclude:: doc-pgr_bdAstarCostMatrix.queries
   :start-after: -- bdAstar q2
   :end-before: -- bdAstar q3

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

.. literalinclude:: doc-pgr_bdAstarCostMatrix.queries
   :start-after: -- bdAstar q3
   :end-before: -- bdAstar q4

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

