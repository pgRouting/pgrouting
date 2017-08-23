..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_withPointsVia:

pgr_withPointsVia - Proposed
===============================================================================


Name
-------------------------------------------------------------------------------

``pgr_withPointsVia`` - Get a path using the vertices/points indicated


.. include:: proposed.rst
   :start-after: begin-warning
   :end-before: end-warning



.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph

   Boost Graph Inside

.. rubric:: Availability: PENDING.

Synopsis
-------------------------------------------------------------------------------

Modify the graph to include points and
using Dijkstra algorithm, extracts all the nodes and points that have costs less
than or equal to the value ``distance`` from the starting point.
The edges extracted will conform the corresponding spanning tree.


Signature Summary
-----------------

.. code-block:: none

	withPointsVia(edges_sql, points_sql, start_pid, distance)
	withPointsVia(edges_sql, points_sql, start_pid, distance, directed, driving_side, details)
    RETURNS SET OF (seq, node, edge, cost, agg_cost)

Signatures
-----------------

.. index::
	single: withPointsVia(edges_sql, points_sql, start_pid, distance) -- proposed

Minimal signature
...................................................

The minimal signature:
    - Is for a **directed** graph.
    - The driving side is set as **b** both. So arriving/departing to/from the point(s) can be in any direction.
    - No **details** are given about distance of other points of the query.

.. code-block:: none

	withPointsVia(edges_sql, points_sql, start_pid, distance)
    RETURNS SET OF (seq, node, edge, cost, agg_cost)


:Example:

.. literalinclude:: doc-pgr_withPointsVia.queries
   :start-after: --q1
   :end-before: --q2

.. index::
	single: withPointsVia(edges_sql, points_sql, start_pid, distance, directed, driving_side, details) -- proposed

Driving distance from a single point
...................................................

Finds the driving distance depending on the optional parameters setup.

.. code-block:: none

	pgr_withPointsVia(edges_sql, points_sql, start_pid, distance,
        directed := true, driving_side := 'b', details := false)
    RETURNS SET OF (seq, node, edge, cost, agg_cost)


:Example: Right side driving topology

.. literalinclude:: doc-pgr_withPointsVia.queries
   :start-after: --q2
   :end-before: --q3

Description of the Signatures
---------------------------------------------------


..
    description of the sql queries

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

.. include:: pgRouting-concepts.rst
    :start-after: points_sql_start
    :end-before: points_sql_end



Description of the parameters of the signatures
...................................................


================ ================= =================================================
Parameter        Type              Description
================ ================= =================================================
**edges_sql**    ``TEXT``          Edges SQL query as described above.
**points_sql**   ``TEXT``          Points SQL query as described above.
**start_pid**    ``ANY-INTEGER``   Starting point id.
**distance**     ``ANY_NUMERICAL`` Distance from the start_pid
**directed**     ``BOOLEAN``       (optional). When ``false`` the graph is considered as Undirected. Default is ``true`` which considers the graph as Directed.
**driving_side** ``CHAR``          (optional) Value in ['b', 'r', 'l', NULL] indicating if the driving side is:
                                     - In the right or left or
                                     - If it doesn't matter with 'b' or NULL.
                                     - If column not present 'b' is considered.

**details**      ``BOOLEAN``       (optional). When ``true`` the results will include the driving distance to the points with in Distance.
                                   Default is ``false`` which ignores other points of the points_sql.
================ ================= =================================================


Description of the return values
...................................................

Returns set of ``(seq, node, edge, cost, agg_cost)``

============ =========== =================================================
Column           Type              Description
============ =========== =================================================
**seq**      ``INT``     row sequence.
**node**     ``BIGINT``  Identifier of the node within the Distance from ``start_pid``. If ``details =: true`` a negative value is the identifier of a point.
**edge**     ``BIGINT``  Identifier of the edge used to go from ``node`` to the next node in the path sequence.
                           - ``-1`` for the last row in the path sequence.

**cost**     ``FLOAT``   Cost to traverse from ``node`` using ``edge`` to the next ``node`` in the path sequence.
                           - ``0`` for the last row in the path sequence.

**agg_cost** ``FLOAT``   Aggregate cost from ``start_pid`` to ``node``.
                           - ``0`` for the first row in the path sequence.

============ =========== =================================================

Examples for queries marked as ``directed`` with ``cost`` and ``reverse_cost`` columns
--------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig1`

:Example: Left side driving topology

.. literalinclude:: doc-pgr_withPointsVia.queries
   :start-after: --q3
   :end-before: --q4

:Example: Does not matter driving side.

.. literalinclude:: doc-pgr_withPointsVia.queries
   :start-after: --q4
   :end-before: --q5







.. rubric:: History

* Proposed in version 2.2


See Also
-------------------------------------------------------------------------------

* :ref:`pgr_drivingDistance` - Driving distance using dijkstra.
* :ref:`pgr_alphashape` - Alpha shape computation.
* :ref:`pgr_points_as_polygon` - Polygon around set of points.
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

