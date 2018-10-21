..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_floydWarshall
===============================================================================

``pgr_floydWarshall`` - Returns the sum of the costs of the shortest path for each
pair of nodes in the graph using Floyd-Warshall algorithm.

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/floyd_warshall_shortest.html

   Boost Graph Inside

.. rubric:: Availability

* 2.0.0

* Re-design of pgr_apspWarshall in Version 2.2.0


Description
-------------------------------------------------------------------------------

The Floyd-Warshall algorithm, also known as Floyd's algorithm,
is a good choice to calculate the sum of the costs of the shortest path for each
pair of nodes in the graph, for *dense graphs*. We use Boost's
implementation which runs in :math:`\Theta(V^3)` time,

The main Characteristics are:
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

  - **Recommended, use a bounding box of no more than 3500 edges.**

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_floydWarshall(edges_sql)
    pgr floydWarshall(edges_sql, directed)
    RETURNS SET OF (start_vid, end_vid,  agg_cost) or EMPTY SET

.. rubric:: Minimal Signature

.. code-block:: none

    pgr_floydWarshall(edges_sql)
    RETURNS SET OF (start_vid, end_vid,  agg_cost) or EMPTY SET

:Example 1: On a directed graph.

.. literalinclude:: doc-floydWarshall.queries
   :start-after: -- q1
   :end-before: -- q2

.. rubric:: Complete Signature

.. code-block:: none

    pgr_floydWarshall(edges_sql, directed)
    RETURNS SET OF (start_vid, end_vid,  agg_cost) or EMPTY SET

:Example 2: On an undirected graph.

.. literalinclude:: doc-floydWarshall.queries
   :start-after: -- q2
   :end-before: -- q3

Parameters 
-------------------------------------------------------------------------------

Receives  ``(edges_sql, directed)``

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

* :doc:`pgr_johnson`
* `Boost floyd-Warshall <http://www.boost.org/libs/graph/doc/floyd_warshall_shortest.html>`_ algorithm
* Queries uses the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

