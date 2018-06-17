..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_aStarCost:

pgr_aStarCost
===============================================================================

Synopsis
-------------------------------------------------------------------------------

``pgr_aStarCost`` — Returns the aggregate cost shortest path using :doc:`pgr_aStar` algorithm.

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org//libs/graph/doc/astar_search.html

   Boost Graph Inside

.. rubric:: Availability: 2.4.0


.. rubric:: Characteristics

The main Characteristics are:

* Using internaly the :doc:`pgr_aStar` algorithm
* Default kind of graph is **directed**  when

  * ``directed`` flag is missing.
  * ``directed`` flag is set to true

* Ordering is:

  *  first by ``start_vid``
  *  then by ``end_vid``

* Let :math:`v` and :math:`u` are nodes on the graph:

  * when there is no path from :math:`v` to :math:`u`:

    * no corresponding row is returned
    * ``agg_cost`` from :math:`v` to :math:`u` is :math:`\infty`

  * when :math:`v = u` then

    * no corresponding row is returned
    * ``agg_cost`` from `v` to `u` is :math:`0`

* The result is equivalent to the union of the results of the `pgr_aStarCost(` `One to One`_ `)` on the:

  * `One to Many`_
  * `Many to One`_
  * `Many to Many`_




Signature Summary
-----------------

.. code-block:: none

    pgr_aStarCost(edges_sql, start_vid, end_vid)
    pgr_aStarCost(edges_sql, start_vid, end_vid [, directed, heuristic, factor, epsilon])
    pgr_aStarCost(edges_sql, start_vid, end_vids [, directed, heuristic, factor, epsilon])
    pgr_aStarCost(edges_sql, starts_vid, end_vid [, directed, heuristic, factor, epsilon])
    pgr_aStarCost(edges_sql, starts_vid, end_vids [, directed, heuristic, factor, epsilon])

    RETURNS SET OF (start_vid, end_vid, agg_cost) OR EMPTY SET



.. index::
    single: aStarCost(Minimal Signature)

Minimal Signature
...............................................................................

.. code-block:: none

    pgr_aStarCost(edges_sql, start_vid, end_vid)
    RETURNS SET OF (start_vid, end_vid, agg_cost) OR EMPTY SET

:Example: Using the defaults

.. literalinclude:: doc-aStarCost.queries
   :start-after: --q1
   :end-before: --q2



.. index::
    single: aStarCost(One to One)

One to One
...............................................................................
.. code-block:: none

    pgr_aStarCost(edges_sql, start_vid, end_vid [, directed, heuristic, factor, epsilon])
    RETURNS SET OF (start_vid, end_vid, agg_cost) OR EMPTY SET

:Example: Using heuristic `2`

.. literalinclude:: doc-aStarCost.queries
   :start-after: --q2
   :end-before: --q3



.. index::
    single: aStarCost(One to Many)

One to many
.......................................

.. code-block:: none

    pgr_aStarCost(edges_sql, start_vid, end_vids [, directed, heuristic, factor, epsilon])
    RETURNS SET OF (start_vid, end_vid, agg_cost) OR EMPTY SET

:Example: From vertex `2` to vertices `3` and `12` using heuristic `2`

.. literalinclude:: doc-aStarCost.queries
   :start-after: --q3
   :end-before: --q4

.. index::
    single: aStarCost(Many to One)

Many to One
.......................................

.. code-block:: none

    pgr_aStarCost(edges_sql, start_vids, end_vid [, directed, heuristic, factor, epsilon])
    RETURNS SET OF (start_vid, end_vid, agg_cost) OR EMPTY SET

:Example: From vertices `2` and `7` to vertices `3` and `12` using heuristic `0`

.. literalinclude:: doc-aStarCost.queries
   :start-after: --q4
   :end-before: --q5



.. index::
    single: aStarCost(Many to Many)

Many to Many
.......................................

.. code-block:: none

    pgr_aStarCost(edges_sql, start_vids, end_vids [, directed, heuristic, factor, epsilon])
    RETURNS SET OF (start_vid, end_vid, agg_cost) OR EMPTY SET

:Example:

.. literalinclude:: doc-aStarCost.queries
   :start-after: --q5
   :end-before: --q6




Parameters
--------------------------------------------------------

================ ====================== =================================================
Parameter        Type                   Description
================ ====================== =================================================
**edges_sql**    ``TEXT``               `edges_sql`_ inner query.
**start_vid**    ``ANY-INTEGER``        Starting vertex identifier.
**end_vid**      ``ANY-INTEGER``        Ending vertex identifier.
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

**factor**       ``FLOAT``              (optional). For units manipulation. :math:`factor > 0`.  Default ``1``. See :ref:`astar_factor`
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




See Also
-------------------------------------------------------------------------------

* :doc:`aStar-family`
* :doc:`cost-category`
* :doc:`costMatrix-category`
* Examples use :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
