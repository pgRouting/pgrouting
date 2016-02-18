.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_withPointsCost:

pgr_withPointsCost
===============================================================================


Name
-------------------------------------------------------------------------------

``pgr_withPointsCost`` - Calculates the shortest path and returns only the aggregate cost of the shortest path(s) found, for the combination of points given.

.. note::  This is a proposed function for version 2.3.

     - Is not officially in the version 2.2 release.


.. figure:: ../../../doc/src/introduction/images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph

   Boost Graph Inside

Synopsis
-------------------------------------------------------------------------------

Modify the graph to include points defined by points sql.
Using Dijkstra algorithm, return only the aggregate cost of the shortest path(s) found.

Characteristics:
----------------

The main Characteristics are:
  - It does not return a path.
  - Returns the sum of the costs of the shortest path for pair combination of vertices in the mondified graph.
  - Vertices of the graph are:

    - **positive** when it belongs to the edges_sql
    - **negative** when it belongs to the points_sql

  - Process is done only on edges with positive costs.
  - Values are returned when there is a path.

    - The returned values are in the form of a set of `(start_vid, end_vid, agg_cost)`.

    - When the starting vertex and ending vertex are the same, there is no path.

      - The `agg_cost` int the non included values `(v, v)` is `0`

    - When the starting vertex and ending vertex are the different and there is no path.

      - The `agg_cost` in the non included values `(u, v)` is :math:`\infty`

  - Let be the case the values returned are stored in a table, so the unique index would be the pair:
    `(start_vid, end_vid)`.

  - For undirected graphs, the results are symetric.

    - The  `agg_cost` of `(u, v)` is the same as for `(v, u)`.

  - Any duplicated value in the `start_vids` or `end_vids` is ignored.

  - The returned values are ordered:

    - `start_vid` ascending
    - `end_vid` ascending

  - Runing time: :math:`O(| start\_vids | * (V \log V + E))`


Signature Summary
-----------------

.. code-block:: none

    pgr_withPointsCost(edges_sql, points_sql, start_pid, end_pid)
    pgr_withPointsCost(edges_sql, points_sql, start_pid, end_pid, directed, driving_side)
    pgr_withPointsCost(edges_sql, points_sql, start_pid, end_pids, directed, driving_side)
    pgr_withPointsCost(edges_sql, points_sql, start_pids, end_pid, directed, driving_side)
    pgr_withPointsCost(edges_sql, points_sql, start_pids, end_pids, directed, driving_side)
    RETURNS SET OF (start_pid, end_pid, agg_cost)

.. note:: There is no **details** flag, unlike the other members of the family of functions,  


Signatures
==========

.. index::
    single: withPointsCost(edges_sql, points_sql, start_pid, end_pid) -- proposed

Minimal signature
-----------------

The minimal signature:
    - Is for a **directed** graph.
    - The driving side is set as **b** both. So arriving/departing to/from the point(s) can be in any direction.
    - No **details** are given about distance of other points of the query.

.. code-block:: none

    pgr_withPointsCost(edges_sql, points_sql, start_pid, end_pid)
    RETURNS SET OF (start_pid, end_pid, agg_cost)


:Example:

.. literalinclude:: doc-pgr_withPointsCost.queries
   :start-after: --e1
   :end-before: --e2

.. index::
    single: withPointsCost(edges_sql, points_sql, start_pid, end_pid, directed, driving_side) -- proposed

One to One
----------


.. code-block:: none

    pgr_withPointsCost(edges_sql, points_sql, start_pid, end_pid, directed, driving_side)
    RETURNS SET OF (seq, node, edge, cost, agg_cost)


:Example:

.. literalinclude:: doc-pgr_withPointsCost.queries
   :start-after: --e2
   :end-before: --e3

.. index::
    single: withPointsCost(edges_sql, points_sql, start_pid, end_pids, directed, driving_side) -- proposed

One to Many
-----------


.. code-block:: none

    pgr_withPointsCost(edges_sql, points_sql, start_pid, end_pid, directed, driving_side)
    RETURNS SET OF (start_pid, end_pid, agg_cost)


:Example:

.. literalinclude:: doc-pgr_withPointsCost.queries
   :start-after: --e3
   :end-before: --e4

.. index::
    single: withPointsCost(edges_sql, points_sql, start_pids, end_pid, directed, driving_side) -- proposed

Many to One
-----------


.. code-block:: none

    pgr_withPointsCost(edges_sql, points_sql, start_pids, end_pid, directed, driving_side)
    RETURNS SET OF (start_pid, end_pid, agg_cost)


:Example:

.. literalinclude:: doc-pgr_withPointsCost.queries
   :start-after: --e4
   :end-before: --e5

.. index::
    single: withPointsCost(edges_sql, points_sql, start_pid, end_pid, directed, driving_side) -- proposed

Many to Many
------------


.. code-block:: none

    pgr_withPointsCost(edges_sql, points_sql, start_pids, end_pids, directed, driving_side)
    RETURNS SET OF (start_pid, end_pid, agg_cost)


:Example:

.. literalinclude:: doc-pgr_withPointsCost.queries
   :start-after: --e5
   :end-before: --q2




Description of the Signatures
=============================

..
    description of the sql queries


.. include:: withPoints_queries.txt 


Description of the parameters of the signatures
-------------------------------------------------------------------------------


================ ====================== =================================================
Parameter        Type                   Description
================ ====================== =================================================
**edges_sql**    ``TEXT``               Edges SQL query as decribed above.
**points_sql**   ``TEXT``               Points SQL query as decribed above.
**start_vid**    ``ANY-INTEGER``        Starting vertex identifier. When negative: is a point's pid.
**end_vid**      ``ANY-INTEGER``        Ending point identifier. When negative: is a point's pid.
**start_vids**   ``ARRAY[ANY-INTEGER]`` Array of starting points identifiers. When negative: is a point's pid.
**end_vids**     ``ARRAY[ANY-INTEGER]`` Array of ending points identifiers. When negative: is a point's pid.
**directed**     ``BOOLEAN``            (optional). When ``false`` the graph is considered as Undirected. Default is ``true`` which considers the graph as Directed.
**driving_side** ``CHAR``               (optional) Value in ['b', 'r', 'l', NULL] indicating if the driving side is:
                                          - In the right or left or
                                          - If it doesn't matter with 'b' or NULL.
                                          - If column not present 'b' is considered.

================ ====================== =================================================


Description of the return values
-------------------------------------------------------------------------------

Returns set of ``(start_pid, end_pid, agg_cost)``

============= =========== =================================================
Column           Type              Description
============= =========== =================================================
**start_vid** ``BIGINT``  Identifier of the starting vertex. When negative: is a point's pid.
**end_vid**   ``BIGINT``  Identifier of the ending point. When negative: is a point's pid.
**agg_cost**  ``FLOAT``   Aggregate cost from ``start_pid`` to ``node``.
============= =========== =================================================



Examples
--------------------------------------------------------------------------------------

:Example: With **right** side driving topology.

.. literalinclude:: doc-pgr_withPointsCost.queries
   :start-after: --q2
   :end-before: --q3

:Example: With **left** side driving topology.

.. literalinclude:: doc-pgr_withPointsCost.queries
   :start-after: --q3
   :end-before: --q4

:Example: Does not matter driving side.

.. literalinclude:: doc-pgr_withPointsCost.queries
   :start-after: --q4
   :end-before: --q5


The queries use the :ref:`sampledata` network.



.. rubric:: History

* Proposed in version 2.2


See Also
-------------------------------------------------------------------------------

* :ref:`withPoints`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

