..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


Migration guide
===============================================================================

Several functions are having changes on the signatures, and/or have been
replaced by new functions.

Results can be different because of the changes.

.. warning::
   All deprecated functions will be removed on next major version 4.0.0

.. contents:: Contents
   :depth: 3


Migration to standardized columns
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. |old-pid-result| replace:: ``(seq, path_seq, [start_pid], [end_pid], node, edge, cost, agg_cost)``
.. |pid-1-m| replace:: ``(seq, path_seq, end_pid, node, edge, cost, agg_cost)``
.. |pid-m-1| replace:: ``(seq, path_seq, start_pid, node, edge, cost, agg_cost)``
.. |pid-m-m| replace:: ``(seq, path_seq, start_pid, end_pid, node, edge, cost, agg_cost)``
.. |matrix-pid| replace:: ``(start_pid, end_pid, agg_cost)``

There has been an effort to standardize function output columns names and
types.

* :ref:`pgRouting-concepts:Result columns for cost functions`

  * |matrix-result|

* :ref:`pgRouting-concepts:Result columns for single path functions`

  * |short-generic-result|

* :ref:`pgRouting-concepts:Result columns for spanning tree functions`

  * |result-spantree|


.. list-table::
   :header-rows: 1

   * - Function
     - Migration guide
   * - .. versionchanged:: 3.5.0 :doc:`pgr_dijkstra` [1]_
     - `Migration of single path functions`_
   * - .. versionchanged:: 3.6.0 :doc:`pgr_aStar` [1]_
     - `Migration of single path functions`_
   * - .. versionchanged:: 3.6.0 :doc:`pgr_bdAstar` [1]_
     - `Migration of single path functions`_
   * - .. versionchanged:: 3.6.0 :doc:`pgr_drivingDistance` [1]_
     - `Migration of spanning tree functions`_
   * - .. versionchanged:: 3.6.0 :doc:`pgr_withPointsDD` [2]_
     - `Migration of spanning tree functions`_
   * - .. versionchanged:: 3.7.0 :doc:`pgr_kruskalBFS` [1]_
     - `Migration of spanning tree functions`_
   * - .. versionchanged:: 3.7.0 :doc:`pgr_kruskalDD` [1]_
     - `Migration of spanning tree functions`_
   * - .. versionchanged:: 3.7.0 :doc:`pgr_kruskalDFS` [1]_
     - `Migration of spanning tree functions`_
   * - .. versionchanged:: 3.7.0 :doc:`pgr_primBFS` [1]_
     - `Migration of spanning tree functions`_
   * - .. versionchanged:: 3.7.0 :doc:`pgr_primDD` [1]_
     - `Migration of spanning tree functions`_
   * - .. versionchanged:: 3.7.0 :doc:`pgr_primDFS` [1]_
     - `Migration of spanning tree functions`_
   * - .. versionchanged:: 4.0.0 :doc:`pgr_bdDijkstra` [1]_
     - `Migration of single path functions`_
   * - .. versionchanged:: 4.0.0 :doc:`pgr_withPoints` [2]_
     - `Migration of single path functions`_
   * - .. versionchanged:: 4.0.0 :doc:`pgr_withPointsCost` [2]_
     - `Migration of cost functions`_
   * - .. versionchanged:: 4.0.0 :doc:`pgr_withPointsCostMatrix` [2]_
     - `Migration of cost functions`_


..
   * - .. versionchanged:: 4.0.0 :doc:`pgr_bellmanFord`
     - `Migration of single path functions`_
   * - .. versionchanged:: 4.0.0 :doc:`pgr_binaryBreadthFirstSearch`
     - `Migration of single path functions`_
   * - .. versionchanged:: 4.0.0 :doc:`pgr_edwardMoore` [1]_
     - `Migration of single path functions`_

.. [1] Official function before v4.0.0
.. [2] Official function in v4.0.0

.. contents:: Contents
   :local:

Migration of cost functions
-------------------------------------------------------------------------------

The standardized :ref:`pgRouting-concepts:Result columns for cost functions` are
|matrix-result|

The following functions need to be migrated when they are being used in an
application.

.. list-table::
   :header-rows: 1

   * - Function
     - Version
     - From
   * - ``pgr_withPointsCost``
     - v < 4.0
     - |matrix-pid|
   * - ``pgr_withPointsCostMatrix``
     - v < 4.0
     - |matrix-pid|

**to** |matrix-result|

.. rubric:: Migration of |matrix-pid|

Signatures to be migrated:

* One to One
* One to Many
* Many to One
* Many to Many
* Combinations

.. warning:: Breaking change

   If using ``pgr_withPointsCost``: column names must be changed after updating
   pgRouting

New output columns are |matrix-result|

To get the old version column names: rename ``start_vid`` to ``start_pid`` and
``end_vid`` to ``end_pid``.

.. contents:: Examples
   :local:

Examples for One to One for cost functions
...............................................................................

.. rubric:: Using ``pgr_withPointsCost``

Migrating `this v3.8
<https://docs.pgrouting.org/3.8/en/pgr_withPointsCost.html#one-to-one>`__
example.

:from: |matrix-pid|
:to: |matrix-result|

.. literalinclude:: migration.queries
   :start-after: --withPointsCost-1-to-1
   :end-before: --withPointsCost-1-to-1-filter

