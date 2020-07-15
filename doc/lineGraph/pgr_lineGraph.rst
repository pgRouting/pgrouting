..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_lineGraph - Experimental
===============================================================================

``pgr_lineGraph`` — Transforms a given graph into its corresponding edge-based graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/dijkstra_shortest_paths.html

   Boost Graph Inside

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 2.5.0

  * New **Experimental** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_lineGraph.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_lineGraph.html>`__

* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_lineGraph.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_lineGraph.html>`__

Description
-------------------------------------------------------------------------------

Given a graph G, its line graph L(G) is a graph such that:

- Each vertex of L(G) represents an edge of G
- Two vertices of L(G) are adjacent if and only if their corresponding edges share a common endpoint in G.

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_lineGraph(edges_sql, directed)
    RETURNS SET OF (seq, source, target, cost, reverse_cost)
    OR EMPTY SET

.. rubric:: Using defaults

.. code-block:: none

    pgr_lineGraph(edges_sql)
    RETURNS SET OF (seq, source, target, cost, reverse_cost) OR EMPTY SET

:Example: For a **directed** graph

.. literalinclude:: doc-pgr_lineGraph.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: lineGraph(Complete signature)

Complete Signature
...............................................................................

.. code-block:: none

    pgr_lineGraph(edges_sql, directed);
    RETURNS SET OF (seq, source, target, cost, reverse_cost) OR EMPTY SET

:Example: For an **undirected** graph

.. literalinclude:: doc-pgr_lineGraph.queries
   :start-after: -- q2
   :end-before: -- q3

Parameters
-------------------------------------------------------------------------------

======================= ====================== =================================================
Column                  Type                   Description
======================= ====================== =================================================
**edges_sql**           ``TEXT``               SQL query as described above.
**directed**            ``BOOLEAN``            * When ``true`` the graph is considered as `Directed`.
                                               * When ``false`` the graph is considered as `Undirected`.
======================= ====================== =================================================

Inner query
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

RETURNS SETOF  (seq, source, target, cost, reverse_cost)

============================ =================   ===================================================================
Column                       Type                Description
============================ =================   ===================================================================
**seq**                      ``INTEGER``         Sequential value starting from **1**.

**source**                   ``BIGINT``          Identifier of the source vertex of the current edge `id`.

                                                 * When `negative`: the source is the reverse edge in the original graph.

**target**                   ``BIGINT``          Identifier of the target vertex of the current edge `id`.

                                                 * When `negative`: the target is the reverse edge in the original graph.

**cost**                     ``FLOAT``           Weight of the edge (`source`, `target`).

                                                 * When `negative`: edge (`source`, `target`) does not exist, therefore it’s not part of the graph.

**reverse_cost**             ``FLOAT``           Weight of the edge (`target`, `source`).

                                                 * When `negative`: edge (`target`, `source`) does not exist, therefore it’s not part of the graph.
============================ =================   ===================================================================

See Also
-------------------------------------------------------------------------------

* https://en.wikipedia.org/wiki/Line_graph
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
