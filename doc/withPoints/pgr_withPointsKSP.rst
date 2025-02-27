..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: withPoints Family ; pgr_withPointsKSP
   single: With Points Category ; pgr_withPointsKSP
   single: K Shortest Paths Category ; pgr_withPointsKSP
   single: withPointsKSP

|

``pgr_withPointsKSP`` - Proposed
===============================================================================

``pgr_withPointsKSP`` — Yen's algorithm for K shortest paths using Dijkstra.

.. include:: proposed.rst
   :start-after: warning-begin
   :end-before: end-warning

.. rubric:: Version 3.6.0

* Standarizing output columns to |nksp-result|
* pgr_withPointsKSP(One to One)

  * Signature change: ``driving_side`` parameter changed from named optional to
    unnamed compulsory **driving side**.
  * Added ``start_vid`` and ``end_vid`` result columns.

* New proposed signatures:

  * pgr_withPointsKSP(One to Many)
  * pgr_withPointsKSP(Many to One)
  * pgr_withPointsKSP(Many to Many)
  * pgr_withPointsKSP(Combinations)

* Deprecated signature

  * pgr_withpointsksp(text,text,bigint,bigint,integer,boolean,boolean,char,boolean)``

.. rubric:: Version 2.2.0

* New proposed function.


Description
-------------------------------------------------------------------------------

Modifies the graph to include the points defined in the `Points SQL`_ and
using Yen algorithm, finds the :math:`K` shortest paths.

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_withPointsKSP(`Edges SQL`_, `Points SQL`_, **start vid**, **end vid**, **K**, **driving_side**, [**options**])
   | pgr_withPointsKSP(`Edges SQL`_, `Points SQL`_, **start vid**, **end vids**, **K**, **driving_side**, [**options**])
   | pgr_withPointsKSP(`Edges SQL`_, `Points SQL`_, **start vids**, **end vid**, **K**, **driving_side**, [**options**])
   | pgr_withPointsKSP(`Edges SQL`_, `Points SQL`_, **start vids**, **end vids**, **K**, **driving_side**, [**options**])
   | pgr_withPointsKSP(`Edges SQL`_, `Points SQL`_, `Combinations SQL`_, **K**, **driving_side**, [**options**])
   | **options:** ``[directed, heap_paths, details]``

   | Returns set of |ksp-result|
   | OR EMPTY SET

.. index::
    single: withPointsKSP ; One to One - Proposed on v2.2

One to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_withPointsKSP(`Edges SQL`_, `Points SQL`_, **start vid**, **end vid**, **K**, **driving_side**, [**options**])
   | **options:** ``[directed, heap_paths, details]``

   | Returns set of |nksp-result|
   | OR EMTPY SET

:Example: Get 2 paths from Point :math:`1` to point :math:`2` on a directed
          graph with **left** side driving.

* For a directed graph.
* No details are given about distance of other points of the query.
* No heap paths are returned.

.. literalinclude:: withPointsKSP.queries
   :start-after: --q1
   :end-before: --q2

.. index::
    single: withPointsKSP ; One to Many - Proposed on v3.6

One to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_withPointsKSP(`Edges SQL`_, `Points SQL`_, **start vid**, **end vids**, **K**, **driving_side**, [**options**])
   | **options:** ``[directed, heap_paths, details]``

   | Returns set of |ksp-result|
   | OR EMTPY SET

:Example: Get 2 paths from point :math:`1` to point :math:`3` and vertex :math:`7` on an
          undirected graph

.. literalinclude:: withPointsKSP.queries
   :start-after: --q2
   :end-before: --q3

.. index::
    single: withPointsKSP ; Many to One - Proposed on v3.6

Many to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_withPointsKSP(`Edges SQL`_, `Points SQL`_, **start vids**, **end vid**, **K**, **driving_side**, [**options**])
   | **options:** ``[directed, heap_paths, details]``

   | Returns set of |ksp-result|
   | OR EMTPY SET

:Example: Get a path from point :math:`1` and vertex :math:`6` to point :math:`3` on a **directed**
          graph with **right** side driving and **details** set to **True**

.. literalinclude:: withPointsKSP.queries
   :start-after: --q3
   :end-before: --q4

.. index::
    single: withPointsKSP ; Many to Many - Proposed on v3.6