To get the old version column names: rename ``start_vid`` to ``start_pid`` and
``end_vid`` to ``end_pid``.

.. literalinclude:: migration.queries
   :start-after: --withPointsCost-1-to-1-filter
   :end-before: --withPointsCost-1-to-m

.. warning:: Breaking change

   If using ``pgr_withPointsCost``: column names must be changed after updating
   pgRouting

Examples for One to Many for cost functions
...............................................................................

.. rubric:: Using ``pgr_withPointsCost``

Migrating `this v3.8
<https://docs.pgrouting.org/3.8/en/pgr_withPointsCost.html#one-to-many>`__
example.

:from: |matrix-pid|
:to: |matrix-result|

.. literalinclude:: migration.queries
   :start-after: --withPointsCost-1-to-m
   :end-before: --withPointsCost-1-to-m-filter

To get the old version column names: rename ``start_vid`` to ``start_pid`` and
``end_vid`` to ``end_pid``.

.. literalinclude:: migration.queries
   :start-after: --withPointsCost-1-to-m-filter
   :end-before: --withPointsCost-m-to-1

.. warning:: Breaking change

   If using ``pgr_withPointsCost``: column names must be changed after updating
   pgRouting

Examples for Many to One for cost functions
...............................................................................

.. rubric:: Using ``pgr_withPointsCost``

Migrating `this v3.8
<https://docs.pgrouting.org/3.8/en/pgr_withPointsCost.html#many-to-one>`__
example.

:from: |matrix-pid|
:to: |matrix-result|

.. literalinclude:: migration.queries
   :start-after: --withPointsCost-m-to-1
   :end-before: --withPointsCost-m-to-1-filter

To get the old version column names: rename ``start_vid`` to ``start_pid`` and
``end_vid`` to ``end_pid``.

.. literalinclude:: migration.queries
   :start-after: --withPointsCost-m-to-1-filter
   :end-before: --withPointsCost-m-to-m

.. warning:: Breaking change

   If using ``pgr_withPointsCost``: column names must be changed after updating
   pgRouting

Examples for Many to Many for cost functions
...............................................................................

.. rubric:: Using ``pgr_withPointsCost``

Migrating `this v3.8
<https://docs.pgrouting.org/3.8/en/pgr_withPointsCost.html#many-to-many>`__
example.

:from: |matrix-pid|
:to: |matrix-result|

.. literalinclude:: migration.queries
   :start-after: --withPointsCost-m-to-m
   :end-before: --withPointsCost-m-to-m-filter

To get the old version column names: rename ``start_vid`` to ``start_pid`` and
``end_vid`` to ``end_pid``.

.. literalinclude:: migration.queries
   :start-after: --withPointsCost-m-to-m-filter
   :end-before: --withPointsCost-c

.. rubric:: Using ``pgr_withPointsCost``

.. warning:: Breaking change

   If using ``pgr_withPointsCost``: column names must be changed after updating
   pgRouting

Examples for Combinations for cost functions
...............................................................................

.. rubric:: Using ``pgr_withPointsCost``

Migrating `this v3.8
<https://docs.pgrouting.org/3.8/en/pgr_withPointsCost.html#combinations>`__
example.

:from: |matrix-pid|
:to: |matrix-result|

.. literalinclude:: migration.queries
   :start-after: --withPointsCost-c
   :end-before: --withPointsCost-c-filter

To get the old version column names: rename ``start_vid`` to ``start_pid`` and
``end_vid`` to ``end_pid``.

.. literalinclude:: migration.queries
   :start-after: --withPointsCost-c-filter
   :end-before: --withPointsCost-END

.. warning:: Breaking change

   If using ``pgr_withPointsCost``: column names must be changed after updating
   pgRouting

Migration of single path functions
-------------------------------------------------------------------------------

THe standardized :ref:`pgRouting-concepts:Result columns for single path
functions` are |short-generic-result|

The following functions need to be migrated when they are being used in an
application.

.. list-table::
   :header-rows: 1

   * - Function
     - Version
     - From
   * - ``pgr_aStar``
     - v < 3.6
     - |old-generic-result|
   * - ``pgr_dijkstra``
     - v < 3.5
     - |old-generic-result|
   * - ``pgr_bdDijkstra``
     - v < 4.0
     - |old-generic-result|
   * - ``pgr_withPoints``
     - v < 4.0
     - |old-pid-result|

..
   * - ``pgr_bellmanFord``
     - v < 4.0
     - |old-generic-result|
   * - ``pgr_BinaryBreadthFirstSearch``
     - v < 4.0
     - |old-generic-result|
   * - ``pgr_edwardMoore``
     - v < 4.0
     - |old-generic-result|

:to: |short-generic-result|

.. rubric:: Migration of |old-generic-result|

Signatures to be migrated:

* One to One
* One to Many
* Many to One

Before updating pgRouting, enumerate the corresponding columns of the signature

* Skip when applicable, ``start_vid``
* Skip when applicable, ``end_vid``

.. rubric:: Migration of |old-pid-result|

Signatures to be migrated:

* One to One
* One to Many
* Many to One
* Many to Many
* Combinations

To get the old version column names, depending on the signature:

* Filter out the columns: ``start_vid`` and/or ``end_vid``
* Rename the columns:

  * ``start_vid`` to ``start_pid``
  * ``end_vid`` to ``end_pid``

