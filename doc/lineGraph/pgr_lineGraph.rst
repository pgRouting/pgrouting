..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_lineGraph.html>`__
  (`3.6 <https://docs.pgrouting.org/3.6/en/pgr_lineGraph.html>`__)
  `3.5 <https://docs.pgrouting.org/3.5/en/pgr_lineGraph.html>`__
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_lineGraph.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_lineGraph.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_lineGraph.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_lineGraph.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_lineGraph.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_lineGraph.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_lineGraph.html>`__

``pgr_lineGraph`` - Experimental
===============================================================================

``pgr_lineGraph`` — Transforms the given graph into its corresponding edge-based
graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/dijkstra_shortest_paths.html

   Boost Graph Inside

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 2.5.0

  * New **Experimental** function


Description
-------------------------------------------------------------------------------

Given a graph G, its line graph L(G) is a graph such that:

- Each vertex of L(G) represents an edge of G
- Two vertices of L(G) are adjacent if and only if their corresponding edges
  share a common endpoint in G.

.. index::
    single: lineGraph - Experimental on v2.5

Signatures
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_lineGraph(`Edges SQL`_, [``directed``])

   | RETURNS SET OF |result-lineg|
   | OR EMPTY SET

:Example: For a **directed** graph

.. literalinclude:: doc-pgr_lineGraph.queries
   :start-after: -- q1
   :end-before: -- q2

Parameters
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
   :start-after: only_edge_param_start
   :end-before: only_edge_param_end

Optional parameters
...............................................................................

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

RETURNS SET OF |result-lineg|

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``seq``
     - ``INTEGER``
     - Sequential value starting from **1**.

       - Gives a local identifier for the edge
   * - ``source``
     - ``BIGINT``
     - Identifier of the source vertex of the current edge.

       * When `negative`: the source is the reverse edge in the original graph.
   * - ``target``
     - ``BIGINT``
     - Identifier of the target vertex of the current edge.

       * When `negative`: the target is the reverse edge in the original graph.
   * - ``cost``
     - ``FLOAT``
     - Weight of the edge (``source``, ``target``).

       * When `negative`: edge (``source``, ``target``) does not exist,
         therefore it’s not part of the graph.
   * - ``reverse_cost``
     - ``FLOAT``
     - Weight of the edge (``target``, ``source``).

       * When `negative`: edge (``target``, ``source``) does not exist,
         therefore it’s not part of the graph.

See Also
-------------------------------------------------------------------------------

* https://en.wikipedia.org/wiki/Line_graph
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
