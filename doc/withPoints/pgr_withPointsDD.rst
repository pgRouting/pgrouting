..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_withPointsDD - Proposed
===============================================================================

``pgr_withPointsDD`` - Returns the driving distance from a starting point.

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
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_withPointsDD.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_withPointsDD.html>`__

* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_withPointsDD.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_withPointsDD.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_withPointsDD.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/withPoints/doc/pgr_withPointsDD.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/withPoints/doc/pgr_withPointsDD.html>`__

Description
-------------------------------------------------------------------------------

Modify the graph to include points and
using Dijkstra algorithm, extracts all the nodes and points that have costs less
than or equal to the value ``distance`` from the starting point.
The edges extracted will conform the corresponding spanning tree.

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_withPointsDD(edges_sql, points_sql, from_vids, distance [, directed] [, driving_side] [, details] [, equicost])
    RETURNS SET OF (seq, node, edge, cost, agg_cost)

.. rubric:: Using defaults

- For a **directed** graph.
- The driving side is set as **b** both. So arriving/departing to/from the point(s) can be in any direction.
- No **details** are given about distance of other points of the query.

.. code-block:: none

    pgr_withPointsDD(edges_sql, points_sql, start_vid, distance)
    RETURNS SET OF (seq, node, edge, cost, agg_cost)

:Example: From point :math:`1` with :math:`agg\_cost <= 3.8`

- For a **directed** graph.
- The driving side is set as **b** both. So arriving/departing to/from the point(s) can be in any direction.
- No **details** are given about distance of other points of the query.


.. literalinclude:: doc-pgr_withPointsDD.queries
   :start-after: --q1
   :end-before: --q2

.. index::
    single: withPointsDD(Single Start Vertex) - proposed

Single vertex
...............................................................................

Finds the driving distance depending on the optional parameters setup.

.. code-block:: none

    pgr_withPointsDD(edges_sql, points_sql, from_vid,  distance [, directed] [, driving_side] [, details])
    RETURNS SET OF (seq, node, edge, cost, agg_cost)

:Example: Right side driving topology, from point :math:`1` with :math:`agg\_cost <= 3.8`

.. literalinclude:: doc-pgr_withPointsDD.queries
   :start-after: --q2
   :end-before: --q3

.. index::
    single: withPointsDD(Multiple Starting Vertices) - proposed

Multiple vertices
...............................................................................

Finds the driving distance depending on the optional parameters setup.

.. code-block:: none

    pgr_withPointsDD(edges_sql, points_sql, from_vids, distance [, directed] [, driving_side] [, details] [, equicost])
    RETURNS SET OF (seq, node, edge, cost, agg_cost)

Parameters
-------------------------------------------------------------------------------

================ ================= =================================================
Parameter        Type              Description
================ ================= =================================================
**edges_sql**    ``TEXT``          Edges SQL query as described above.
**points_sql**   ``TEXT``          Points SQL query as described above.
**start_vid**    ``ANY-INTEGER``   Starting point id
**distance**     ``ANY-NUMERICAL`` Distance from the start_pid
**directed**     ``BOOLEAN``       (optional). When ``false`` the graph is considered as Undirected. Default is ``true`` which considers the graph as Directed.
**driving_side** ``CHAR``          (optional). Value in ['b', 'r', 'l', NULL] indicating if the driving side is:
                                     - In the right or left or
                                     - If it doesn't matter with 'b' or NULL.
                                     - If column not present 'b' is considered.

**details**      ``BOOLEAN``       (optional). When ``true`` the results will include the driving distance to the points with in the ``distance``.
                                   Default is ``false`` which ignores other points of the points_sql.
**equicost**     ``BOOLEAN``       (optional). When ``true`` the nodes will only appear in the closest start_v list. Default is ``false`` which resembles several calls using the single starting point signatures. Tie brakes are arbitrary.
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
**seq**      ``INT``     row sequence.
**node**     ``BIGINT``  Identifier of the node within the Distance from ``start_pid``. If ``details =: true`` a negative value is the identifier of a point.
**edge**     ``BIGINT``  Identifier of the edge used to go from ``node`` to the next node in the path sequence.
                           - ``-1`` when ``start_vid`` = ``node``.

**cost**     ``FLOAT``   Cost to traverse ``edge``.
                           - ``0`` when ``start_vid`` = ``node``.

**agg_cost** ``FLOAT``   Aggregate cost from ``start_vid`` to ``node``.
                           - ``0`` when ``start_vid`` = ``node``.

============ =========== =================================================

Additional Examples
--------------------------------------------------------------------------------------

.. rubric:: Examples for queries marked as ``directed`` with ``cost`` and ``reverse_cost`` columns.

The examples in this section use the following :ref:`fig1`

:Example: Left side driving topology from point :math:`1` with :math:`agg\_cost <= 3.8`, with details

.. literalinclude:: doc-pgr_withPointsDD.queries
   :start-after: --q3
   :end-before: --q4

:Example: From point :math:`1` with :math:`agg\_cost <= 3.8`, does not matter driving side, with details

.. literalinclude:: doc-pgr_withPointsDD.queries
   :start-after: --q4
   :end-before: --q5

The queries use the :doc:`sampledata` network.

See Also
-------------------------------------------------------------------------------

* :doc:`pgr_drivingDistance` - Driving distance using dijkstra.
* :doc:`pgr_alphaShape` - Alpha shape computation.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