.. warning:: Breaking change

   If using ``pgr_withPoints``: column names must be changed after updating
   pgRouting

.. rubric:: Migration of |result-1-1|

Signatures to be migrated:

* One to One
* One to Many
* Many to One
* Many to Many
* Combinations

To get the old version column names:

* Filter out the columns: ``start_vid`` and ``end_vid``

.. contents:: Examples
   :local:

Examples with One to One with one route result
...............................................................................

.. rubric:: Using ``pgr_aStar``

Migrating `this v3.5
<https://docs.pgrouting.org/3.5/en/pgr_aStar.html#one-to-one>`__
example.

.. literalinclude:: migration.queries
   :start-after: --astar1
   :end-before: --astar2

Before updating pgRouting enumerate the columns: |result-1-1|

.. literalinclude:: migration.queries
   :start-after: --astar4
   :end-before: --astar5

.. rubric:: Using ``pgr_bdDijkstra``

Migrating `this v3.8
<https://docs.pgrouting.org/3.8/en/pgr_bdDijkstra.html#one-to-one>`__
example.

.. literalinclude:: migration.queries
   :start-after: --bdDijkstra-1-to-1
   :end-before: --bdDijkstra-1-to-1-filter

Before updating pgRouting enumerate the columns: |result-1-1|

.. literalinclude:: migration.queries
   :start-after: --bdDijkstra-1-to-1-filter
   :end-before: --bdDijkstra-1-to-m

Examples for One to Many with one route result
...............................................................................

.. rubric:: Using ``pgr_bdAstar``

Migrating `this v3.5
<https://docs.pgrouting.org/3.5/en/pgr_bdAstar.html#one-to-many>`__
example.

.. literalinclude:: migration.queries
   :start-after: --bdastar2
   :end-before: --bdastar3

Before updating pgRouting enumerate the columns: |result-1-m|

.. literalinclude:: migration.queries
   :start-after: --bdastar3
   :end-before: --bdastar4

.. rubric:: Using ``pgr_withPoints``

Migrating `this v3.8
<https://docs.pgrouting.org/3.8/en/pgr_withPoints.html#one-to-many>`__
example.

.. literalinclude:: migration.queries
   :start-after: --withPoints-1-to-m
   :end-before: --withPoints-1-to-m-filter

.. warning:: Breaking change

   If using ``pgr_withPoints``: column names must be changed after updating
   pgRouting

   :from: |pid-1-m|
   :to: |short-generic-result|

To get the old signature column names: filter out the
column ``start_vid`` and rename ``end_vid`` to ``end_pid``.

.. literalinclude:: migration.queries
   :start-after: --withPoints-1-to-m-filter
   :end-before: --withPoints-m-to-1

Examples for Many to One with one route result
...............................................................................

.. rubric:: Using ``pgr_bdDijkstra``

Migrating `this v3.8
<https://docs.pgrouting.org/3.8/en/pgr_bdDijkstra.html#many-to-one>`__
example.

.. literalinclude:: migration.queries
   :start-after: --bdDijkstra-m-to-1
   :end-before: --bdDijkstra-m-to-1-filter

Before updating pgRouting enumerate the columns: |result-m-1|

.. literalinclude:: migration.queries
   :start-after: --bdDijkstra-m-to-1-filter
   :end-before: --bdDijkstra-END

.. rubric:: Using ``pgr_dijkstra``

Migrating `this v3.4
<https://docs.pgrouting.org/3.4/en/pgr_dijkstra.html#many-to-one>`__
example.

.. literalinclude:: migration.queries
   :start-after: --dijkstra3
   :end-before: --dijkstra4

Before updating pgRouting enumerate the columns: |result-m-1|

.. literalinclude:: migration.queries
   :start-after: --dijkstra4
   :end-before: --dijkstra5

Examples for Many to Many with one route result
...............................................................................

.. rubric:: Using ``pgr_withPoints``

Migrating `this v3.8
<https://docs.pgrouting.org/3.8/en/pgr_withPoints.html#many-to-many>`__
example.

.. literalinclude:: migration.queries
   :start-after: --withPoints-m-to-m
   :end-before: --withPoints-m-to-m-filter

.. warning:: Breaking change

   If using ``pgr_withPoints``: column names must be changed after updating
   pgRouting

   :from: |pid-m-m|
   :to: |short-generic-result|

To get the old version column names: rename ``start_vid`` to ``start_pid`` and
``end_vid`` to ``end_pid``.

.. literalinclude:: migration.queries
   :start-after: --withPoints-m-to-m-filter
   :end-before: --withPoints-c

Examples for combinations with one route result
...............................................................................

.. rubric:: Using ``pgr_withPoints``

Migrating `this v3.8
`this <https://docs.pgrouting.org/3.8/en/pgr_withPoints.html#combinations>`__
example.

.. literalinclude:: migration.queries
   :start-after: --withPoints-c
   :end-before: --withPoints-c-filter

.. warning:: Breaking change

   If using ``pgr_withPoints``: column names must be changed after updating
   pgRouting

   :from: |pid-m-m|
   :to: |short-generic-result|

To get the old version column names: rename ``start_vid`` to ``start_pid`` and
``end_vid`` to ``end_pid``.

