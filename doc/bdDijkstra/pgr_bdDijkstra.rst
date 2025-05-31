..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: Bidirectional Dijkstra Family ; pgr_bdDijkstra
   single: bdDijkstra

|

``pgr_bdDijkstra``
===============================================================================

``pgr_bdDijkstra`` — Returns the shortest path using Bidirectional Dijkstra
algorithm.

.. rubric:: Availability:

.. rubric:: Version 4.0.0

* Output columns standardized to |short-generic-result|
* Combinations signature promoted to official.

.. rubric:: Version 3.2.0

* New proposed signature:

  * pgr_bdDijkstra(Combinations)

.. rubric:: Version 3.0.0

* Function promoted to official.

.. rubric:: Version 2.5.0

* New proposed signatures:

  * pgr_bdDijkstra(One to Many)
  * pgr_bdDijkstra(Many to One)
  * pgr_bdDijkstra(Many to Many)

.. rubric:: Version 2.4.0

* Signature change on pgr_bdDijsktra(One to One)

  * Old signature no longer supported

.. rubric:: Version 2.0.0

* New official function.


Description
-------------------------------------------------------------------------------

**The main characteristics are:**

.. include:: bdDijkstra-family.rst
   :start-after: description start
   :end-before: description end

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_bdDijkstra(`Edges SQL`_, **start vid**, **end vid**, [``directed``])
   | pgr_bdDijkstra(`Edges SQL`_, **start vid**, **end vids**, [``directed``])
   | pgr_bdDijkstra(`Edges SQL`_, **start vids**, **end vid**, [``directed``])
   | pgr_bdDijkstra(`Edges SQL`_, **start vids**, **end vids**, [``directed``])
   | pgr_bdDijkstra(`Edges SQL`_, `Combinations SQL`_, [``directed``])

   | Returns set of |short-generic-result|
   | OR EMPTY SET

.. index::
    single: bdDijkstra ; One to One

One to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_bdDijkstra(`Edges SQL`_, **start vid**, **end vid**, [``directed``])

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: From vertex :math:`6` to vertex :math:`10` on a **directed** graph

.. literalinclude:: bdDijkstra.queries
    :start-after: -- q2
    :end-before: -- q3

.. index::
    single: bdDijkstra ; One to Many

One to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_bdDijkstra(`Edges SQL`_, **start vid**, **end vids**, [``directed``])

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: From vertex :math:`6` to vertices :math:`\{10, 17\}` on a **directed**
          graph

.. literalinclude:: bdDijkstra.queries
    :start-after: -- q3
    :end-before: -- q4

.. index::
    single: bdDijkstra ; Many to One

Many to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_bdDijkstra(`Edges SQL`_, **start vids**, **end vid**, [``directed``])

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: From vertices :math:`\{6, 1\}` to vertex :math:`17` on a **directed**
          graph

.. literalinclude:: bdDijkstra.queries
    :start-after: -- q4
    :end-before: -- q5

.. index::
    single: bdDijkstra ; Many to Many

Many to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_bdDijkstra(`Edges SQL`_, **start vids**, **end vids**, [``directed``])

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: From vertices :math:`\{6, 1\}` to vertices :math:`\{10, 17\}` on an
          **undirected** graph

.. literalinclude:: bdDijkstra.queries
    :start-after: -- q5
    :end-before: -- q51

.. index::
    single: bdDijkstra ; Combinations

Combinations
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_bdDijkstra(`Edges SQL`_, `Combinations SQL`_, [``directed``])

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: Using a combinations table on an **undirected** graph

The combinations table:

.. literalinclude:: bdDijkstra.queries
    :start-after: -- q51
    :end-before: -- q52

The query:

.. literalinclude:: bdDijkstra.queries
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

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Combinations SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_combinations_sql_start
    :end-before: basic_combinations_sql_end

Result columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_path_short_start
    :end-before: return_path_short_end

Additional Examples
-------------------------------------------------------------------------------

:Example 1: Demonstration of repeated values are ignored, and result is sorted.

.. literalinclude:: bdDijkstra.queries
    :start-after: -- q6
    :end-before: -- q7

:Example 2: Making **start vids** the same as **end vids**.

.. literalinclude:: bdDijkstra.queries
    :start-after: -- q7
    :end-before: -- q8

:Example 3: Manually assigned vertex combinations.

.. literalinclude:: bdDijkstra.queries
    :start-after: -- q8
    :end-before: -- q9

See Also
-------------------------------------------------------------------------------

* :doc:`bdDijkstra-family`
* :doc:`sampledata`
* https://en.wikipedia.org/wiki/Bidirectional_search

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
