..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_aStarCostMatrix
===============================================================================

Synopsis
-------------------------------------------------------------------------------

``pgr_aStarCostMatrix`` - Calculates the a cost matrix using :doc:`pgr_aStar`.

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph

   Boost Graph Inside

.. rubric:: Availability

* New on version 2.4.0

.. rubric:: Characteristics

The main Characteristics are:

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


Signature Summary
-------------------------------------------------------------------------------

.. code-block:: none

    pgr_aStarCostMatrix(edges_sql, vids)
    pgr_aStarCostMatrix(edges_sql, vids, directed, heuristic, factor, epsilon)
    RETURNS SET OF (start_vid, end_vid, agg_cost)



.. index::
    single: aStarCostMatrix(Minimal Use)

Minimal Signature
...............................................................................

The minimal signature:

* Is for a **directed** graph.

.. code-block:: none

    pgr_aStarCostMatrix(edges_sql, vids)
    RETURNS SET OF (start_vid, end_vid, agg_cost)


:Example: Cost matrix for vertices 1, 2, 3, and 4.


.. literalinclude:: doc-pgr_fooDmatrix.queries
   :start-after: -- astar q1
   :end-before: -- astar q2


.. index::
    single: aStarCostMatrix(Complete Signature)

Complete Signature
...............................................................................

.. code-block:: none

    pgr_aStarCostMatrix(edges_sql, vids, directed, heuristic, factor, epsilon)
    RETURNS SET OF (start_vid, end_vid, agg_cost)


:Example: Symmetric cost matrix for vertices 1, 2, 3, and 4 using heuristic 2.

.. literalinclude:: doc-pgr_fooDmatrix.queries
   :start-after: -- astar q2
   :end-before: -- astar q3

Parameters
--------------------------------------------------------

================ ====================== =================================================
Parameter        Type                   Description
================ ====================== =================================================
**edges_sql**    ``TEXT``               `edges_sql`_  inner query.
**vids**         ``ARRAY[ANY-INTEGER]`` Array of vertices_identifiers.
**directed**     ``BOOLEAN``            - Optional.

                                          - When ``false`` the graph is considered as Undirected.
                                          - Default is ``true`` which considers the graph as Directed.

**heuristic**    ``INTEGER``            (optional). Heuristic number. Current valid values 0~5. Default ``5``

                                        - 0: h(v) = 0 (Use this value to compare with pgr_dijkstra)
                                        - 1: h(v) abs(max(dx, dy))
                                        - 2: h(v) abs(min(dx, dy))
                                        - 3: h(v) = dx * dx + dy * dy
                                        - 4: h(v) = sqrt(dx * dx + dy * dy)
                                        - 5: h(v) = abs(dx) + abs(dy)

**factor**       ``FLOAT``              (optional). For units manipulation. :math:`factor > 0`.  Default ``1``.
**epsilon**      ``FLOAT``              (optional). For less restricted results. :math:`epsilon >= 1`.  Default ``1``.
================ ====================== =================================================


Inner query
--------------------------------------------------------

edges_sql
...........................................................

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

.. literalinclude:: doc-pgr_fooDmatrix.queries
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