.. literalinclude:: migration.queries
   :start-after: --withPoints-c-filter
   :end-before: --withPoints-END

Migration of spanning tree functions
-------------------------------------------------------------------------------

The standardized :ref:`pgRouting-concepts:Result columns for spanning tree
functions` are |result-spantree|

.. list-table::
   :header-rows: 1

   * - Function
     - Version
     - From
   * - ``pgr_kruskalDD``
     - v < 3.7
     - |result-bfs|
   * - ``pgr_kruskalBFS``
     - v < 3.7
     - |result-bfs|
   * - ``pgr_kruskalDFS``
     - v < 3.7
     - |result-bfs|
   * - ``pgr_primDD``
     - v < 3.7
     - |result-bfs|
   * - ``pgr_primBFS``
     - v < 3.7
     - |result-bfs|
   * - ``pgr_primDFS``
     - v < 3.7
     - |result-bfs|
   * - ``pgr_drivingDistance``
     - v < 3.6
     - |result-dij-dd|
   * - ``pgr_withPointsDD``
     - v < 3.6
     - |result-generic-no-seq|

to |result-spantree|

.. rubric:: Migration of |result-bfs|

Signatures to be migrated:

* Single vertex
* Multiple vertices

Before updating pgRouting enumerate the columns: |result-bfs|

.. rubric:: Migration of |result-dij-dd|

Signatures to be migrated:

* Single vertex
* Multiple vertices

Migration depends on the signature.

For single vertex:

* Before updating pgRouting, enumerate |result-1-1| columns

For multiple vertices:

.. warning:: Breaking change

   If using ``pgr_drivingDistance`` with multiple vertices: column names must be
   changed after updating pgRouting.

To get the old version column names |result-dij-dd-m|

* Enumerate |result-dij-dd-m|
* Rename ``start_vid`` to ``from_v``.

.. rubric:: Migration of |result-generic-no-seq|

Signatures to be migrated:

* Single vertex
* Multiple vertices


.. warning:: Breaking change

   If using ``pgr_withPointsDD``: function call must be changed after updating
   pgRouting.

   ``pgr_withPointsDD``'s parameter ``driving_side`` changed from named optional
   to unnamed positional parameter (position 5 in the function call) and its
   validity differ for directed and undirected graphs.

Migration depends on the signature.

For ``pgr_withPointsDD`` Single vertex

To get the * Output columns were |result-1-1-no-seq|
* Does not have ``start_vid``, ``pred`` and ``depth`` result columns.
* ``driving_side`` parameter was named optional now it is compulsory unnamed.

For ``pgr_withPointsDD`` (Multiple vertices)

* Output columns were |result-m-1-no-seq|
* Does not have ``depth`` and ``pred`` result columns.
* ``driving_side`` parameter was named optional now it is compulsory unnamed.

Validity of driving_side:

* On directed graph ``b`` could be used as **driving side**
* On undirected graph ``r``, ``l`` could be used as **driving side**


.. rubric:: After Migration

* Be aware of the existence of the additional result Columns.

  * Output columns are |result-spantree|

* **driving side** parameter is unnamed compulsory, and valid values differ for
  directed and undirected graphs.

  * Does not have a default value.
  * In directed graph: valid values are [``r``, ``R``, ``l``, ``L``]
  * In undirected graph: valid values are [``b``, ``B``]
  * Using an invalid value throws an ``ERROR``.

.. contents:: Examples
   :local:

Examples for single vertex
...............................................................................

Using ``pgr_kruskalDD``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Migrating `this v3.6
<https://docs.pgrouting.org/3.6/en/pgr_kruskalDD.html#single-vertex>`__ example.

.. literalinclude:: migration.queries
   :start-after: --kruskalDD1
   :end-before: --kruskalDD2

Before updating pgRouting enumerate the columns: |result-bfs|.

.. literalinclude:: migration.queries
   :start-after: --kruskalDD2
   :end-before: --kruskalDD3

Using ``pgr_primBFS``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Migrating `this v3.6
<https://docs.pgrouting.org/3.6/en/pgr_primBFS.html#single-vertex>`__
example.

.. literalinclude:: migration.queries
   :start-after: --primDD1
   :end-before: --primDD2

Before updating pgRouting enumerate the columns: |result-bfs|.

.. literalinclude:: migration.queries
   :start-after: --primDD2
   :end-before: --primDD3

Using ``pgr_drivingDistance``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Migrating `this v3.5
<https://docs.pgrouting.org/3.5/en/pgr_drivingDistance.html#single-vertex>`__
example.

.. literalinclude:: migration.queries
   :start-after: --drivingdistance1
   :end-before: --drivingdistance2

Before updating pgRouting, enumerate |result-1-1-no-seq| columns

.. literalinclude:: migration.queries
   :start-after: --drivingdistance2
   :end-before: --drivingdistance3

Using ``pgr_withPointsDD``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Migrating `this v3.5
<https://docs.pgrouting.org/3.5/en/pgr_withPointsDD.html#single-vertex>`__
example.

.. warning:: Breaking change

   If using ``pgr_withPointsDD``: function call must be changed after updating
   pgRouting.

   ``pgr_withPointsDD``'s parameter ``driving_side`` changed from named optional
   to unnamed positional parameter (position 5 in the function call) and its
   validity differ for directed and undirected graphs.