Many to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_withPointsKSP(`Edges SQL`_, `Points SQL`_, **start vids**, **end vids**, **K**, **driving_side**, [**options**])
   | **options:** ``[directed, heap_paths, details]``

   | Returns set of |nksp-result|
   | OR EMTPY SET

:Example: Get a path from point :math:`1` and vertex :math:`6` to point :math:`3` and vertex :math:`1` on a **directed**
          graph with **left** side driving and **heap_paths** set to **True**

.. literalinclude:: withPointsKSP.queries
   :start-after: --q4
   :end-before: --q5

.. index::
    single: withPointsKSP ; Combinations - Proposed on v3.6

Combinations
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_withPointsKSP(`Edges SQL`_, `Points SQL`_, `Combinations SQL`_, **K**, **driving_side**, [**options**])
   | **options:** ``[directed, heap_paths, details]``

   | Returns set of |ksp-result|
   | OR EMTPY SET

:Example: Using a combinations table on an **directed** graph

.. literalinclude:: withPointsKSP.queries
    :start-after: --q5
    :end-before: --q6

Parameters
-------------------------------------------------------------------------------

.. list-table::
   :width: 81
   :widths: 17 22 44
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - `Edges SQL`_
     - ``TEXT``
     - `Edges SQL`_ query as described.
   * - `Points SQL`_
     - ``TEXT``
     - `Points SQL`_ query as described.
   * - **start vid**
     - **ANY-INTEGER**
     - Identifier of the departure vertex.

       - Negative values represent a point
   * - **end vid**
     - **ANY-INTEGER**
     - Identifier of the destination vertex.

       - Negative values represent a point
   * - **K**
     - **ANY-INTEGER**
     - Number of required paths
   * - **driving_side**
     - **CHAR**
     - Value in [``r``, ``R``, ``l``, ``L``, ``b``, ``B``] indicating if the driving side is:

       - [``r``, ``R``] for right driving side (for directed graph only)
       - [``l``, ``L``] for left driving side (for directed graph only)
       - [``b``, ``B``] for both (only for undirected graph)

Where:

:ANY-INTEGER: ``SMALLINT``, ``INTEGER``, ``BIGINT``

Optional parameters
...............................................................................

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

KSP Optional parameters
-------------------------------------------------------------------------------

.. include:: pgr_KSP.rst
    :start-after: ksp_optionals_start
    :end-before: ksp_optionals_end

withPointsKSP optional parameters
...............................................................................

.. list-table::
   :width: 81
   :widths: 14 7 7 60
   :header-rows: 1

   * - Parameter
     - Type
     - Default
     - Description
   * - ``details``
     - ``BOOLEAN``
     - ``false``
     - - When ``true`` the results will include the points that are in the path.
       - When ``false`` the results will not include the points that are in the
         path.

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Points SQL
...............................................................................

.. include:: withPoints-category.rst
    :start-after: points_sql_start
    :end-before: points_sql_end

Combinations SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_combinations_sql_start
    :end-before: basic_combinations_sql_end

Result columns
-------------------------------------------------------------------------------

.. include:: pgr_KSP.rst
    :start-after: ksp_returns_start
    :end-before: ksp_returns_end

Additional Examples
-------------------------------------------------------------------------------

.. contents::
   :local:

Use :doc:`pgr_findCloseEdges` in the `Points SQL`_.
...............................................................................

Get :math:`2` paths using left side driving topology, from vertex :math:`1` to
the closest location on the graph of point `(2.9, 1.8)`.

.. literalinclude:: withPointsKSP.queries
    :start-after: --q6
    :end-before: --q7

* Point :math:`-1` corresponds to the closest edge from point `(2.9, 1.8)`.

Left driving side
...............................................................................

Get :math:`2` paths using left side driving topology, from point :math:`1` to
point :math:`3` with details.

.. literalinclude:: withPointsKSP.queries
   :start-after: --q7
   :end-before: --q8

Right driving side
...............................................................................

Get :math:`2` paths using right side driving topology from, point :math:`1` to
point :math:`2` with heap paths and details.

.. literalinclude:: withPointsKSP.queries
   :start-after: --q8
   :end-before: --q9

See Also
-------------------------------------------------------------------------------

* :doc:`withPoints-family`
* :doc:`KSP-category`
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

