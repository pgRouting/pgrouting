..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_lineGraph:

pgr_lineGraph
===============================================================================

``pgr_lineGraph`` — Transforms a given graph into its corresponding edge-based graph.

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/dijkstra_shortest_paths.html

   Boost Graph Inside


Synopsis
-------------------------------------------------------------------------------

Given a graph G, its line graph L(G) is a graph such that:-

   - each vertex of L(G) represents an edge of G

   - two vertices of L(G) are adjacent if and only if their corresponding edges
     share a common endpoint in G.

The current implementation only works for the `directed graph`.


The following figures show a graph (left, with blue vertices) and its
Line Graph (right, with green vertices).

.. footer::

    .. class:: tablapie

    +-------------------------------------------------------------------------------------------------------------------------------------------------------+---------------------------------------------------------------------+----------------------------------+-----------------------------------+
| |first|       |second|               |third|  |fourth|

.. |first| image:: images/135px-Line_graph_construction_1.svg.png
   :align: middle

.. |second| image:: images/135px-Line_graph_construction_2.svg.png
   :align: middle

.. |third| image:: images/135px-Line_graph_construction_3.svg.png
   :align: middle

.. |fourth| image:: images/128px-Line_graph_construction_4.svg.png
   :align: middle

Handling of Costs
-------------------------------------------------------------------------------

- **Cost associated with the nodes.**
    + Consider the following graph with nodes having costs:-

    .. image:: images/Graph1.png

    The transformed **Line Graph**:-

    .. image:: images/Line-graph-with-edge-cost.png

    Each node of the transformed graph is an edge from the original graph.
    Here `[1,2]` denotes the node in the transformed graph which was an edge
    from node `1` to node `2` in the original graph.

    Each edge of the transformed graph is a tuple of nodes `(n1, n2, n3)` where
    `n1`, `n2` and `n3` are nodes from the original graph such that there was
    an edge from `n1 -> n2` and another edge from `n2 -> n3`.

    Thus, the connection `[1,4] -> [3, 4]` goes through the vertex `4` as it is
    made up of `(1, 4, 3)` tuple which has an associated cost of `6` therefore
    the corresponding edge in the above graph gets a cost of `6`.

- **Cost associated with the edges**
    + Consider the following graph with edges having costs:-

    .. image:: images/Graph2.png

    The transformed **Line Graph**:-

    .. image:: images/Line-graph-with-node-cost.png

    Here, the cost associated with an edge in the original graph moves to the
    corresponding nodes in the transformed Line Graph.

Signature Summary
-----------------

.. code-block:: none

    pgr_lineGraph(edges_sql, directed)

    RETURNS SET OF (seq, source, target, cost, reverse_cost)
        OR EMPTY SET


Signatures
-------------------------------------------------------------------------------

.. index::
    single: lineGraph(Minimal Use)

Minimal signature
.......................................

.. code-block:: none

    pgr_lineGraph(edges_sql)
    RETURNS SET OF (seq, source, target, cost, reverse_cost) or EMPTY SET

The minimal signature is for a **directed** graph:

:Example:

.. literalinclude:: doc-pgr_lineGraph.queries
   :start-after: -- q1
   :end-before: -- q2


.. index::
    single: lineGraph(Complete signature)

Complete Signature
.......................................

.. code-block:: none

    pgr_lineGraph(edges_sql, directed);
    RETURNS SET OF (seq, source, target, cost, reverse_cost) or EMPTY SET

This signature returns the Line Graph of the current graph:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

:Example:

.. literalinclude:: doc-pgr_lineGraph.queries
   :start-after: -- q2
   :end-before: -- q3



Description of the Signatures
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end


Description of the parameters of the signatures
-------------------------------------------------------------------------------

======================= ====================== =================================================
Column                  Type                   Description
======================= ====================== =================================================
**edges_sql**           ``TEXT``               SQL query as described above.
**directed**            ``BOOLEAN``            * When ``true`` the graph is considered as `Directed`.
                                               * When ``false`` the graph is considered as `Undirected`.
======================= ====================== =================================================

Description of the return values
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

Examples
-------------------------------------------------------------------------------

.. literalinclude:: doc-pgr_lineGraph.queries
   :start-after: -- q3
   :end-before: -- q4

See Also
-------------------------------------------------------------------------------

* https://en.wikipedia.org/wiki/Line_graph
* The queries use the :ref:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