.. literalinclude:: migration.queries
   :start-after: --withpointsdd4
   :end-before: --withpointsdd5

- Before updating pgRouting, enumerate |result-1-1-no-seq| columns
- After updating pgROuting use an unnamed valid value for **driving side** after
  the **distance** parameter.

.. literalinclude:: migration.queries
   :start-after: --withpointsdd5
   :end-before: --withpointsdd6

Examples for multiple vertices
...............................................................................

.. rubric:: Using ``pgr_kruskalDFS``

Migrating `this v3.6
<https://docs.pgrouting.org/3.6/en/pgr_kruskalDD.html#multiple-vertices>`__
example.

.. literalinclude:: migration.queries
   :start-after: --kruskalDD3
   :end-before: --kruskalDD4

Before updating pgRouting enumerate the columns: |result-bfs|.

.. literalinclude:: migration.queries
   :start-after: --kruskalDD4
   :end-before: --kruskalDD5

.. rubric:: Using ``pgr_primDD``

Migrating `this v3.6
<https://docs.pgrouting.org/3.6/en/pgr_primDD.html#multiple-vertices>`__
example.

.. literalinclude:: migration.queries
   :start-after: --primDD3
   :end-before: --primDD4

Before updating pgRouting enumerate the columns: |result-bfs|.

.. literalinclude:: migration.queries
   :start-after: --primDD4
   :end-before: --primDD5

.. rubric:: Using ``pgr_drivingDistance``

Migrating `this v3.5
<https://docs.pgrouting.org/3.5/en/pgr_drivingDistance.html#multiple-vertices>`__
example.

.. literalinclude:: migration.queries
   :start-after: --drivingdistance3
   :end-before: --drivingdistance4

.. warning:: Breaking change

   If using ``pgr_drivingDistance`` with multiple vertices: column names must be
   changed after updating pgRouting

To get the old version column names |result-dij-dd-m|: filter out the column
``pred`` and ``depth`` and rename ``start_vid`` to ``from_v``.


.. literalinclude:: migration.queries
   :start-after: --drivingdistance4
   :end-before: --drivingdistance5

Migration of deleted functions
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Functions no longer on pgRouting

.. contents:: Contents
   :local:

Migration of ``pgr_alphaShape``
-------------------------------------------------------------------------------

:Deprecated: `v3.8.0 <https://docs.pgrouting.org/3.8>`__
:Removed: `v4.0.0 <https://docs.pgrouting.org/4.0>`__

**Before Deprecation:** The following was calculated:

* An alphaShape was calculated

**After Deprecation:**

PostGIS has two ways of generating alphaShape.

If you have SFCGAL, which you can install using

::

  CREATE EXTENSION postgis_sfcgal

* Since PostGIS 3.5+ use `CG_AlphaShape <https://postgis.net/docs/CG_AlphaShape.html>`__
* For PostGIS 3.5+ use the old name ``ST_AlphaShape``

Other PostGIS options are

* `ST_ConvexHull <https://postgis.net/docs/ST_ConvexHull.html>`__
* `ST_ConcaveHull <https://postgis.net/docs/ST_ConcaveHull.html>`__

Migration of ``pgr_nodeNetwork``
-------------------------------------------------------------------------------

:Deprecated: `v3.8.0 <https://docs.pgrouting.org/3.8>`__
:Removed: `v4.0.0 <https://docs.pgrouting.org/4.0>`__

**Before Deprecation:** A table with `<edges>_nodded` was created. with split
edges.

**Migration**

Use :doc:`pgr_separateTouching` and/or use :doc:`pgr_separateCrossing`

Migration of ``pgr_createTopology``
-------------------------------------------------------------------------------

:Deprecated: `v3.8.0 <https://docs.pgrouting.org/3.8>`__
:Removed: `v4.0.0 <https://docs.pgrouting.org/4.0>`__

**Before Deprecation:** The following was calculated:

* A table with `<edges>_vertices_pgr` was created.

**After Deprecation:** The user is responsible to create the complete topology.

.. include:: pgRouting-concepts.rst
   :start-after: createTopology_start
   :end-before: createTopology_end

Migration of ``pgr_createVerticesTable``
-------------------------------------------------------------------------------

:Deprecated: `v3.8.0 <https://docs.pgrouting.org/3.8>`__
:Removed: `v4.0.0 <https://docs.pgrouting.org/4.0>`__

**Before Deprecation:** The following was calculated:

* A table with `<edges>_vertices_pgr` was created.

**After Deprecation:** The user is responsible to create the vertices table,
indexes, etc. They may use :doc:`pgr_extractVertices` for that purpose.

.. literalinclude:: sampledata.queries
   :start-after: -- q1
   :end-before: -- q1-1

Migration of ``pgr_analyzeOneWay``
-------------------------------------------------------------------------------

:Deprecated: `v3.8.0 <https://docs.pgrouting.org/3.8>`__
:Removed: `v4.0.0 <https://docs.pgrouting.org/4.0>`__

**Before Deprecation:** The following was calculated:

* Number of potential problems in directionality

WHERE

Directionality problems were calculated based on codes.

.. rubric:: Dead ends.

A routing problem can arise when from a vertex there is only a way on or a way
out but not both:

Either saving or using directly :doc:`pgr_extractVertices` get the dead ends
information and determine if the adjacent edge is one way or not.

