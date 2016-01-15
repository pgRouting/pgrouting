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
pair of nodes in the graph using Johnson's algorithm.

.. figure:: ../../../doc/src/introduction/images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/johnson_all_pairs_shortest.html

   Boost Graph Inside


The Johnson algorithm, is a good choice to calculate the sum of the costs
of the shortest path for each pair of nodes in the graph, for *sparce graphs*.
We make use of the  Boost's implementation which runs in :math:`O(V E \log V)` time,

    
Signature
===============================================================================


.. index::
    single: johnson(edges_sql, directed)

.. code-block:: none
   
    pgr_johnson(edges_sql, directed:=true)
         RETURNS SET OF (start_vid, end_vid, agg_cost) or EMPTY SET

Example
-------
.. code-block:: none
   
    pgr_johnson(
       'SELECT source, target, cost, reverse_cost FROM edge_table WHERE city_code = 304'
    );

Characteristics:
----------------

The main Characteristics are:
  - It does not return a path.
  - Returns the sum of the costs of the shortest path for each pair of nodes in the graph.
  - Process is done only on edges with positive costs.
  - Boost returns a :math:`V \times V` matrix, where the infinity values.
    represent the distance between vertices for which there is no path.

    - We return only the non infinity values in form of a set of `(start_vid, end_vid, agg_cost)`.

  - Let be the case the values returned are stored in a table, so the unique index would be the pair:
    `(start_vid, end_vid)`.

  - For the undirected graph, the results are symetric.

    - The  `agg_cost` of `(u, v)` is the same as for `(v, u)`.

  - When  `start_vid` = `end_vid`, the `agg_cost` = 0.


Description of the Signature
============================

Description of the SQL query
-------------------------------------------------------------------------------

:edges_sql: is an SQL query, which should return a set of rows with the following columns:

================  ===================   =================================================
Column            Type                      Description
================  ===================   =================================================
**source**        ``ANY-INTEGER``       Identifier of the first end point vertex of the edge.
**target**        ``ANY-INTEGER``       Identifier of the second end point vertex of the edge.
**cost**          ``ANY-NUMERICAL``     Weight of the edge `(source, target)`, If negative: edge `(source, target)` does not exist, therefore it's not part of the graph.
**reverse_cost**  ``ANY-NUMERICAL``     (optional) Weight of the edge `(target, source)`, if negative: edge `(target, source)` does not exist, therefore it's not part of the graph.
================  ===================   =================================================

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, REAL, FLOAT


Description of the parameters of the signatures
-------------------------------------------------------------------------------

Recives  ``(edges_sql, directed)``

============= ============= =================================================
Parameter     Type          Description
============= ============= =================================================
**edges_sql** ``TEXT``      SQL query as decribed above.
**directed**  ``BOOLEAN``   (optional) Default is true (is directed). When set to false the graph is considered as Undirected
============= ============= =================================================


Description of the return values
-------------------------------------------------------------------------------

Returns set of ``(start_vid, end_vid, agg_cost)``

============= ============= =================================================
Column        Type          Description
============= ============= =================================================
**start_vid** ``BIGINT``    Identifier of the starting vertex.
**end_vid**   ``BIGINT``    Identifier of the ending vertex.
**agg_cost**  ``FLOAT``     Total cost from ``start_vid`` to ``end_vid``.
============= ============= =================================================



Examples
============================

:Example 1: On a directed graph.

.. literalinclude:: doc-johnson.queries
   :start-after: -- q1
   :end-before: -- q2

:Example 2: On an undirected graph.

.. literalinclude:: doc-johnson.queries
   :start-after: -- q2


These queries uses the :ref:`sampledata` network.


.. rubric:: History

* Re-design of pgr_apspJohnson in version 2.2.0

See Also
-------------------------------------------------------------------------------

* :ref:`pgr_floydWarshall`
* `Boost Jhonson <http://www.boost.org/libs/graph/doc/johnson_all_pairs_shortest.html>`_ algorithm implementation.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

