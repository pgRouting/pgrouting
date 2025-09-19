..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: aStar Family ; pgr_aStar
   single: aStar

|

``pgr_aStar``
===============================================================================

``pgr_aStar`` — Shortest path using the A* algorithm.

.. rubric:: Availability

.. rubric:: Version 4.0.0

* Combinations signature promoted to official.

.. rubric:: Version 3.6.0

* Standardizing output columns to |short-generic-result|

  * pgr_aStar(One to One) added ``start_vid`` and ``end_vid`` columns.
  * pgr_aStar(One to Many) added ``end_vid`` column.
  * pgr_aStar(Many to One) added ``start_vid`` column.

.. rubric:: Version 3.2.0

* New proposed signature:

  * pgr_aStar(Combinations)

.. rubric:: Version 3.0.0

* Function promoted to official.

.. rubric:: Version 2.4.0

* New proposed signatures:

  * pgr_aStar(One to Many)
  * pgr_aStar(Many to One)
  * pgr_aStar(Many to Many)

.. rubric:: Version 2.3.0

* Signature change on pgr_aStar(One to One)

  * Old signature no longer supported

.. rubric:: Version 2.0.0

* New official function.

Description
-------------------------------------------------------------------------------

**The main characteristics are:**

.. include:: aStar-family.rst
   :start-after: astar general info start
   :end-before: astar general info end

* The results are equivalent to the union of the results of the `pgr_aStar(`
  `One to One`_ `)` on the:

  * pgr_aStar(One to Many)
  * pgr_aStar(Many to One)
  * pgr_aStar(Many to Many)
  * pgr_aStar(Combinations)

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_aStar(`Edges SQL`_, **start vid**, **end vid**, [**options**])
   | pgr_aStar(`Edges SQL`_, **start vid**, **end vids**, [**options**])
   | pgr_aStar(`Edges SQL`_, **start vids**, **end vid**, [**options**])
   | pgr_aStar(`Edges SQL`_, **start vids**, **end vids**, [**options**])
   | pgr_aStar(`Edges SQL`_, `Combinations SQL`_, [**options**])
   | **options:** ``[directed, heuristic, factor, epsilon]``

   | Returns set of |short-generic-result|
   | OR EMPTY SET

Optional parameters are `named parameters` and have a default value.

.. index::
    single: aStar ; One to One

One to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_aStar(`Edges SQL`_, **start vid**, **end vid**, [**options**])
   | **options:** ``[directed, heuristic, factor, epsilon]``

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: From vertex :math:`6` to vertex :math:`12` on a **directed** graph
          with heuristic :math:`2`

.. literalinclude:: astar.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: aStar ; One to Many

One to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_aStar(`Edges SQL`_, **start vid**, **end vids**, [**options**])
   | **options:** ``[directed, heuristic, factor, epsilon]``

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: From vertex :math:`6` to vertices :math:`\{10, 12\}` on a **directed**
          graph with heuristic :math:`3` and factor :math:`3.5`

.. literalinclude:: astar.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: aStar ; Many to One

Many to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_aStar(`Edges SQL`_, **start vids**, **end vid**, [**options**])
   | **options:** ``[directed, heuristic, factor, epsilon]``

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: From vertices :math:`\{6, 8\}` to vertex :math:`10` on an
          **undirected** graph with heuristic :math:`4`

.. literalinclude:: astar.queries
   :start-after: -- q4
   :end-before: -- q5

.. index::
    single: aStar ; Many to Many

Many to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_aStar(`Edges SQL`_, **start vids**, **end vids**, [**options**])
   | **options:** ``[directed, heuristic, factor, epsilon]``

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: From vertices :math:`\{6, 8\}` to vertices :math:`\{10, 12\}` on a
          **directed** graph with factor :math:`0.5`

.. literalinclude:: astar.queries
   :start-after: -- q5
   :end-before: -- q51

.. index::
    single: aStar ; Combinations

Combinations
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_aStar(`Edges SQL`_, `Combinations SQL`_, [**options**])
   | **options:** ``[directed, heuristic, factor, epsilon]``

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: Using a combinations table on a **directed** graph with factor
          :math:`0.5`.

The combinations table:

.. literalinclude:: astar.queries
   :start-after: -- q51
   :end-before: -- q52

The query:

.. literalinclude:: astar.queries
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

aStar optional parameters
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

Result columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_path_complete_start
    :end-before: return_path_complete_end

Additional Examples
-------------------------------------------------------------------------------

:Example 1: Demonstration of repeated values are ignored, and result is sorted.

.. literalinclude:: astar.queries
    :start-after: -- q6
    :end-before: -- q7

:Example 2: Making **start vids** the same as **end vids**.

.. literalinclude:: astar.queries
    :start-after: -- q7
    :end-before: -- q8

:Example 3: Manually assigned vertex combinations.

.. literalinclude:: astar.queries
   :start-after: -- q8
   :end-before: -- q9

See Also
-------------------------------------------------------------------------------

* :doc:`aStar-family`
* :doc:`bdAstar-family`
* :doc:`sampledata`
* `Boost: A* search <https://www.boost.org/libs/graph/doc/astar_search.html>`__
* https://en.wikipedia.org/wiki/A*_search_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