In this example :doc:`pgr_extractVertices` has already been applied.

.. literalinclude:: migration.queries
   :start-after: --OneWay1
   :end-before: --OneWay2

.. rubric:: Bridges.

Another routing problem can arise when there is an edge of an undirected graph
whose deletion increases its number of connected components, and the bridge is
only one way.

To determine if the bridges are or not one way.

.. literalinclude:: migration.queries
   :start-after: --OneWay2
   :end-before: --OneWay3

Migration of ``pgr_analyzeGraph``
-------------------------------------------------------------------------------

:Deprecated: `v3.8.0 <https://docs.pgrouting.org/3.8>`__
:Removed: `v4.0.0 <https://docs.pgrouting.org/4.0>`__

**Before Deprecation:** The following was calculated:

* Number of isolated segments.
* Number of dead ends.
* Number of potential gaps found near dead ends.
* Number of intersections. (between 2 edges)

WHERE

:Graph component: A connected subgraph that is not part of any larger connected
                  subgraph.
:Isolated segment: A graph component with only one segment.
:Dead ends: A vertex that participates in only one edge.
:gaps: Space between two geometries.
:Intersection: Is a topological relationship between two geometries.

.. rubric:: Migration.

.. rubric:: Components.

Instead of counting only isolated segments, determine all the components of the
graph.

Depending of the final application requirements use:

* :doc:`pgr_connectedComponents`
* :doc:`pgr_strongComponents`
* :doc:`pgr_biconnectedComponents`

For example:

.. literalinclude:: migration.queries
   :start-after: --analysis1
   :end-before: --analysis2

.. rubric:: Dead ends.

Instead of counting the dead ends, determine all the dead ends of the graph
using :doc:`pgr_degree`.

For example:

.. literalinclude:: migration.queries
   :start-after: --analysis2
   :end-before: --analysis3

.. rubric:: Potential gaps near dead ends.

Instead of counting potential gaps between geometries, determine the geometric
gaps in the graph using :doc:`pgr_findCloseEdges`.

For example:

.. literalinclude:: migration.queries
   :start-after: --analysis3
   :end-before: --analysis4

.. rubric::  Topological relationships.

Instead of counting intersections, determine topological relationships between
geometries.

Several PostGIS functions can be used:
`ST_Intersects <https://postgis.net/docs/ST_Intersects.html>`__,
`ST_Crosses <https://postgis.net/docs/ST_Crosses.html>`__,
`ST_Overlaps <https://postgis.net/docs/ST_Overlaps.html>`__, etc.

For example:

.. literalinclude:: migration.queries
   :start-after: --analysis4
   :end-before: --analysis5

Migration of ``pgr_trsp`` (Vertices)
-------------------------------------------------------------------------------

Signature:

.. parsed-literal::

   pgr_trsp(Edges SQL, source, target, directed boolean, has_rcost boolean
           [,restrict_sql text]);
   RETURNS SETOF (seq, id1, id2, cost)

:Deprecated: `v3.4.0 <https://docs.pgrouting.org/3.4>`__
:Removed: `v4.0.0 <https://docs.pgrouting.org/4.0>`__

.. contents::
   :local:

.. rubric:: See Also

- :doc:`pgr_dijkstra`
- :doc:`pgr_trsp`
- `Migration of restrictions`_

Use ``pgr_dijkstra`` when there are no restrictions.
...............................................................................

Use :doc:`pgr_dijkstra` instead.

.. literalinclude:: migration.queries
   :start-after: --verticesv2
   :end-before: --verticesv3

To get the original column names:

.. literalinclude:: migration.queries
   :start-after: --verticesv3
   :end-before: --verticesv4

* ``id1`` is the node
* ``id2`` is the edge

Use ``pgr_trsp`` when there are restrictions.
...............................................................................

Use :doc:`pgr_trsp` (One to One) instead.

.. literalinclude:: migration.queries
   :start-after: --verticesv5
   :end-before: --verticesv6

To get the original column names:

.. literalinclude:: migration.queries
   :start-after: --verticesv6
   :end-before: --verticesv7

* ``id1`` is the node
* ``id2`` is the edge

Migration of ``pgr_trsp`` (Edges)
-------------------------------------------------------------------------------

Signature:

.. parsed-literal::

   pgr_trsp(sql text, source_edge integer, source_pos float8,
                    target_edge integer, target_pos float8,
                    directed boolean, has_rcost boolean
                    [,restrict_sql text]);
   RETURNS SETOF (seq, id1, id2, cost)

:Deprecated: `v3.4.0 <https://docs.pgrouting.org/3.4>`__
:Removed: `v4.0.0 <https://docs.pgrouting.org/4.0>`__

.. contents::
   :local:

.. rubric:: See Also

- :doc:`pgr_withPoints`
- :doc:`pgr_trsp_withPoints`
- `Migration of restrictions`_

Migration of ``pgr_trspViaVertices``
-------------------------------------------------------------------------------

Signature:

.. parsed-literal::

   pgr_trspViaVertices(sql text, vids integer[],
                     directed boolean, has_rcost boolean
                     [, turn_restrict_sql text]);
   RETURNS SETOF (seq, id1, id2, id3, cost)

