..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_johnson
===============================================================================

``pgr_johnson`` - Returns the sum of the costs of the shortest path for each
pair of nodes in the graph using Floyd-Warshall algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/johnson_all_pairs_shortest.html

   Boost Graph Inside

.. rubric:: Availability

* Version 2.2.0

  * Signature change
  * Old signature no longer supported

* Version 2.0.0

  * **Official** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_johnson.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_johnson.html>`__
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_johnson.html>`__

* **Unsupported versions:**
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_johnson.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_johnson.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/allpairs/doc/pgr_johnson.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/allpairs/doc/pgr_johnson.html>`__
  `2.1 <https://docs.pgrouting.org/2.1/en/src/apsp_johnson/doc/index.html>`__
  `2.0 <https://docs.pgrouting.org/2.0/en/src/apsp_johnson/doc/index.html>`__

Description
-------------------------------------------------------------------------------

The Johnson algorithm, is a good choice to calculate the sum of the costs
of the shortest path for each pair of nodes in the graph, for *sparse graphs*.
It usees the Boost's implementation which runs in :math:`O(V E \log V)` time,

The main characteristics are:
  - It does not return a path.
  - Returns the sum of the costs of the shortest path for each pair of nodes in the graph.
  - Process is done only on edges with positive costs.
  - Boost returns a :math:`V \times V` matrix, where the infinity values.
    Represent the distance between vertices for which there is no path.

    - We return only the non infinity values in form of a set of `(start_vid, end_vid, agg_cost)`.

  - Let be the case the values returned are stored in a table, so the unique index would be the pair:
    `(start_vid, end_vid)`.

  - For the undirected graph, the results are symmetric.

    - The  `agg_cost` of `(u, v)` is the same as for `(v, u)`.

  - When  `start_vid` = `end_vid`, the `agg_cost` = 0.

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_johnson(edges_sql)
    pgr johnson(edges_sql [, directed])
    RETURNS SET OF (start_vid, end_vid, agg_cost)
    OR EMPTY SET

.. rubric:: Using default

.. code-block:: none

    pgr_johnson(edges_sql)
    RETURNS SET OF (start_vid, end_vid, agg_cost)
    OR EMPTY SET

:Example 1: For vertices :math:`\{1, 2, 3, 4\}` on a **directed** graph

.. literalinclude:: doc-johnson.queries
   :start-after: -- q1
   :end-before: -- q2

Complete Signature
...............................................................................

.. code-block:: none

    pgr_johnson(edges_sql[, directed])
    RETURNS SET OF (start_vid, end_vid, agg_cost)
    OR EMPTY SET

:Example 2: For vertices :math:`\{1, 2, 3, 4\}` on an **undirected** graph

.. literalinclude:: doc-johnson.queries
   :start-after: -- q2
   :end-before: -- q3

Parameters
-------------------------------------------------------------------------------

============= ============= =================================================
Parameter     Type          Description
============= ============= =================================================
**edges_sql** ``TEXT``      SQL query as described above.
**directed**  ``BOOLEAN``   (optional) Default is true (is directed). When set to false the graph is considered as Undirected
============= ============= =================================================

Inner query
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: no_id_edges_sql_start
    :end-before: no_id_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

Returns set of ``(start_vid, end_vid, agg_cost)``

============= ============= =================================================
Column        Type          Description
============= ============= =================================================
**start_vid** ``BIGINT``    Identifier of the starting vertex.
**end_vid**   ``BIGINT``    Identifier of the ending vertex.
**agg_cost**  ``FLOAT``     Total cost from ``start_vid`` to ``end_vid``.
============= ============= =================================================

See Also
-------------------------------------------------------------------------------

* :doc:`pgr_floydWarshall`
* `Boost Johnson <https://www.boost.org/libs/graph/doc/johnson_all_pairs_shortest.html>`_ algorithm implementation.
* Queries uses the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

