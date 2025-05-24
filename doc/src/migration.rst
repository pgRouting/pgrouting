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
   :depth: 2

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

Migration of ``pgr_aStar``
-------------------------------------------------------------------------------

Starting from `v3.6.0 <https://docs.pgrouting.org/3.6/en/migration.html>`__

Signatures to be migrated:

* ``pgr_aStar`` (`One to One`)
* ``pgr_aStar`` (`One to Many`)
* ``pgr_aStar`` (`Many to One`)

.. rubric:: Before Migration

* Output columns were |old-generic-result|

  * Depending on the overload used, the columns ``start_vid`` and ``end_vid``
    might be missing:

    * ``pgr_aStar`` (`One to One`) does not have ``start_vid`` and ``end_vid``.
    * ``pgr_aStar`` (`One to Many`) does not have ``start_vid``.
    * ``pgr_aStar`` (`Many to One`) does not have ``end_vid``.

:Migration:

* Be aware of the existence of the additional columns.

* In ``pgr_aStar`` (`One to One`)

  * ``start_vid`` contains the **start vid** parameter value.
  * ``end_vid`` contains the **end vid** parameter value.

.. literalinclude:: migration.queries
   :start-after: --astar1
   :end-before: --astar2

* In ``pgr_aStar`` (`One to Many`)

  * ``start_vid`` contains the **start vid** parameter value.

.. literalinclude:: migration.queries
   :start-after: --astar2
   :end-before: --astar3

* In ``pgr_aStar`` (`Many to One`)

  * ``end_vid`` contains the **end vid** parameter value.

.. literalinclude:: migration.queries
   :start-after: --astar3
   :end-before: --astar4

* If needed filter out the added columns, for example:

.. literalinclude:: migration.queries
   :start-after: --astar4
   :end-before: --astar5

* If needed add the new columns, similar to the following example where
  ``pgr_dijkstra`` is used, and the function had to be modified to be able to
  return the new columns:

  * In `v3.0
    <https://docs.pgrouting.org/3.0/en/contraction-family.html#case-1-both-source-and-target-belong-to-the-contracted-graph>`__
    the function ``my_dijkstra`` uses ``pgr_dijkstra``.
  * Starting from `v3.5
    <https://docs.pgrouting.org/3.5/en/contraction-family.html#case-1-both-source-and-target-belong-to-the-contracted-graph>`__
    the function ``my_dijkstra`` returns the new additional columns of
    ``pgr_dijkstra``.

Migration of ``pgr_bdAstar``
-------------------------------------------------------------------------------

Starting from `v3.6.0 <https://docs.pgrouting.org/3.6/en/migration.html>`__

Signatures to be migrated:

* ``pgr_bdAstar`` (`One to One`)
* ``pgr_bdAstar`` (`One to Many`)
* ``pgr_bdAstar`` (`Many to One`)

:Before Migration:

* Output columns were |old-generic-result|

  * Depending on the overload used, the columns ``start_vid`` and ``end_vid``
    might be missing:

    * ``pgr_bdAstar`` (`One to One`) does not have ``start_vid`` and ``end_vid``.
    * ``pgr_bdAstar`` (`One to Many`) does not have ``start_vid``.
    * ``pgr_bdAstar`` (`Many to One`) does not have ``end_vid``.

:Migration:

* Be aware of the existence of the additional columns.

* In ``pgr_bdAstar`` (`One to One`)

  * ``start_vid`` contains the **start vid** parameter value.
  * ``end_vid`` contains the **end vid** parameter value.

.. literalinclude:: migration.queries
   :start-after: --bdastar1
   :end-before: --bdastar2

* In ``pgr_bdAstar`` (`One to Many`)

  * ``start_vid`` contains the **start vid** parameter value.

.. literalinclude:: migration.queries
   :start-after: --bdastar2
   :end-before: --bdastar3

* In ``pgr_bdAstar`` (`Many to One`)

  * ``end_vid`` contains the **end vid** parameter value.

.. literalinclude:: migration.queries
   :start-after: --bdastar3
   :end-before: --bdastar4

* If needed filter out the added columns, for example:

.. literalinclude:: migration.queries
   :start-after: --bdastar4
   :end-before: --bdastar5

