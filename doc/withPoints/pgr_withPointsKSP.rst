..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_withPointsKSP - Proposed
===============================================================================

``pgr_withPointsKSP`` - Find the K shortest paths using Yen's algorithm.

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
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_withPointsKSP.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_withPointsKSP.html>`__

* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_withPointsKSP.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_withPointsKSP.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_withPointsKSP.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/withPoints/doc/pgr_withPointsKSP.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/withPoints/doc/pgr_withPointsKSP.html>`__

Description
-------------------------------------------------------------------------------

Modifies the graph to include the points defined in the ``points_sql`` and
using Yen algorithm, finds the :math:`K` shortest paths.

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_withPointsKSP(edges_sql, points_sql, start_pid, end_pid, K [, directed] [, heap_paths] [, driving_side] [, details])
    RETURNS SET OF (seq, path_id, path_seq, node, edge, cost, agg_cost)

.. rubric:: Using defaults

.. code-block:: none

    pgr_withPointsKSP(edges_sql, points_sql, start_pid, end_pid, K)
    RETURNS SET OF (seq, path_id, path_seq, node, edge, cost, agg_cost)

:Example: From point :math:`1` to point :math:`2` in :math:`2` cycles

- For a **directed** graph.
- The driving side is set as **b** both. So arriving/departing to/from the point(s) can be in any direction.
- No **details** are given about distance of other points of the query.
- No **heap paths** are returned.

.. literalinclude:: doc-pgr_withPointsKSP.queries
   :start-after: --q1
   :end-before: --q2

Complete Signature
...............................................................................

Finds the :math:`K` shortest paths depending on the optional parameters setup.

.. code-block:: none

    pgr_withPointsKSP(edges_sql, points_sql, start_pid, end_pid, K [, directed] [, heap_paths] [, driving_side] [, details])
    RETURNS SET OF (seq, path_id, path_seq, node, edge, cost, agg_cost)

:Example: From point :math:`1` to vertex :math:`6` in :math:`2` cycles with details.

.. literalinclude:: doc-pgr_withPointsKSP.queries
   :start-after: --q2
   :end-before: --q3

Parameters
-------------------------------------------------------------------------------

================ ================= =================================================
Parameter        Type              Description
================ ================= =================================================
**edges_sql**    ``TEXT``          Edges SQL query as described above.
**points_sql**   ``TEXT``          Points SQL query as described above.
**start_pid**    ``ANY-INTEGER``   Starting point id.
**end_pid**      ``ANY-INTEGER``   Ending point id.
**K**            ``INTEGER``       Number of shortest paths.
**directed**     ``BOOLEAN``       (optional). When ``false`` the graph is considered as Undirected. Default is ``true`` which considers the graph as Directed.
**heap_paths**   ``BOOLEAN``       (optional). When ``true`` the paths calculated to get the shortests paths will be returned also. Default is ``false`` only the K shortest paths are returned.
**driving_side** ``CHAR``          (optional) Value in ['b', 'r', 'l', NULL] indicating if the driving side is:
                                     - In the right or left or
                                     - If it doesn't matter with 'b' or NULL.
                                     - If column not present 'b' is considered.

**details**      ``BOOLEAN``       (optional). When ``true`` the results will include the driving distance to the points with in the ``distance``.
                                   Default is ``false`` which ignores other points of the points_sql.
================ ================= =================================================

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

============ =========== =================================================
Column           Type              Description
============ =========== =================================================
**seq**      ``INTEGER`` Row sequence.
**path_seq** ``INTEGER`` Relative position in the path of node and edge. Has value 1 for the beginning of a path.
**path_id**  ``INTEGER``  Path identifier. The ordering of the paths: For two paths i, j if i < j then agg_cost(i) <= agg_cost(j).
**node**     ``BIGINT``  Identifier of the node in the path. Negative values are the identifiers of a point.
**edge**     ``BIGINT``  Identifier of the edge used to go from ``node`` to the next node in the path sequence.
                           - ``-1`` for the last row in the path sequence.

**cost**     ``FLOAT``   Cost to traverse from ``node`` using ``edge`` to the next ``node`` in the path sequence.
                           - ``0`` for the last row in the path sequence.

**agg_cost** ``FLOAT``   Aggregate cost from ``start_pid`` to ``node``.
                           - ``0`` for the first row in the path sequence.

============ =========== =================================================

Additional Examples
--------------------------------------------------------------------------------------

:Example: Left side driving topology from point :math:`1` to point :math:`2` in :math:`2` cycles, with details

.. literalinclude:: doc-pgr_withPointsKSP.queries
   :start-after: --q3
   :end-before: --q4

:Example: Right side driving topology from point :math:`1` to point :math:`2` in :math:`2` cycles, with heap paths and details

.. literalinclude:: doc-pgr_withPointsKSP.queries
   :start-after: --q4
   :end-before: --q5

The queries use the :doc:`sampledata` network.

See Also
-------------------------------------------------------------------------------

* :doc:`withPoints-family`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

