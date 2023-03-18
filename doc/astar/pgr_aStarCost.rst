..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


pgr_aStarCost
===============================================================================

``pgr_aStarCost`` - Total cost of the shortest path(s) using the A* algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/astar_search.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.2.0

  * New **proposed** signature:

    * ``pgr_aStarCost`` (`Combinations`_)

* Version 3.0.0

  * **Official** function

* Version 2.4.0

  * New **proposed** function

Description
-------------------------------------------------------------------------------

The ``pgr_aStarCost`` function sumarizes of the cost of the shortest path(s)
using the A* algorithm.

**The main characteristics are:**

.. include:: aStar-family.rst
   :start-after: astar general info start
   :end-before: astar general info end

- It does not return a path.
- Returns the sum of the costs of the shortest path of each pair combination of
  nodes requested.
- Let be the case the values returned are stored in a table, so the unique index
  would be the pair: `(start_vid, end_vid)`

- For undirected graphs, the results are symmetric.

  - The  `agg_cost` of `(u, v)` is the same as for `(v, u)`.

- The returned values are ordered in ascending order:

  - `start_vid` ascending
  - `end_vid` ascending

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_aStarCost(`Edges SQL`_, **start vid**, **end vid**, [**options**])
   | pgr_aStarCost(`Edges SQL`_, **start vid**, **end vids**, [**options**])
   | pgr_aStarCost(`Edges SQL`_, **start vids**, **end vid**, [**options**])
   | pgr_aStarCost(`Edges SQL`_, **start vids**, **end vids**, [**options**])
   | pgr_aStarCost(`Edges SQL`_, `Combinations SQL`_, [**options**])
   | **options:** ``[directed, heuristic, factor, epsilon]``

   | RETURNS SET OF |matrix-result|
   | OR EMPTY SET

.. index::
    single: aStarCost(One to One)

One to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_aStarCost(`Edges SQL`_, **start vid**, **end vid**, [**options**])
   |  **options:** ``[directed, heuristic, factor, epsilon]``

   | RETURNS SET OF |matrix-result|
   | OR EMPTY SET

:Example: From vertex :math:`6` to vertex :math:`12` on a **directed** graph
          with heuristic :math:`2`

.. literalinclude:: doc-aStarCost.queries
    :start-after: -- q2
    :end-before: -- q3

.. index::
    single: aStarCost(One to Many)

One to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_aStarCost(`Edges SQL`_, **start vid**, **end vids**, [**options**])
   | **options:** ``[directed, heuristic, factor, epsilon]``

   | RETURNS SET OF |matrix-result|
   | OR EMPTY SET

:Example: From vertex :math:`6` to vertices :math:`\{10, 12\}` on a **directed**
          graph with heuristic :math:`3` and factor :math:`3.5`

.. literalinclude:: doc-aStarCost.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: aStarCost(Many to One)

Many to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_aStarCost(`Edges SQL`_, **start vids**, **end vid**, [**options**])
   | **options:** ``[directed, heuristic, factor, epsilon]``

   | RETURNS SET OF |matrix-result|
   | OR EMPTY SET

:Example: From vertices :math:`\{6, 8\}` to vertex :math:`10` on an
          **undirected** graph with heuristic :math:`4`

.. literalinclude:: doc-aStarCost.queries
    :start-after: -- q4
    :end-before: -- q5

.. index::
    single: aStarCost(Many to Many)

Many to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_aStarCost(`Edges SQL`_, **start vids**, **end vids**, [options])
   | **options:** ``[directed, heuristic, factor, epsilon]``

   | RETURNS SET OF |matrix-result|
   | OR EMPTY SET

:Example: From vertices :math:`\{6, 8\}` to vertices :math:`\{10, 12\}` on a
          **directed** graph with factor :math:`0.5`

.. literalinclude:: doc-aStarCost.queries
    :start-after: -- q5
    :end-before: -- q51

.. index::
    single: aStarCost(Combinations) - Proposed on v3.2

Combinations
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_aStarCost(`Edges SQL`_, `Combinations SQL`_, [**options**])
   | **options:** ``[directed, heuristic, factor, epsilon]``

   | RETURNS SET OF |matrix-result|
   | OR EMPTY SET

:Example: Using a combinations table on a **directed** graph with factor
          :math:`0.5`.

The combinations table:

.. literalinclude:: doc-aStarCost.queries
   :start-after: -- q51
   :end-before: -- q52

The query:

.. literalinclude:: doc-aStarCost.queries
   :start-after: -- q52
   :end-before: -- q6

Parameters
-------------------------------------------------------------------------------

.. include:: dijkstra-family.rst
    :start-after: dijkstra_parameters_start
    :end-before: dijkstra_parameters_end

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

Combinations SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_combinations_sql_start
    :end-before: basic_combinations_sql_end

Result Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_cost_start
    :end-before: return_cost_end

Additional Examples
-------------------------------------------------------------------------------

:Example 1: Demonstration of repeated values are ignored, and result is sorted.

.. literalinclude:: doc-aStarCost.queries
    :start-after: -- q6
    :end-before: -- q7

:Example 2: Making **start vids** the same as **end vids**.

.. literalinclude:: doc-aStarCost.queries
    :start-after: -- q7
    :end-before: -- q8

:Example 3: Manually assigned vertex combinations.

.. literalinclude:: doc-aStarCost.queries
   :start-after: -- q8
   :end-before: -- q9

See Also
-------------------------------------------------------------------------------

* :doc:`aStar-family`
* :doc:`cost-category`
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
