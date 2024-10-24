..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


pgr_lineGraph - Proposed
===============================================================================

``pgr_lineGraph`` — Transforms the given graph into its corresponding edge-based
graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/dijkstra_shortest_paths.html

   Boost Graph Inside

.. include:: proposed.rst
   :start-after: stable-begin-warning
   :end-before: stable-end-warning

.. rubric:: Availability

* Version 3.7.0

  * Promoted to **proposed** signature.
  * Works for directed and undirected graphs.

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

  - To get the complete Line Graph use unique identifiers on the double way
    edges (See `Additional Examples`_).

- When the graph is undirected the result is undirected.

  - The ``reverse_cost`` is always :math:`-1`.

.. index::
    single: lineGraph - Proposed on v3.7

Signatures
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_lineGraph(`Edges SQL`_, [``directed``])

   | Returns set of |result-lineg|
   | OR EMPTY SET

:Example: For an undirected graph with edges :math:'{2,4,5,8}'

.. literalinclude:: lineGraph.queries
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


Additional Examples
-------------------------------------------------------------------------------

.. contents::
   :local:

Given the following directed graph

:math:`G(V,E) = G(\{1,2,3,4\},\{
1 \rightarrow 2,
1 \rightarrow 4,
2 \rightarrow 3,
3 \rightarrow 1,
3 \rightarrow 2,
3 \rightarrow 4,
4 \rightarrow 3\})`

.. graphviz::

   digraph G {

      subgraph clusterA {
         style=invis;
         edge [arrowsize=0.5,color=blue];
         node [shape=circle;style=filled;fontsize=8;fixedsize=true;width=.4;color=deepskyblue];
         v1 [label=1,pos="0,2!"];
         v2 [label=2,pos="2,2!"];
         v3 [label=3,pos="2,0!"];
         v4 [label=4,pos="0,0!"];

         v1->{v2,v4} [color=blue];
         v3->{v2,v4} [dir=both,color=blue];
         v3->v1 [arrowsize=0.5,color=blue ];
      }
   }


Representation as directed with shared edge identifiers
...............................................................................

For the simplicity, the design of the edges table on the database, has the
edge's identifiers are represented with 3 digits:

:hundreds: the source vertex
:tens: always 0, acts as a separator
:units: the target vertex

In this image,

* Single or double head arrows represent one edge (row) on the edges table.
* The numbers in the yellow shadow are the edge identifiers.

.. graphviz::

   digraph G {

      subgraph clusterA {
         style=invis;
         edge [arrowsize=0.5,color=blue];
         node [shape=circle;style=filled;fontsize=8;fixedsize=true;width=.4;color=deepskyblue];
         v1 [label=1,pos="0,2!"];
         v2 [label=2,pos="2,2!"];
         v3 [label=3,pos="2,0!"];
         v4 [label=4,pos="0,0!"];

         v1->{v2,v4} [color=blue];
         v3->{v2,v4} [dir=both,color=blue];
         v3->v1 [arrowsize=0.5,color=blue ];
      }

      subgraph clusterB {
         style=invis;
         edge [arrowsize=0.5,color=red,fontsize=10,fontcolor=red];
         node [shape=circle;style=filled;fontsize=8;fixedsize=true;width=.4;color=yellow]

         s102 [label="102",pos="1,2!"];
         s104 [label="104",pos="0,1!"];
         s301 [label="301",pos="1,1!"];
         s203 [label="203",pos="2,1!"];
         s304 [label="304",pos="1,0!"];
      }
    }

Two pair of edges share the same identifier when the ``reverse_cost`` column is
used.

* Edges :math:`{2 \rightarrow 3, 3 \rightarrow 2}` are represented with one edge
  row with :math:`id=203`.
* Edges :math:`{3 \rightarrow 4, 4 \rightarrow 3}` are represented with one edge
  row with :math:`id=304`.

The graph can be created as follows:

.. literalinclude:: lineGraph.queries
   :start-after: -- q2
   :end-before: INSERT 0 5


Line Graph of a directed graph represented with shared edges
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: lineGraph.queries
   :start-after: INSERT 0 5
   :end-before: -- q3

* The result is a directed graph.
* For :math:`seq=4` from :math:`203 \leftrightarrow 304` represent two edges
* For all the other values of ``seq`` represent one edge.
* The ``cost`` and ``reverse_cost`` values represent the existence of the edge.

  * When positive: the edge exists.
  * When negative: the edge does not exist.

