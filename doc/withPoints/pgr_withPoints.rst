..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_withPoints - Proposed
===============================================================================

``pgr_withPoints`` - Returns the shortest path in a graph with additional temporary vertices.

.. include:: proposed.rst
   :start-after: begin-warning
   :end-before: end-warning

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/table_of_contents.html

   Boost Graph Inside

.. rubric:: Availability

* Version 2.2.0

  * New **proposed** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_withPoints.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_withPoints.html>`__

* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_withPoints.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_withPoints.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_withPoints.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/withPoints/doc/pgr_withPoints.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/withPoints/doc/pgr_withPoints.html>`__

Description
-------------------------------------------------------------------------------

Modify the graph to include points defined by points_sql.
Using Dijkstra algorithm, find the shortest path(s)

**The main characteristics are:**

- Process is done only on edges with positive costs.
- Vertices of the graph are:

  - **positive** when it belongs to the edges_sql
  - **negative** when it belongs to the points_sql

- Values are returned when there is a path.

  - When the starting vertex and ending vertex are the same, there is no path.
    - The agg_cost the non included values (v, v) is 0

  - When the starting vertex and ending vertex are the different and there is no path:
    - The agg_cost the non included values (u, v) is ∞

- For optimization purposes, any duplicated value in the start_vids or end_vids are ignored.
- The returned values are ordered:
  - start_vid ascending
  - end_vid ascending

* Running time: :math:`O(|start\_vids|\times(V \log V + E))`

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_withPoints(edges_sql, points_sql, from_vid,  to_vid  [, directed] [, driving_side] [, details])
    pgr_withPoints(edges_sql, points_sql, from_vid,  to_vids [, directed] [, driving_side] [, details])
    pgr_withPoints(edges_sql, points_sql, from_vids, to_vid  [, directed] [, driving_side] [, details])
    pgr_withPoints(edges_sql, points_sql, from_vids, to_vids [, directed] [, driving_side] [, details])
    RETURNS SET OF (seq, path_seq, [start_vid,] [end_vid,] node, edge, cost, agg_cost)

.. rubric:: Using defaults

.. code-block:: none

    pgr_withPoints(edges_sql, points_sql, from_vid, to_vid)
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)

:Example: From point :math:`1` to point :math:`3`

    - For a **directed** graph.
    - The driving side is set as **b** both. So arriving/departing to/from the point(s) can be in any direction.
    - No **details** are given about distance of other points of points_sql query.

.. literalinclude:: doc-pgr_withPoints.queries
   :start-after: --e1
   :end-before: --e2

.. index::
    single: withPoints(One to One) - Proposed

One to One
...............................................................................

.. code-block:: none

    pgr_withPoints(edges_sql, points_sql, from_vid,  to_vid  [, directed] [, driving_side] [, details])
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)

:Example: From point :math:`1` to vertex :math:`3` with details of passing points

.. literalinclude:: doc-pgr_withPoints.queries
   :start-after: --e2
   :end-before: --e3

.. index::
    single: withPoints(One to Many) - Proposed

One to Many
...............................................................................

.. code-block:: none

    pgr_withPoints(edges_sql, points_sql, from_vid,  to_vids [, directed] [, driving_side] [, details])
    RETURNS SET OF (seq, path_seq, end_vid, node, edge, cost, agg_cost)

:Example: From point :math:`1` to point :math:`3` and vertex :math:`5`

.. literalinclude:: doc-pgr_withPoints.queries
   :start-after: --e3
   :end-before: --e4

.. index::
    single: withPoints(Many to One) - Proposed

Many to One
...............................................................................

.. code-block:: none

    pgr_withPoints(edges_sql, points_sql, from_vids, to_vid  [, directed] [, driving_side] [, details])
    RETURNS SET OF (seq, path_seq, start_vid, node, edge, cost, agg_cost)

:Example: From point :math:`1` and vertex :math:`2` to point :math:`3`

.. literalinclude:: doc-pgr_withPoints.queries
   :start-after: --e4
   :end-before: --e5