* If needed add the new columns, similar to the following example where
  ``pgr_dijkstra`` is used, and the function had to be modified to be able to
  return the new columns:

  * In `v3.0 <https://docs.pgrouting.org/3.0/en/contraction-family.html#case-1-both-source-and-target-belong-to-the-contracted-graph>`__
    the function ``my_dijkstra`` uses ``pgr_dijkstra``.
  * Starting from `v3.5 <https://docs.pgrouting.org/3.5/en/contraction-family.html#case-1-both-source-and-target-belong-to-the-contracted-graph>`__
    the function ``my_dijkstra`` returns the new additional columns of
    ``pgr_dijkstra``.


Migration of ``pgr_dijkstra``
-------------------------------------------------------------------------------

Starting from `v3.5.0 <https://docs.pgrouting.org/3.5/en/migration.html>`__

Signatures to be migrated:

* ``pgr_dijkstra`` (`One to One`)
* ``pgr_dijkstra`` (`One to Many`)
* ``pgr_dijkstra`` (`Many to One`)

:Before Migration:

* Output columns were |old-generic-result|

  * Depending on the overload used, the columns ``start_vid`` and ``end_vid``
    might be missing:

    * ``pgr_dijkstra`` (`One to One`) does not have ``start_vid`` and
      ``end_vid``.
    * ``pgr_dijkstra`` (`One to Many`) does not have ``start_vid``.
    * ``pgr_dijkstra`` (`Many to One`) does not have ``end_vid``.

:Migration:

* Be aware of the existence of the additional columns.

* In ``pgr_dijkstra`` (`One to One`)

  * ``start_vid`` contains the **start vid** parameter value.
  * ``end_vid`` contains the **end vid** parameter value.

.. literalinclude:: migration.queries
   :start-after: --dijkstra1
   :end-before: --dijkstra2

* In ``pgr_dijkstra`` (`One to Many`)

  * ``start_vid`` contains the **start vid** parameter value.

.. literalinclude:: migration.queries
   :start-after: --dijkstra2
   :end-before: --dijkstra3

* In ``pgr_dijkstra`` (`Many to One`)

  * ``end_vid`` contains the **end vid** parameter value.

.. literalinclude:: migration.queries
   :start-after: --dijkstra3
   :end-before: --dijkstra4

* If needed filter out the added columns, for example:

.. literalinclude:: migration.queries
   :start-after: --dijkstra4
   :end-before: --dijkstra5

* If needed add the new columns, for example:

  * In `v3.0 <https://docs.pgrouting.org/3.0/en/contraction-family.html#case-1-both-source-and-target-belong-to-the-contracted-graph>`__
    the function ``my_dijkstra`` uses ``pgr_dijkstra``.
  * Starting from `v3.5 <https://docs.pgrouting.org/3.5/en/contraction-family.html#case-1-both-source-and-target-belong-to-the-contracted-graph>`__
    the function ``my_dijkstra`` returns the new additional columns of
    ``pgr_dijkstra``.

Migration of ``pgr_drivingDistance``
-------------------------------------------------------------------------------

Starting from `v3.6.0 <https://docs.pgrouting.org/3.6/en/migration.html>`__
:doc:`pgr_drivingDistance` result columns are being standardized.

:from: |result-dij-dd|
:to: |result-spantree|

Signatures to be migrated:

* pgr_drivingDistance(Single vertex)
* pgr_drivingDistance(Multiple vertices)

:Before Migration:

Output columns were |result-dij-dd|

* pgr_drivingDistance(Single vertex)

  * Does not have ``start_vid`` and ``depth`` result columns.

* pgr_drivingDistance(Multiple vertices)

  * Has ``from_v`` instead of ``start_vid`` result column.
  * does not have ``depth`` result column.

:Migration:

* Be aware of the existence and name change of the result columns.

pgr_drivingDistance(Single vertex)
...............................................................................

Using `this
<https://docs.pgrouting.org/3.5/en/pgr_drivingDistance.html#single-vertex>`__
example.

* ``start_vid`` contains the **start vid** parameter value.
* ``depth`` contains the depth of the ``node``.
* ``pred`` contains the predecessor of the ``node``.

  .. literalinclude:: migration.queries
     :start-after: --drivingdistance1
     :end-before: --drivingdistance2

If needed filter out the added columns, for example, to return the original columns

.. literalinclude:: migration.queries
   :start-after: --drivingdistance2
   :end-before: --drivingdistance3

pgr_drivingDistance(Multiple vertices)
...............................................................................

Using `this
<https://docs.pgrouting.org/3.5/en/pgr_drivingDistance.html#multiple-vertices>`__
example.