.. graphviz::

   digraph G {

      subgraph clusterA {
         style=invis;
         edge [arrowsize=0.5,color=blue];
         node [shape=circle;style=filled;fontsize=8;fixedsize=true;width=.4;color=deepskyblue];
         v1 [label=1,pos="0,4!"];
         v2 [label=2,pos="4,4!"];
         v3 [label=3,pos="4,0!"];
         v4 [label=4,pos="0,0!"];

         v1->{v2,v4} [color=blue];
         v3->{v2,v4} [dir=both,color=blue];
         v3->v1 [arrowsize=0.5,color=blue ];
      }

      subgraph clusterB {
         style=invis;
         edge [arrowsize=0.5,labelfloat=true,color=red,fontsize=14,fontcolor=red];
         node [shape=circle;style=filled;fontsize=8;fixedsize=true;width=.4;color=yellow]

         s102 [label="102",pos="2,4!"];
         s104 [label="104",pos="0,2!"];
         s301 [label="301",pos="2,2!"];
         s203 [label="203",pos="4,2!"];
         s304 [label="304",pos="2,0!"];

         s102 -> s203 [label=1];
         s104 -> s304 [label=2];
         s203 -> s203 [label=3,dir=both];
         s203 -> s301 [label=4];
         s203 -> s304 [label=5,dir=both];
         s301 -> s102 [label=6];
         s301 -> s104 [label=7];
         s304 -> s301 [label=8];
         s304 -> s304 [label=9,dir=both];
      }
    }

Representation as directed with unique edge identifiers
...............................................................................

For the simplicity, the design of the edges table on the database, has the
edge's identifiers are represented with 3 digits:

:hundreds: the source vertex
:tens: always 0, acts as a separator
:units: the target vertex

In this image,

* Single head arrows represent one edge (row) on the edges table.
* There are no double head arrows
* The numbers in the yellow shadow are the edge identifiers.

.. graphviz::

    digraph G {

      subgraph clusterA {
        style=invis;
        edge [arrowsize=0.5,color=blue];
        node [shape=circle;style=filled;fontsize=8;fixedsize=true;width=.4;color=deepskyblue]
        v1 [label=1,pos="0,2!"];
        v2 [label=2,pos="2,2!"];
        v3 [label=3,pos="2,0!"];
        v4 [label=4,pos="0,0!"];

        v1->{v2,v4};
        v3->{v1,v2,v4};
        {v4,v2}->v3;
      }

      subgraph clusterB {
        style=invis;
        edge [arrowsize=0.5,color=red,fontsize=6,fontcolor=red];
        node [shape=circle;style=filled;fontsize=8;fixedsize=true;width=.4;color=yellow]

        sa [label="102",pos="1,2!"];
        sb [label="203",pos="2.2,1!"];
        sc [label="302",pos="1.8,1!"];
        sd [label="104",pos="0,1!"];
        se [label="403",pos="1,0.2!"];
        sf [label="304",pos="1,-0.2!"];
        sg [label="301",pos="1,1!"];
      }
   }

Two pair of edges share the same ending nodes and the ``reverse_cost`` column is
not used.

* Edges :math:`{2 \rightarrow 3, 3 \rightarrow 2}` are represented with two
  edges :math:`id=203` and :math:`id=302` respectively.
* Edges :math:`{3 \rightarrow 4, 4 \rightarrow 3}` are represented with two
  edges :math:`id=304` and :math:`id=403` respectively.

The graph can be created as follows:

.. literalinclude:: lineGraph.queries
   :start-after: -- q3
   :end-before: INSERT 0 7

Line Graph of a directed graph represented with unique edges
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: lineGraph.queries
   :start-after: INSERT 0 7
   :end-before: -- q4

* The result is a directed graph.
* For :math:`seq=7` from :math:`203 \leftrightarrow 302` represent two edges.
* For :math:`seq=8` from :math:`304 \leftrightarrow 403` represent two edges.
* For all the other values of ``seq`` represent one edge.
* The ``cost`` and ``reverse_cost`` values represent the existence of the edge.

  * When positive: the edge exists.
  * When negative: the edge does not exist.

.. graphviz::

    digraph G {

      subgraph clusterA {
        style=invis;
        edge [arrowsize=0.5,color=blue];
        node [shape=circle;style=filled;fontsize=10;fixedsize=true;width=.4;color=deepskyblue]
        v1 [label=1,pos="0,4!"];
        v2 [label=2,pos="4,4!"];
        v3 [label=3,pos="4,0!"];
        v4 [label=4,pos="0,0!"];

        v1->{v2,v4};
        v3->{v1,v2,v4};
        {v4,v2}->v3;
      }

      subgraph clusterB {
        style=invis;
        edge [arrowsize=0.5,labelfloat=true,color=red,fontsize=14,fontcolor=red];
        node [shape=circle;style=filled;fontsize=8;fixedsize=true;width=.4;color=yellow]

        sa [label="102",pos="2,4!"];
        sb [label="203",pos="4.4,2!"];
        sc [label="302",pos="3.6,2!"];
        sd [label="104",pos="0,2!"];
        se [label="403",pos="2,0.4!"];
        sf [label="304",pos="2,-0.4!"];
        sg [label="301",pos="2,2!"];

        sa -> sb [label=1];
        sd -> se [label=2];
        sb -> sg [label=3];
        sb -> sf [label=4];
        sg -> sa [label=5];
        sg -> sd [label=6];
        sc -> sb [dir=both,label=7];
        sf -> se [dir=both,label=8];
        se -> sg [label=9];
        se -> sc [label=10];
      }
   }


See Also
-------------------------------------------------------------------------------

* wikipedia: `Line Graph <https://en.wikipedia.org/wiki/Line_graph>`__
* mathworld: `Line Graph <https://mathworld.wolfram.com/LineGraph.html>`__
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