:Deprecated: `v3.4.0 <https://docs.pgrouting.org/3.4>`__
:Removed: `v4.0.0 <https://docs.pgrouting.org/4.0>`__

.. contents::
   :local:
   :depth: 1

.. rubric:: See Also

- :doc:`pgr_dijkstraVia`
- :doc:`pgr_trspVia`
- `Migration of restrictions`_

Use ``pgr_dijkstraVia`` when there are no restrictions
...............................................................................

Use :doc:`pgr_dijkstraVia` instead.

.. literalinclude:: migration.queries
   :start-after: --viav2
   :end-before: --viav3

To get the original column names:

.. literalinclude:: migration.queries
   :start-after: --viav3
   :end-before: --viav4

* ``id1`` is the path identifier
* ``id2`` is the node
* ``id3`` is the edge

Use ``pgr_trspVia`` when there are restrictions
...............................................................................

Use :doc:`pgr_trspVia` instead.

.. literalinclude:: migration.queries
   :start-after: --viav5
   :end-before: --viav6

To get the original column names:

.. literalinclude:: migration.queries
   :start-after: --viav6
   :end-before: --viav7

* ``id1`` is the path identifier
* ``id2`` is the node
* ``id3`` is the edge

Migration of ``pgr_trspViaEdges``
-------------------------------------------------------------------------------

Signature:

.. parsed-literal::

   pgr_trspViaEdges(sql text, eids integer[], pcts float8[],
                     directed boolean, has_rcost boolean
                     [, turn_restrict_sql text]);
   RETURNS SETOF (seq, id1, id2, id3, cost)

:Deprecated: `v3.4.0 <https://docs.pgrouting.org/3.4>`__
:Removed: `v4.0.0 <https://docs.pgrouting.org/4.0>`__

.. contents::
   :local:
   :depth: 1

.. rubric:: See Also

- :doc:`pgr_withPointsVia`
- :doc:`pgr_trspVia_withPoints`
- `Migration of restrictions`_

Use ``pgr_withPointsVia`` when there are no restrictions
...............................................................................

Use :doc:`pgr_withPointsVia` instead.

.. literalinclude:: migration.queries
   :start-after: --edgesvia2
   :end-before: --edgesvia3

To get the original column names:

.. literalinclude:: migration.queries
   :start-after: --edgesvia3
   :end-before: --edgesvia4

* ``id1`` is the path identifier
* ``id2`` is the node
* ``id3`` is the edge

Use ``pgr_trspVia_withPoints`` when there are restrictions
...............................................................................

Use :doc:`pgr_trspVia_withPoints` instead.

.. literalinclude:: migration.queries
   :start-after: --edgesvia5
   :end-before: --edgesvia6

To get the original column names:

.. literalinclude:: migration.queries
   :start-after: --edgesvia6
   :end-before: --edgesvia7

* ``id1`` is the path identifier
* ``id2`` is the node
* ``id3`` is the edge

Not yet classified migrations
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Migration of ``pgr_KSP``
-------------------------------------------------------------------------------

Starting from `v3.6.0 <https://docs.pgrouting.org/3.6/en/migration.html>`__
:doc:`pgr_KSP` result columns are being standardized.

:from: |ksp-result|
:to: |generic-result|

Signatures to be migrated:

* ``pgr_KSP`` (One to One)

:Before Migration:

* Output columns were |ksp-result|

  * the columns ``start_vid`` and ``end_vid`` do not exist.

    * ``pgr_KSP`` (One to One) does not have ``start_vid`` and ``end_vid``.

:Migration:

* Be aware of the existence of the additional columns.
* If needed filter out the added columns, for example, to return the original
  columns.

``pgr_KSP`` (One to One)
...............................................................................

Using
`this <https://docs.pgrouting.org/3.5/en/pgr_KSP.html#signatures>`__
example.

* ``start_vid`` contains the **start vid** parameter value.
* ``end_vid`` contains the **end vid** parameter value.

.. literalinclude:: migration.queries
   :start-after: --ksp1
   :end-before: --ksp2

If needed filter out the added columns, for example, to return the original
columns:

.. literalinclude:: migration.queries
   :start-after: --ksp2
   :end-before: --ksp3

Migration of ``pgr_withPointsKSP``
-------------------------------------------------------------------------------

Starting from `v3.6.0 <https://docs.pgrouting.org/3.6/en/migration.html>`__
:doc:`pgr_withPointsKSP` result columns are being standardized.

:from: |ksp-result|
:to: |generic-result|

And ``driving side`` parameter changed from named optional to unnamed compulsory
**driving side** and its validity differ for directed and undirected graphs.

Signatures to be migrated:

* ``pgr_withPointsKSP`` (`One to One`)

:Before Migration:

* Output columns were |old-pid-result|

  * the columns ``start_vid`` and ``end_vid`` do not exist.


:Migration:

* Be aware of the existence of the additional result Columns.
* New output columns are |generic-result|
* **driving side** parameter is unnamed compulsory, and valid values differ for
  directed and undirected graphs.

  * Does not have a default value.
  * In directed graph: valid values are [``r``, ``R``, ``l``, ``L``]
  * In undirected graph: valid values are [``b``, ``B``]
  * Using an invalid value throws an ``ERROR``.

``pgr_withPointsKSP`` (`One to One`)
...............................................................................