* The ``from_v`` result column name changes to ``start_vid``.
* ``depth`` contains the depth of the ``node``.
* ``pred`` contains the predecessor of the ``node``.

  .. literalinclude:: migration.queries
     :start-after: --drivingdistance3
     :end-before: --drivingdistance4

If needed filter out and rename columns, for example, to return the original
columns:

.. literalinclude:: migration.queries
   :start-after: --drivingdistance4
   :end-before: --drivingdistance5

Migration of ``pgr_kruskalDD`` / ``pgr_kruskalBFS`` / ``pgr_kruskalDFS``
-------------------------------------------------------------------------------

Starting from `v3.7.0 <https://docs.pgrouting.org/3.7/en/migration.html>`__
:doc:`pgr_kruskalDD`, :doc:`pgr_kruskalBFS` and
:doc:`pgr_kruskalDFS` result columns are being standardized.

:from: |result-bfs|
:to: |result-spantree|

* ``pgr_kruskalDD``

  * Single vertex
  * Multiple vertices

* ``pgr_kruskalDFS``

  * Single vertex
  * Multiple vertices

* ``pgr_kruskalBFS``

  * Single vertex
  * Multiple vertices


:Before Migration:

Output columns were |result-bfs|

* Single vertex and Multiple vertices

  * Do not have ``pred`` result column.

:Migration:

* Be aware of the existence of `pred` result columns.
* If needed filter out the added columns

Kruskal single vertex
...............................................................................

Using ``pgr_KruskalDD`` as example.
Migration is similar to al the affected functions.

Comparing with `this
<https://docs.pgrouting.org/3.6/en/pgr_kruskalDD.html#single-vertex>`__ example.

Now column ``pred`` exists and contains the predecessor of the ``node``.

.. literalinclude:: migration.queries
   :start-after: --kruskalDD1
   :end-before: --kruskalDD2

If needed filter out the added columns, for example, to return the original
columns

.. literalinclude:: migration.queries
   :start-after: --kruskalDD2
   :end-before: --kruskalDD3

Kruskal multiple vertices
...............................................................................

Using ``pgr_KruskalDD`` as example.
Migration is similar to al the affected functions.

Comparing with `this
<https://docs.pgrouting.org/3.6/en/pgr_kruskalDD.html#multiple-vertex>`__
example.

Now column ``pred`` exists and contains the predecessor of the ``node``.

.. literalinclude:: migration.queries
   :start-after: --kruskalDD3
   :end-before: --kruskalDD4

If needed filter out the added columns, for example, to return the original
columns

.. literalinclude:: migration.queries
   :start-after: --kruskalDD4
   :end-before: --kruskalDD5

Migration of ``pgr_KSP``
-------------------------------------------------------------------------------

Starting from `v3.6.0 <https://docs.pgrouting.org/3.6/en/migration.html>`__
:doc:`pgr_KSP` result columns are being standardized.

:from: |ksp-result|
:from: |nksp-result|

Signatures to be migrated:

* ``pgr_KSP`` (One to One)

:Before Migration:

* Output columns were |ksp-result|

  * the columns ``start_vid`` and ``end_vid`` do not exist.

    * ``pgr_KSP`` (One to One) does not have ``start_vid`` and ``end_vid``.

:Migration:

* Be aware of the existence of the additional columns.

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

Migration of ``pgr_primDD`` / ``pgr_primBFS`` / ``pgr_primDFS``
-------------------------------------------------------------------------------

Starting from `v3.7.0 <https://docs.pgrouting.org/3.7/en/migration.html>`__
:doc:`pgr_primDD`, :doc:`pgr_primBFS` and :doc:`pgr_primDFS` result columns are
being standardized.

:from: |result-bfs|
:to: |result-spantree|

* ``pgr_primDD``

  * Single vertex
  * Multiple vertices

* ``pgr_primDFS``

  * Single vertex
  * Multiple vertices

* ``pgr_primBFS``

  * Single vertex
  * Multiple vertices


:Before Migration:

Output columns were |result-bfs|

* Single vertex and Multiple vertices

  * Do not have ``pred`` result column.

:Migration:

* Be aware of the existence of `pred` result columns.
* If needed filter out the added columns

Prim single vertex
...............................................................................

Using ``pgr_primDD`` as example.
Migration is similar to al the affected functions.

Comparing with `this
<https://docs.pgrouting.org/3.6/en/pgr_primDD.html#single-vertex>`__ example.

