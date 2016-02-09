.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_withPoints:

pgr_withPoints
===============================================================================


Name
-------------------------------------------------------------------------------

``pgr_withPoints`` - Returns the driving distance from a starting point.

.. note::  This is a proposed function for version 2.3.

     - Is not officially in the version 2.2 release.


.. figure:: ../../../doc/src/introduction/images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph

   Boost Graph Inside

Synopsis
-------------------------------------------------------------------------------

Modify the graph to include points and 
using Dijkstra algorithm, extracts all the nodes and points that have costs less
than or equal to the value ``distance`` from the starting point.
The edges extracted will conform the corresponding spanning tree.


Signature Summary
-----------------

.. code-block:: none

    pgr_withPoints(edges_sql, points_sql, start_pid, end_pid)
    pgr_withPoints(edges_sql, points_sql, start_pid, end_pid, directed, driving_side, details)
    pgr_withPoints(edges_sql, points_sql, start_pid, end_pids, directed, driving_side, details)
    pgr_withPoints(edges_sql, points_sql, start_pids, end_pid, directed, driving_side, details)
    pgr_withPoints(edges_sql, points_sql, start_pids, end_pids, directed, driving_side, details)
    RETURNS SET OF (seq, [path_seq,] [start_pid,] [end_pid,] node, edge, cost, agg_cost)


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

    pgr_withPoints(edges_sql, points_sql, start_pid, end_pid)
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)


:Example:

.. literalinclude:: doc-pgr_withPoints.queries
   :start-after: --e1
   :end-before: --e2

.. index::
    single: withPoints(edges_sql, points_sql, start_pid, end_pid, directed, driving_side, details) -- proposed

One to One
----------


.. code-block:: none

    pgr_withPoints(edges_sql, points_sql, start_pid, end_pid, directed, driving_side, details)
    RETURNS SET OF (seq, node, edge, cost, agg_cost)


:Example:

.. literalinclude:: doc-pgr_withPoints.queries
   :start-after: --e2
   :end-before: --e3

.. index::
    single: withPoints(edges_sql, points_sql, start_pid, end_pids, directed, driving_side, details) -- proposed

One to Many
-----------


.. code-block:: none

    pgr_withPoints(edges_sql, points_sql, start_pid, end_pid, directed, driving_side, details)
    RETURNS SET OF (seq, path_seq, end_pid, node, edge, cost, agg_cost)


:Example:

.. literalinclude:: doc-pgr_withPoints.queries
   :start-after: --e3
   :end-before: --e4

.. index::
    single: withPoints(edges_sql, points_sql, start_pids, end_pid, directed, driving_side, details) -- proposed

Many to One
-----------


.. code-block:: none

    pgr_withPoints(edges_sql, points_sql, start_pids, end_pid, directed, driving_side, details)
    RETURNS SET OF (seq, path_seq, start_pid, node, edge, cost, agg_cost)


:Example:

.. literalinclude:: doc-pgr_withPoints.queries
   :start-after: --e4
   :end-before: --e5

.. index::
    single: withPoints(edges_sql, points_sql, start_pid, end_pid, directed, driving_side, details) -- proposed

Many to Many
------------


.. code-block:: none

    pgr_withPoints(edges_sql, points_sql, start_pids, end_pids, directed, driving_side, details)
    RETURNS SET OF (seq, path_seq, start_pid, end_pid, node, edge, cost, agg_cost)


:Example:

.. literalinclude:: doc-pgr_withPoints.queries
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
**start_vid**    ``ANY-INTEGER``        Starting vertex identifier. A negative value represents a point pid.
**end_vid**      ``ANY-INTEGER``        Ending point identifier. A negative value represents a point pid.
**start_vids**   ``ARRAY[ANY-INTEGER]`` Array of starting points identifiers. Negative values represents points.
**end_vids**     ``ARRAY[ANY-INTEGER]`` Array of ending points identifiers. Negative values represents  points.
**directed**     ``BOOLEAN``            (optional). When ``false`` the graph is considered as Undirected. Default is ``true`` which considers the graph as Directed.
**driving_side** ``CHAR``               (optional) Value in ['b', 'r', 'l', NULL] indicating if the driving side is:
                                          - In the right or left or
                                          - If it doesn't matter with 'b' or NULL.
                                          - If column not present 'b' is considered.

**details**      ``BOOLEAN``            (optional). When ``true`` the results will include the points in points_sql that are in the path.
                                        Default is ``false`` which ignores other points of the points_sql.
================ ====================== =================================================


Description of the return values
-------------------------------------------------------------------------------

Returns set of ``(seq, [path_seq,] [start_pid,] [end_pid,] node, edge, cost, agg_cost)``

============= =========== =================================================
Column           Type              Description
============= =========== =================================================
**seq**       ``INTEGER`` Row sequence.
**path_seq**  ``INTEGER`` Path sequence that indicates the relative position on the path.
**start_vid** ``BIGINT``  Identifier of the starting vertex. When negative is a point's pid.
**end_vid**   ``BIGINT``  Identifier of the ending point. The negative value of the original point's pid is given.
**node**      ``BIGINT``  Identifier of the node:
                            - A positive value indicates the node is a vertex of edges_sql.
                            - A negative value indicates the node is a point  of points_sql.

**edge**      ``BIGINT``  Identifier of the edge used to arrive to ``node``. ``0`` when the ``node`` is the ``start_vid``.
**cost**      ``FLOAT``   Cost to traverse ``edge``.  from ``node`` to the next   node on the path sequence. -1 for the last row of the path sequence.
**agg_cost**  ``FLOAT``   Aggregate cost from ``start_pid`` to ``node``.
============= =========== =================================================



Examples
--------------------------------------------------------------------------------------

:Example: Which path (if any) passes in front of point 6 or vertex 6 with **right** side driving topology.

.. literalinclude:: doc-pgr_withPoints.queries
   :start-after: --q2
   :end-before: --q3

:Example: Which path (if any) passes in front of point 6 or vertex 6 with **left** side driving topology.

.. literalinclude:: doc-pgr_withPoints.queries
   :start-after: --q3
   :end-before: --q4

:Example: Many to many example with a twist: on undirected graph and showing details.

.. literalinclude:: doc-pgr_withPoints.queries
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

