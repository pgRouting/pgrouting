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

For the combination of points given,
modify the graph to include points and 
using Dijkstra algorithm, returns only the aggregate cost of the shortest path(s) found.


Signature Summary
-----------------

.. code-block:: none

    pgr_withPointsCost(edges_sql, points_sql, start_pid, end_pid)
    pgr_withPointsCost(edges_sql, points_sql, start_pid, end_pid, directed, driving_side)
    pgr_withPointsCost(edges_sql, points_sql, start_pid, end_pids, directed, driving_side)
    pgr_withPointsCost(edges_sql, points_sql, start_pids, end_pid, directed, driving_side)
    pgr_withPointsCost(edges_sql, points_sql, start_pids, end_pids, directed, driving_side)
    RETURNS SET OF (start_pid, end_pid, agg_cost)


Signatures
==========

.. index::
    single: withPoints(edges_sql, points_sql, start_pid, end_pid) -- proposed

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
    single: withPoints(edges_sql, points_sql, start_pid, end_pid, directed, driving_side) -- proposed

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
    single: withPoints(edges_sql, points_sql, start_pid, end_pids, directed, driving_side) -- proposed

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
    single: withPoints(edges_sql, points_sql, start_pids, end_pid, directed, driving_side) -- proposed

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
    single: withPoints(edges_sql, points_sql, start_pid, end_pid, directed, driving_side) -- proposed

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

:Example: Which path (if any) passes in front of point 4 or vertex 4 with **right** side driving topology.

.. literalinclude:: doc-pgr_withPointsCost.queries
   :start-after: --q2
   :end-before: --q3

:Example: Which path (if any) passes in front of point 4 or vertex 4 with **left** side driving topology.

.. literalinclude:: doc-pgr_withPointsCost.queries
   :start-after: --q3
   :end-before: --q4

:Example: Many to many example with a twist: on undirected graph.

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

