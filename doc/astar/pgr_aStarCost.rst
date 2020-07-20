..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_aStarCost
===============================================================================

``pgr_aStarCost`` — Returns the aggregate cost shortest path using :doc:`pgr_aStar` algorithm.

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
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_aStarCost.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_aStarCost.html>`__
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_aStarCost.html>`__

* **Unsupported versions:**
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_aStarCost.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_aStarCost.html>`__

Description
-------------------------------------------------------------------------------

**The main characteristics are:**

.. include:: aStar-family.rst
   :start-after: astar general info start
   :end-before: astar general info end

* The results are equivalent to the union of the results of the `pgr_aStarCost(` `One to One`_ `)` on the:

  * `pgr_aStarCost(` `One to Many`_ `)`
  * `pgr_aStarCost(` `Many to One`_ `)`
  * `pgr_aStarCost(` `Many to Many`_ `)`

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_aStarCost(edges_sql, from_vid,  to_vid   [, directed] [, heuristic] [, factor] [, epsilon])
    pgr_aStarCost(edges_sql, from_vid,  to_vids  [, directed] [, heuristic] [, factor] [, epsilon])
    pgr_aStarCost(edges_sql, from_vids, to_vid   [, directed] [, heuristic] [, factor] [, epsilon])
    pgr_aStarCost(edges_sql, from_vids, to_vids  [, directed] [, heuristic] [, factor] [, epsilon])

    RETURNS SET OF (start_vid, end_vid, agg_cost)
    OR EMPTY SET

Optional parameters are `named parameters` and have a default value.

.. rubric:: Using defaults

.. code-block:: none

    pgr_aStarCost(edges_sql, start_vid, end_vid)
    RETURNS SET OF (start_vid, end_vid, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertex  :math:`12` on a **directed** graph

.. literalinclude:: doc-aStarCost.queries
   :start-after: --q1
   :end-before: --q2

.. index::
    single: aStarCost(One to One)

One to One
...............................................................................

.. code-block:: none

    pgr_aStarCost(edges_sql, from_vid,  to_vid   [, directed] [, heuristic] [, factor] [, epsilon])
    RETURNS SET OF (start_vid, end_vid, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertex  :math:`12` on an **undirected** graph using heuristic :math:`2`

.. literalinclude:: doc-aStarCost.queries
   :start-after: --q2
   :end-before: --q3

.. index::
    single: aStarCost(One to Many)

One to many
...............................................................................

.. code-block:: none

    pgr_aStarCost(edges_sql, from_vid,  to_vids  [, directed] [, heuristic] [, factor] [, epsilon])
    RETURNS SET OF (start_vid, end_vid, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertices :math:`\{3, 12\}` on a **directed** graph using heuristic :math:`2`

.. literalinclude:: doc-aStarCost.queries
   :start-after: --q3
   :end-before: --q4

.. index::
    single: aStarCost(Many to One)

Many to One
...............................................................................

.. code-block:: none

    pgr_aStarCost(edges_sql, from_vids, to_vid   [, directed] [, heuristic] [, factor] [, epsilon])
    RETURNS SET OF (start_vid, end_vid, agg_cost)
    OR EMPTY SET

:Example: From vertices :math:`\{7, 2\}` to vertex :math:`12` on a **directed** graph using heuristic :math:`0`

.. literalinclude:: doc-aStarCost.queries
   :start-after: --q4
   :end-before: --q5

.. index::
    single: aStarCost(Many to Many)

Many to Many
..............................................................................

.. code-block:: none

    pgr_aStarCost(edges_sql, from_vids, to_vids  [, directed] [, heuristic] [, factor] [, epsilon])
    RETURNS SET OF (start_vid, end_vid, agg_cost)
    OR EMPTY SET

:Example: From vertices :math:`\{7, 2\}` to vertices :math:`\{3, 12\}` on a **directed** graph using heuristic :math:`2`

.. literalinclude:: doc-aStarCost.queries
   :start-after: --q5
   :end-before: --q6

Parameters
-------------------------------------------------------------------------------

.. include:: pgr_aStar.rst
   :start-after: aStar parameters start
   :end-before: aStar parameters end

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
--------------------------------------------------------------------------------

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