Now column ``pred`` exists and contains the predecessor of the ``node``.

.. literalinclude:: migration.queries
   :start-after: --primDD1
   :end-before: --primDD2

If needed filter out the added columns, for example, to return the original
columns

.. literalinclude:: migration.queries
   :start-after: --primDD2
   :end-before: --primDD3

Prim multiple vertices
...............................................................................

Using ``pgr_primDD`` as example.
Migration is similar to al the affected functions.

Comparing with `this
<https://docs.pgrouting.org/3.6/en/pgr_primDD.html#multiple-vertex>`__
example.

Now column ``pred`` exists and contains the predecessor of the ``node``.

.. literalinclude:: migration.queries
   :start-after: --primDD3
   :end-before: --primDD4

If needed filter out the added columns, for example, to return the original
columns

.. literalinclude:: migration.queries
   :start-after: --primDD4
   :end-before: --primDD5

Migration of ``pgr_withPointsDD``
-------------------------------------------------------------------------------

Starting from `v3.6.0 <https://docs.pgrouting.org/3.6/en/migration.html>`__
:doc:`pgr_withPointsDD` result columns are being standardized.

:from: |result-generic-no-seq|
:to: |result-spantree|

And ``driving_side`` parameter changed from named optional to unnamed compulsory
**driving side** and its validity differ for directed and undirected graphs.

Signatures to be migrated:

* ``pgr_withPointsDD`` (Single vertex)
* ``pgr_withPointsDD`` (Multiple vertices)

.. rubric:: Before Migration

* ``pgr_withPointsDD`` (Single vertex)

  * Output columns were |result-1-1-no-seq|
  * Does not have ``start_vid``, ``pred`` and ``depth`` result columns.
  * ``driving_side`` parameter was named optional now it is compulsory unnamed.

* ``pgr_withPointsDD`` (`Multiple vertices`)

  * Output columns were |result-m-1-no-seq|
  * Does not have ``depth`` and ``pred`` result columns.
  * ``driving_side`` parameter was named optional now it is compulsory unnamed.

* Validity of driving_side:

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

``pgr_withPointsDD`` (Single vertex)
...............................................................................

Using
`this <https://docs.pgrouting.org/3.5/en/pgr_withPointsDD.html#single-vertex>`__
example.

To migrate, use an unnamed valid value for **driving side** after the
**distance** parameter:

.. literalinclude:: migration.queries
   :start-after: --withpointsdd4
   :end-before: --withpointsdd5

To get results from previous versions:

* Filter out the additional columns

.. literalinclude:: migration.queries
   :start-after: --withpointsdd5
   :end-before: --withpointsdd6

``pgr_withPointsDD`` (Multiple vertices)
...............................................................................

Using
`this <https://docs.pgrouting.org/3.5/en/pgr_withPointsDD.html#multiple-vertices>`__
example.

To migrate, use an unnamed valid value for **driving side** after the
**distance** parameter:

.. literalinclude:: migration.queries
   :start-after: --withpointsdd6
   :end-before: --withpointsdd7

To get results from previous versions:

* Filter out the additional columns
* When ``details => false``, the default, remove the points by using
  ``WHERE node >= 0 OR cost = 0``

.. literalinclude:: migration.queries
   :start-after: --withpointsdd7
   :end-before: --withpointsdd8

Migration of ``pgr_withPointsKSP``
-------------------------------------------------------------------------------

Starting from `v3.6.0 <https://docs.pgrouting.org/3.6/en/migration.html>`__
:doc:`pgr_withPointsKSP` result columns are being standardized.

:from: |ksp-result|
:from: |nksp-result|

And ``driving side`` parameter changed from named optional to unnamed compulsory
**driving side** and its validity differ for directed and undirected graphs.

Signatures to be migrated:

* ``pgr_withPointsKSP`` (`One to One`)

:Before Migration:

* Output columns were |old-pid-result|

  * the columns ``start_vid`` and ``end_vid`` do not exist.


:Migration:

* Be aware of the existence of the additional result Columns.
* New output columns are |nksp-result|
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

Migration of restrictions
-------------------------------------------------------------------------------

Starting from `v3.4.0 <https://docs.pgrouting.org/3.4/en/migration.html>`__

The structure of the restrictions have changed:

Old restrictions structure
...............................................................................

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
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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
...............................................................................

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
-------------------------------------------------------------------------------

* :doc:`TRSP-family`
* :doc:`withPoints-category`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
