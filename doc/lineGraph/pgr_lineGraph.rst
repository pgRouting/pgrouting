..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


pgr_lineGraph - Experimental
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

Given a graph :math:`G`, its line graph :math:`L(G)` is a graph such that:

- Each vertex of :math:`L(G)` represents an edge of :math:`G`.
- Two vertices of :math:`L(G)` are adjacent if and only if their corresponding
  edges share a common endpoint in :math:`G`

**The main characteristics are:**

- Works for directed and undirected graphs.
- The ``cost`` and ``reverse_cost`` columns of the result represent existence of
  the edge.
- When the graph is directed the result is directed.
- When the graph is undirected the result is undirected.

  - The ``reverse_cost`` is always :math:`-1`.

.. index::
    single: lineGraph - Experimental on v2.5

Signatures
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_lineGraph(`Edges SQL`_, [``directed``])

   | Returns set of |result-lineg|
   | OR EMPTY SET

:Example: For an undirected graph with edges :math:'{2,4,5,8}'

.. literalinclude:: doc-pgr_lineGraph.queries
   :start-after: -- q1
   :end-before: -- q2

.. graphviz::

   graph G {

    v6 [label=6,shape=circle;style=filled;fixedsize=true;width=.4;color=deepskyblue,pos="0,0!"];
    v7 [label=7,shape=circle;style=filled;fixedsize=true;width=.4;color=deepskyblue,pos="0,2!"];
    v10 [label=10,shape=circle;style=filled;fixedsize=true;width=.4;color=deepskyblue,pos="2,0!"];
    v11 [label=11,shape=circle;style=filled;fixedsize=true;width=.4;color=deepskyblue,pos="2,2!"];

    v7--v6 [color=blue];
    v7--v11 [color=blue];
    v10--v6 [color=blue];
    v10--v11 [color=blue];

    s2 [label="2",shape=circle;style=filled;width=.4;color=yellow,pos="1,0!"];
    s4 [label="4",shape=circle;style=filled;width=.4;color=yellow,pos="0,1!"];
    s5 [label="5",shape=circle;style=filled;width=.4;color=yellow,pos="2,1!"];
    s8 [label="8",shape=circle;style=filled;width=.4;color=yellow,pos="1,2!"];

    s2--s4 [color=red];
    s2--s5 [color=red];
    s4--s8 [color=red];
    s5--s8 [color=red];
 }



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

Result columns
-------------------------------------------------------------------------------

Returns set of |result-lineg|

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

* wikipedia: `Line Graph <https://en.wikipedia.org/wiki/Line_graph>`__
* mathworld: `Line Graph <https://mathworld.wolfram.com/LineGraph.html>`__
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
