..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
    single: K Shortest Paths Category ; pgr_KSP
    single: KSP

|

``pgr_KSP``
===============================================================================

``pgr_KSP`` — Yen's algorithm for K shortest paths using Dijkstra.

.. rubric:: Availability

.. rubric:: Version 4.0.0

* All signatures promoted to official.

.. rubric:: Version 3.6.0

* Standardizing output columns to |nksp-result|
* pgr_ksp(One to One)

  * Added ``start_vid`` and ``end_vid`` result columns.

* New proposed signatures:

  * pgr_ksp(One to Many)
  * pgr_ksp(Many to One)
  * pgr_ksp(Many to Many)
  * pgr_ksp(Combinations)

.. rubric:: Version 2.1.0

* Signature change

  * Old signature no longer supported

.. rubric:: Version 2.0.0

* Official function.


Description
-------------------------------------------------------------------------------

The K shortest path routing algorithm based on Yen's algorithm. "K" is the
number of shortest paths desired.

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_KSP(`Edges SQL`_, **start vid**, **end vid**, **K**, [**options**])
   | pgr_KSP(`Edges SQL`_, **start vid**, **end vids**, **K**, [**options**])
   | pgr_KSP(`Edges SQL`_, **start vids**, **end vid**, **K**, [**options**])
   | pgr_KSP(`Edges SQL`_, **start vids**, **end vids**, **K**, [**options**])
   | pgr_KSP(`Edges SQL`_, `Combinations SQL`_, **K**, [**options**])
   | **options:** ``[directed, heap_paths]``

   | Returns set of |nksp-result|
   | OR EMPTY SET

.. index::
    single: KSP ; One to One

One to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_KSP(`Edges SQL`_, **start vid**, **end vid**, **K**, [**options**])
   | **options:** ``[directed, heap_paths]``

   | Returns set of |nksp-result|
   | OR EMPTY SET

:Example: Get 2 paths from :math:`6` to :math:`17` on a directed graph.

.. literalinclude:: ksp.queries
    :start-after: --q1
    :end-before: --q2

.. index::
    single: KSP ; One to Many

One to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_KSP(`Edges SQL`_, **start vid**, **end vids**, **K**, [**options**])
   | **options:** ``[directed, heap_paths]``

   | Returns set of |nksp-result|
   | OR EMPTY SET

:Example: Get 2 paths from vertex :math:`6` to vertices :math:`\{10, 17\}` on a directed graph.

.. literalinclude:: ksp.queries
    :start-after: --q2
    :end-before: --q3

.. index::
    single: KSP ; Many to One

Many to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_KSP(`Edges SQL`_, **start vids**, **end vid**, **K**, [**options**])
   | **options:** ``[directed, heap_paths]``

   | Returns set of |nksp-result|
   | OR EMPTY SET

:Example: Get 2 paths from vertices :math:`\{6, 1\}` to vertex :math:`17` on a directed graph.

.. literalinclude:: ksp.queries
    :start-after: --q3
    :end-before: --q4

.. index::
    single: KSP ; Many to Many

Many to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_KSP(`Edges SQL`_, **start vids**, **end vids**, **K**, [**options**])
   | **options:** ``[directed, heap_paths]``

   | Returns set of |nksp-result|
   | OR EMPTY SET

:Example: Get 2 paths vertices :math:`\{6, 1\}` to vertices :math:`\{10, 17\}` on a directed graph.

.. literalinclude:: ksp.queries
    :start-after: --q4
    :end-before: --q5

.. index::
   single: KSP ; Combinations

Combinations
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_KSP(`Edges SQL`_, `Combinations SQL`_, **K**, [**options**])
   | **options:** ``[directed, heap_paths]``

   | Returns set of |nksp-result|
   | OR EMPTY SET

:Example: Using a combinations table on an directed graph

The combinations table:

.. literalinclude:: dijkstra.queries
    :start-after: -- q51
    :end-before: -- q52

The query:

.. literalinclude:: ksp.queries
    :start-after: --q5
    :end-before: --q6


Parameters
-------------------------------------------------------------------------------

.. ksp_parameters_start

.. list-table::
   :width: 81
   :widths: 17 22 44
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - `Edges SQL`_
     - ``TEXT``
     - SQL query as described.
   * - **start vid**
     - **ANY-INTEGER**
     - Identifier of the departure vertex.
   * - **end vid**
     - **ANY-INTEGER**
     - Identifier of the destination vertex.
   * - **K**
     - **ANY-INTEGER**
     - Number of required paths.

Where:

:ANY-INTEGER: ``SMALLINT``, ``INTEGER``, ``BIGINT``

.. ksp_parameters_end

Optional parameters
...............................................................................

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

KSP Optional parameters
-------------------------------------------------------------------------------

.. ksp_optionals_start

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Default
     - Description
   * - ``heap_paths``
     - ``BOOLEAN``
     - ``false``
     - * When ``false`` Returns at most K paths.
       * When ``true`` all the calculated paths while processing are returned.
       * Roughly, when the shortest path has ``N`` edges, the heap will contain
         about than ``N * K`` paths for small value of ``K`` and ``K > 5``.

.. ksp_optionals_end


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
    :start-after: return_path_all_columns_start
    :end-before: return_path_all_columns_end

Additional Examples
-------------------------------------------------------------------------------

:Example: Get 2 paths from :math:`6` to :math:`17` on an undirected graph

Also get the paths in the heap.

.. literalinclude:: ksp.queries
    :start-after: --q6
    :end-before: --q7

:Example: Get 2 paths using combinations table on an undirected graph

Also get the paths in the heap.

.. literalinclude:: ksp.queries
    :start-after: --q7
    :end-before: --q8

:Example: Get 2 paths from vertices :math:`\{6, 1\}` to vertex :math:`17` on a undirected graph.

.. literalinclude:: ksp.queries
    :start-after: --q8
    :end-before: --q9

:Example: Get 2 paths vertices :math:`\{6, 1\}` to vertices :math:`\{10, 17\}` on a directed graph.

Also get the paths in the heap.

.. literalinclude:: ksp.queries
    :start-after: --q9
    :end-before: --q10

See Also
-------------------------------------------------------------------------------

* :doc:`KSP-category`
* :doc:`sampledata`
* https://en.wikipedia.org/wiki/K_shortest_path_routing

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