Using
`this <https://docs.pgrouting.org/3.5/en/pgr_withPointsKSP.html#signatures>`__
example.

* ``start_vid`` contains the **start vid** parameter value.
* ``end_vid`` contains the **end vid** parameter value.

.. literalinclude:: migration.queries
   :start-after: --withPointsKSP1
   :end-before: --withPointsKSP2

If needed filter out the additional columns, for example, to return the original
columns:

.. literalinclude:: migration.queries
   :start-after: --withPointsKSP2
   :end-before: --withPointsKSP3


Use ``pgr_withPoints`` when there are no restrictions.
...............................................................................

Use :doc:`pgr_withPoints` (One to One) instead.

.. literalinclude:: migration.queries
   :start-after: --edgesv2
   :end-before: --edgesv3

To get the original column names:

.. literalinclude:: migration.queries
   :start-after: --edgesv3
   :end-before: --edgesv4

* ``id1`` is the node
* ``id2`` is the edge

Use ``pgr_trsp_withPoints`` when there are restrictions.
...............................................................................

Use :doc:`pgr_trsp_withPoints` instead.

.. literalinclude:: migration.queries
   :start-after: --edgesv5
   :end-before: --edgesv6

To get the original column names:

.. literalinclude:: migration.queries
   :start-after: --edgesv6
   :end-before: --edgesv7

* ``id1`` is the node
* ``id2`` is the edge

Migration of ``pgr_maxCardinalityMatch``
-------------------------------------------------------------------------------

:doc:`pgr_maxCardinalityMatch` works only for undirected graphs, therefore the
``directed`` flag has been removed.

Starting from `v3.4.0 <https://docs.pgrouting.org/3.4/en/migration.html>`__

Signature to be migrated:

.. parsed-literal::

  pgr_maxCardinalityMatch(Edges SQL, [directed])
   RETURNS SETOF (seq, edge, source, target)

Migration is needed, because:

* Use ``cost`` and ``reverse_cost`` on the inner query
* Results are ordered
* Works for undirected graphs.
* New signature

  * ``pgr_maxCardinalityMatch(text)`` returns only ``edge`` column.
  * The optional flag ``directed`` is removed.

:Before migration:

* Columns used are ``going`` and ``coming`` to represent the existence of an
  edge.
* Flag ``directed`` was used to indicate if it was for a **directed** or
  **undirected** graph.

  * The flag ``directed`` is ignored.

    * Regardless of it's value it gives the result considering the graph as
      **undirected**.

:Migration:

* Use the columns ``cost`` and ``reverse_cost`` to represent the existence of an
  edge.
* Do not use the flag ``directed``.
* In the query returns only ``edge`` column.

.. literalinclude:: migration.queries
   :start-after: --maxcard2
   :end-before: --maxcard3

Migration of restrictions
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Starting from `v3.4.0 <https://docs.pgrouting.org/3.4/en/migration.html>`__

The structure of the restrictions have changed:

Old restrictions structure
-------------------------------------------------------------------------------

On the deprecated signatures:

* Column ``rid`` is ignored
* ``via_path``

  * Must be in reverse order.
  * Is of type ``TEXT``.
  * When more than one via edge must be separated with ``,``.

* ``target_id``

  * Is the last edge of the forbidden path.
  * Is of type ``INTEGER``.

* ``to_cost``

  * Is of type ``FLOAT``.

Creation of the old restrictions table

.. literalinclude:: migration.queries
   :start-after: --rest00
   :end-before: --rest01

Old restrictions fill up

.. literalinclude:: migration.queries
   :start-after: --rest01
   :end-before: --rest1

Old restrictions contents
...............................................................................

.. literalinclude:: migration.queries
   :start-after: --rest1
   :end-before: --rest2

The restriction with ``rid = 2`` is representing :math:`3 \rightarrow 5
\rightarrow9`

* :math:`3\rightarrow5`

  * is on column ``via_path`` in reverse order
  * is of type ``TEXT``

* :math:`9`

  * is on column ``target_id``
  * is of type ``INTEGER``


New restrictions structure
-------------------------------------------------------------------------------

* Column ``id`` is ignored
* Column ``path``

  * Is of type ``ARRAY[ANY-INTEGER]``.
  * Contains all the edges involved on the restriction.
  * The array has the ordered edges of the restriction.

* Column ``cost``

  * Is of type ``ANY-NUMERICAL``

.. include:: sampledata.rst
   :start-after: the_restrictions_start
   :end-before: the_restrictions_end


The restriction with ``rid = 2`` represents the path :math:`3 \rightarrow5
\rightarrow9`.

* By inspection the path is clear.

Migration
...............................................................................

To transform the old restrictions table to the new restrictions structure,

* Create a new table with the new restrictions structure.

  * In this migration guide ``new_restrictions`` is been used.

* For this migration pgRouting supplies an auxiliary function for reversal of an
  array ``_pgr_array_reverse`` needed for the migration.

  * ``_pgr_array_reverse``:

    * Was created temporally for this migration
    * Is not documented.
    * Will be removed on the next mayor version 4.0.0

.. literalinclude:: migration.queries
   :start-after: --rest3
   :end-before: --rest4

The migrated table contents:

.. literalinclude:: migration.queries
   :start-after: --rest4
   :end-before: --rest5


See Also
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

* :doc:`pgRouting-concepts`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