.. index::
    single: withPoints(Many to Many) - Proposed

Many to Many
...............................................................................

.. code-block:: none

    pgr_withPoints(edges_sql, points_sql, from_vids, to_vids [, directed] [, driving_side] [, details])
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)

:Example: From point :math:`1` and vertex :math:`2`  to point :math:`3` and vertex :math:`7`

.. literalinclude:: doc-pgr_withPoints.queries
   :start-after: --e5
   :end-before: --q2

Parameters
-------------------------------------------------------------------------------

================ ====================== =================================================
Parameter        Type                   Description
================ ====================== =================================================
**edges_sql**    ``TEXT``               Edges SQL query as described above.
**points_sql**   ``TEXT``               Points SQL query as described above.
**start_vid**    ``ANY-INTEGER``        Starting vertex identifier. When negative: is a point's pid.
**end_vid**      ``ANY-INTEGER``        Ending vertex identifier. When negative: is a point's pid.
**start_vids**   ``ARRAY[ANY-INTEGER]`` Array of identifiers of starting vertices. When negative: is a point's pid.
**end_vids**     ``ARRAY[ANY-INTEGER]`` Array of identifiers of ending vertices. When negative: is a point's pid.
**directed**     ``BOOLEAN``            (optional). When ``false`` the graph is considered as Undirected. Default is ``true`` which considers the graph as Directed.
**driving_side** ``CHAR``               (optional) Value in ['b', 'r', 'l', NULL] indicating if the driving side is:
                                          - In the right or left or
                                          - If it doesn't matter with 'b' or NULL.
                                          - If column not present 'b' is considered.

**details**      ``BOOLEAN``            (optional). When ``true`` the results will include the points in points_sql that are in the path.
                                        Default is ``false`` which ignores other points of the points_sql.
================ ====================== =================================================

Inner query
-------------------------------------------------------------------------------

..
    description of the sql queries

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

.. include:: pgRouting-concepts.rst
    :start-after: points_sql_start
    :end-before: points_sql_end

Result Columns
-------------------------------------------------------------------------------

============= =========== =================================================
Column           Type              Description
============= =========== =================================================
**seq**       ``INTEGER`` Row sequence.
**path_seq**  ``INTEGER`` Path sequence that indicates the relative position on the path.
**start_vid** ``BIGINT``  Identifier of the starting vertex. When negative: is a point's pid.
**end_vid**   ``BIGINT``  Identifier of the ending vertex. When negative: is a point's pid.
**node**      ``BIGINT``  Identifier of the node:
                            - A positive value indicates the node is a vertex of edges_sql.
                            - A negative value indicates the node is a point of points_sql.

**edge**      ``BIGINT``  Identifier of the edge used to go from ``node`` to the next node in the path sequence.
                            - ``-1`` for the last row in the path sequence.

**cost**      ``FLOAT``   Cost to traverse from ``node`` using ``edge`` to the next ``node`` in the path sequence.
                            - ``0`` for the last row in the path sequence.

**agg_cost**  ``FLOAT``   Aggregate cost from ``start_pid`` to ``node``.
                            - ``0`` for the first row in the path sequence.

============= =========== =================================================

Additional Examples
-------------------------------------------------------------------------------

:Example: Which path (if any) passes in front of point :math:`6` or vertex :math:`6` with **right** side driving topology.

.. literalinclude:: doc-pgr_withPoints.queries
   :start-after: --q2
   :end-before: --q3

:Example: Which path (if any) passes in front of point :math:`6` or vertex :math:`6` with **left** side driving topology.

.. literalinclude:: doc-pgr_withPoints.queries
   :start-after: --q3
   :end-before: --q4

:Example: From point :math:`1` and vertex :math:`2` to point :math:`3` to vertex :math:`7` on an **undirected** graph, with details.

.. literalinclude:: doc-pgr_withPoints.queries
   :start-after: --q4
   :end-before: --q5

The queries use the :doc:`sampledata` network

See Also
-------------------------------------------------------------------------------

* :doc:`withPoints-family`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
