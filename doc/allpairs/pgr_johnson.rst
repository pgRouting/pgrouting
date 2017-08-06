..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_johnson:

pgr_johnson
===============================================================================

Synopsis
-------------------------------------------------------------------------------

``pgr_johnson`` - Returns the sum of the costs of the shortest path for each
pair of nodes in the graph using Floyd-Warshall algorithm.

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/johnson_all_pairs_shortest.html

   Boost Graph Inside

.. rubric:: Availability: 2.0.0

* Renamed on 2.2.0, previous name pgr_apspJohnson


The Johnson algorithm, is a good choice to calculate the sum of the costs
of the shortest path for each pair of nodes in the graph, for *sparse graphs*.
It usees the Boost's implementation which runs in :math:`O(V E \log V)` time,


Characteristics
----------------

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



Signature Summary
--------------------------------------------

.. code-block:: none

    pgr_johnson(edges_sql)
    pgr johnson(edges_sql, directed)
    RETURNS SET OF (start_vid, end_vid,  agg_cost) or EMPTY SET

Signatures
--------------------------------------------


.. index::
    single: johnson(Minimal Signature)

Minimal Signature
...................

.. code-block:: none

    pgr_johnson(edges_sql)
    RETURNS SET OF (start_vid, end_vid,  agg_cost) or EMPTY SET

:Example 1: On a directed graph.

.. literalinclude:: doc-johnson.queries
   :start-after: -- q1
   :end-before: -- q2


.. index::
    single: johnson(Complete Signature)

Complete Signature
...................

.. code-block:: none

    pgr_johnson(edges_sql, directed)
    RETURNS SET OF (start_vid, end_vid,  agg_cost) or EMPTY SET


:Example 2: On an undirected graph.

.. literalinclude:: doc-johnson.queries
   :start-after: -- q2
   :end-before: -- q3

Description of the Signatures
------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: no_id_edges_sql_start
    :end-before: no_id_edges_sql_end


Description of the parameters of the signatures
................................................

Receives  ``(edges_sql, directed)``

============= ============= =================================================
Parameter     Type          Description
============= ============= =================================================
**edges_sql** ``TEXT``      SQL query as described above.
**directed**  ``BOOLEAN``   (optional) Default is true (is directed). When set to false the graph is considered as Undirected
============= ============= =================================================


Description of the return values
..................................

Returns set of ``(start_vid, end_vid, agg_cost)``

============= ============= =================================================
Column        Type          Description
============= ============= =================================================
**start_vid** ``BIGINT``    Identifier of the starting vertex.
**end_vid**   ``BIGINT``    Identifier of the ending vertex.
**agg_cost**  ``FLOAT``     Total cost from ``start_vid`` to ``end_vid``.
============= ============= =================================================





.. rubric:: History

* Re-design of pgr_apspJohnson in Version 2.2.0

See Also
-------------------------------------------------------------------------------

* :ref:`pgr_floydWarshall`
* `Boost Johnson <http://www.boost.org/libs/graph/doc/johnson_all_pairs_shortest.html>`_ algorithm implementation.
* Queries uses the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

