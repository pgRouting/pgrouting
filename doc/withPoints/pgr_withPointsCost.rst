..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_withPointsCost - Proposed
===============================================================================

``pgr_withPointsCost`` - Calculates the shortest path and returns only the aggregate cost of the shortest path(s) found, for the combination of points given.

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
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_withPointsCost.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_withPointsCost.html>`__

* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_withPointsCost.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_withPointsCost.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_withPointsCost.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/withPoints/doc/pgr_withPointsCost.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/withPoints/doc/pgr_withPointsCost.html>`__

Description
-------------------------------------------------------------------------------

Modify the graph to include points defined by points_sql.
Using Dijkstra algorithm, return only the aggregate cost of the shortest path(s) found.

The main characteristics are:
  - It does not return a path.
  - Returns the sum of the costs of the shortest path for pair combination of vertices in the modified graph.
  - Vertices of the graph are:

    - **positive** when it belongs to the edges_sql
    - **negative** when it belongs to the points_sql

  - Process is done only on edges with positive costs.
  - Values are returned when there is a path.

    - The returned values are in the form of a set of `(start_vid, end_vid, agg_cost)`.

    - When the starting vertex and ending vertex are the same, there is no path.

      - The `agg_cost` in the non included values `(v, v)` is `0`

    - When the starting vertex and ending vertex are the different and there is no path.

      - The `agg_cost` in the non included values `(u, v)` is :math:`\infty`

  - If the values returned are stored in a table, the unique index would be the pair:
    `(start_vid, end_vid)`.

  - For **undirected** graphs, the results are **symmetric**.

    - The  `agg_cost` of `(u, v)` is the same as for `(v, u)`.

  - For optimization purposes, any duplicated value in the `start_vids` or `end_vids` is ignored.

  - The returned values are ordered:

    - `start_vid` ascending
    - `end_vid` ascending

  - Running time: :math:`O(| start\_vids | * (V \log V + E))`

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_withPointsCost(edges_sql, points_sql, from_vid,  to_vid  [, directed] [, driving_side])
    pgr_withPointsCost(edges_sql, points_sql, from_vid,  to_vids [, directed] [, driving_side])
    pgr_withPointsCost(edges_sql, points_sql, from_vids, to_vid  [, directed] [, driving_side])
    pgr_withPointsCost(edges_sql, points_sql, from_vids, to_vids [, directed] [, driving_side])
    RETURNS SET OF (start_vid, end_vid, agg_cost)

.. note:: There is no **details** flag, unlike the other members of the withPoints family of functions.

.. index::
    single: withPointsCost(Minimal Use) - proposed

.. rubric:: Using defaults

.. code-block:: none

    pgr_withPointsCost(edges_sql, points_sql, start_vid, end_vid)
    RETURNS SET OF (start_vid, end_vid, agg_cost)

:Example: From point :math:`1` to point :math:`3`

- For a **directed** graph.
- The driving side is set as **b** both. So arriving/departing to/from the point(s) can be in any direction.


.. literalinclude:: doc-pgr_withPointsCost.queries
   :start-after: --e1
   :end-before: --e2

.. index::
    single: withPointsCost(One To One) - proposed

One to One
...............................................................................

.. code-block:: none

    pgr_withPointsCost(edges_sql, points_sql, from_vid,  to_vid  [, directed] [, driving_side])
    RETURNS SET OF (seq, node, edge, cost, agg_cost)

:Example: From point :math:`1` to vertex :math:`3` on an **undirected** graph.

.. literalinclude:: doc-pgr_withPointsCost.queries
   :start-after: --e2
   :end-before: --e3

.. index::
    single: withPointsCost(One To Many) - proposed

One to Many
...............................................................................

.. code-block:: none

    pgr_withPointsCost(edges_sql, points_sql, from_vid,  to_vids [, directed] [, driving_side])
    RETURNS SET OF (start_vid, end_vid, agg_cost)

:Example: From point :math:`1` to point :math:`3` and vertex :math:`5` on a **directed** graph.

.. literalinclude:: doc-pgr_withPointsCost.queries
   :start-after: --e3
   :end-before: --e4

.. index::
    single: withPointsCost(Many To One) - proposed

Many to One
...............................................................................

.. code-block:: none

    pgr_withPointsCost(edges_sql, points_sql, from_vids, to_vid  [, directed] [, driving_side])
    RETURNS SET OF (start_vid, end_vid, agg_cost)

:Example: From point :math:`1` and vertex :math:`2` to point :math:`3` on a **directed** graph.

.. literalinclude:: doc-pgr_withPointsCost.queries
   :start-after: --e4
   :end-before: --e5

.. index::
    single: withPointsCost(Many To Many) - proposed

Many to Many
...............................................................................

.. code-block:: none

    pgr_withPointsCost(edges_sql, points_sql, from_vids, to_vids [, directed] [, driving_side])
    RETURNS SET OF (start_vid, end_vid, agg_cost)

:Example: From point :math:`1` and vertex :math:`2` to point :math:`3` and vertex :math:`7` on a **directed** graph.

.. literalinclude:: doc-pgr_withPointsCost.queries
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
**start_vid** ``BIGINT``  Identifier of the starting vertex. When negative: is a point's pid.
**end_vid**   ``BIGINT``  Identifier of the ending point. When negative: is a point's pid.
**agg_cost**  ``FLOAT``   Aggregate cost from ``start_vid`` to ``end_vid``.
============= =========== =================================================

Additional Examples
-------------------------------------------------------------------------------

:Example: From point :math:`1` and vertex :math:`2` to point :math:`3` and vertex :math:`7`, with **right** side driving topology

.. literalinclude:: doc-pgr_withPointsCost.queries
   :start-after: --q2
   :end-before: --q3

:Example: From point :math:`1` and vertex :math:`2` to point :math:`3` and vertex :math:`7`, with **left** side driving topology

.. literalinclude:: doc-pgr_withPointsCost.queries
   :start-after: --q3
   :end-before: --q4

:Example: From point :math:`1` and vertex :math:`2` to point :math:`3` and vertex :math:`7`, does not matter driving side.

.. literalinclude:: doc-pgr_withPointsCost.queries
   :start-after: --q4
   :end-before: --q5

The queries use the :doc:`sampledata` network.

See Also
-------------------------------------------------------------------------------

* :doc:`withPoints-family`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

