..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_aStarCost
===============================================================================

Synopsis
-------------------------------------------------------------------------------

``pgr_aStarCost`` — Returns the aggregate cost shortest path using :doc:`pgr_aStar` algorithm.

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org//libs/graph/doc/astar_search.html

   Boost Graph Inside

.. rubric:: Characteristics

.. include:: aStar-family.rst
   :start-after: astar general info start
   :end-before: astar general info end

* The results are equivalent to the union of the results of the `pgr_aStarCost(` `One to One`_ `)` on the:

  * `pgr_aStarCost(` `One to Many`_ `)`
  * `pgr_aStarCost(` `Many to One`_ `)`
  * `pgr_aStarCost(` `Many to Many`_ `)`

.. rubric:: Availability

* New in v2.4.0


Signature Summary
-----------------

.. code-block:: none

    pgr_aStarCost(edges_sql, start_vid, end_vid)
    pgr_aStarCost(edges_sql, start_vid, end_vid [, directed, heuristic, factor, epsilon])
    pgr_aStarCost(edges_sql, start_vid, end_vids [, directed, heuristic, factor, epsilon])
    pgr_aStarCost(edges_sql, starts_vid, end_vid [, directed, heuristic, factor, epsilon])
    pgr_aStarCost(edges_sql, starts_vid, end_vids [, directed, heuristic, factor, epsilon])

    RETURNS SET OF (start_vid, end_vid, agg_cost) OR EMPTY SET

Optional parameters are `named parameters` and have a default value.



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

:Example: From vertex `2` to vertices :math:`\{3, 12\}` using heuristic `2`

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

:Example: From vertices :math:`\{2, 7\}` to vertex :math:`12` using heuristic `0`

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

:Example: From vertices :math:`\{2, 7\}` to vertices :math:`\{3, 12\}` using heuristic `2`

.. literalinclude:: doc-aStarCost.queries
   :start-after: --q5
   :end-before: --q6




Parameters
--------------------------------------------------------

.. include:: pgr_aStar.rst
   :start-after: aStar aStarCost parameters start
   :end-before: aStar aStarCost parameters end


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
