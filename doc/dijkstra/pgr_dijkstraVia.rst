..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_dijkstraVia - Proposed
===============================================================================

``pgr_dijkstraVia`` — Using dijkstra algorithm, it finds the route that goes through
a list of vertices.

.. include:: proposed.rst
   :start-after: stable-begin-warning
   :end-before: stable-end-warning

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/table_of_contents.html

   Boost Graph Inside

.. rubric:: Availability

* Version 2.2.0

  * New **proposed** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_dijkstraVia.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_dijkstraVia.html>`__

* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_dijkstraVia.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_dijkstraVia.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_dijkstraVia.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/dijkstra/doc/pgr_dijkstraVia.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/dijkstra/doc/pgr_dijkstraVia.html>`__


Description
-------------------------------------------------------------------------------

Given a list of vertices and a graph, this function is equivalent to finding the
shortest path between :math:`vertex_i` and :math:`vertex_{i+1}` for all :math:`i < size\_of(vertex_via)`.

The paths represents the sections of the route.

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_dijkstraVia(edges_sql, via_vertices [, directed] [, strict] [, U_turn_on_edge])
    RETURNS SET OF (seq, path_pid, path_seq, start_vid, end_vid,
        node, edge, cost, agg_cost, route_agg_cost)
    OR EMPTY SET

.. rubric:: Using default

.. code-block:: none

    pgr_dijkstraVia(edges_sql, via_vertices)
    RETURNS SET OF (seq, path_pid, path_seq, start_vid, end_vid,
        node, edge, cost, agg_cost, route_agg_cost)
    OR EMPTY SET

:Example: Find the route that visits the vertices :math:`\{ 1, 3, 9\}`  in that order

.. literalinclude:: doc-pgr_dijkstraVia.queries
    :start-after: --q00
    :end-before: -- q0

.. index::
    single: dijkstraVia - Proposed

Complete Signature
...............................................................................

.. code-block:: none

    pgr_dijkstraVia(edges_sql, via_vertices [, directed] [, strict] [, U_turn_on_edge])
    RETURNS SET OF (seq, path_pid, path_seq, start_vid, end_vid,
        node, edge, cost, agg_cost, route_agg_cost)
    OR EMPTY SET

:Example: Find the route that visits the vertices :math:`\{ 1, 3, 9\}` in that order on an **undirected** graph, avoiding U-turns when possible

.. literalinclude:: doc-pgr_dijkstraVia.queries
    :start-after: -- q0
    :end-before: -- q1

.. include:: pgRouting-concepts.rst
    :start-after: pgr_dijkstra_via_parameters_start
    :end-before: pgr_dijkstra_via_parameters_end

Inner query
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Return Columns
-------------------------------------------------------------------------------

Returns set of ``(start_vid, end_vid, agg_cost)``

================== ============= =================================================
Column             Type          Description
================== ============= =================================================
**seq**            ``BIGINT``    Sequential value starting from 1.
**path_pid**       ``BIGINT``    Identifier of the path.
**path_seq**       ``BIGINT``    Sequential value starting from 1 for the path.
**start_vid**      ``BIGINT``    Identifier of the starting vertex of the path.
**end_vid**        ``BIGINT``    Identifier of the ending vertex of the path.
**node**           ``BIGINT``    Identifier of the node in the path from start_vid to end_vid.
**edge**           ``BIGINT``    Identifier of the edge used to go from node to the next node in the path sequence. -1 for the last node of the path. -2 for the last node of the route.
**cost**           ``FLOAT``     Cost to traverse from ``node`` using ``edge`` to the next node in the route sequence.
**agg_cost**       ``FLOAT``     Total cost from ``start_vid`` to ``end_vid`` of the path.
**route_agg_cost** ``FLOAT``     Total cost from ``start_vid`` of ``path_pid = 1`` to ``end_vid`` of the current ``path_pid`` .
================== ============= =================================================

Additional Examples
-------------------------------------------------------------------------------

:Example 1: Find the route that visits the vertices :math:`\{1, 5, 3, 9, 4\}` in that order

.. literalinclude:: doc-pgr_dijkstraVia.queries
    :start-after: -- q1
    :end-before: -- q2

:Example 2: What's the aggregate cost of the third path?

.. literalinclude:: doc-pgr_dijkstraVia.queries
    :start-after: -- q2
    :end-before: -- q3

:Example 3: What's the route's aggregate cost of the route at the end of the third path?

.. literalinclude:: doc-pgr_dijkstraVia.queries
    :start-after: -- q3
    :end-before: -- q4

:Example 4: How are the nodes visited in the route?

.. literalinclude:: doc-pgr_dijkstraVia.queries
    :start-after: -- q4
    :end-before: -- q5

:Example 5: What are the aggregate costs of the route when the visited vertices are reached?

.. literalinclude:: doc-pgr_dijkstraVia.queries
    :start-after: -- q5
    :end-before: -- q6

:Example 6: Show the route's seq and aggregate cost and a status of "passes in front" or "visits" node :math:`9`

.. literalinclude:: doc-pgr_dijkstraVia.queries
    :start-after: -- q6

See Also
-------------------------------------------------------------------------------

* https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
* :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

