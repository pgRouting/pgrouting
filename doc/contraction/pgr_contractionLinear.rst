..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************


.. index::
   single: Contraction Family ; pgr_contractionLinear - Proposed
   single: contractionLinear - Proposed

|

``pgr_contractionLinear`` - Proposed
===============================================================================

``pgr_contractionLinear`` — Performs graph contraction and returns the contracted
vertices and edges.

.. rubric:: Availability

* Version 3.8.0

  * New proposed function.


Description
-------------------------------------------------------------------------------

.. include:: pgr_contraction.rst
   :start-after: characteristics_start
   :end-before: characteristics_end

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_contractionLinear(`Edges SQL`_, [**options**])

   | **options:** ``[directed, max_cycles, forbidden_vertices]``
   | Returns set of |result-contract|

:Example: Linear contraction on an undirected graph.

.. literalinclude:: contractionLinear.queries
   :start-after: -- q1
   :end-before: -- q2

- The green nodes are linear nodes and will not be part of the contracted graph.

  - All edges adjacent will not be part of the contracted graph.

- The red lines will be new edges of the contracted graph.

.. graphviz::

   graph G {
     splines=false;
     3,15,17 [shape=circle;style=filled;color=lightgreen;fontsize=8;width=0.3;fixedsize=true];
     1,2,4,5,6,7,8,9,10,11,12,13,14,16 [shape=circle;style=filled;color=cyan;fontsize=8;width=0.3;fixedsize=true];

     1:n -- 7:n   [label="-1",fontsize=8,color=red];
     12:s -- 17:sw -- 16:w [label="-2",fontsize=8,color=red];
     10:n -- 15:nw -- 16:w [label="-3",fontsize=8,color=red];
     5 -- 6 [label="1",fontsize=8];     6 -- 10 [label="2",fontsize=8];
     10 -- 15 [label="3",fontsize=8];   6 -- 7 [label="4",fontsize=8];
     10 -- 11 [label="5",fontsize=8];   1 -- 3 [label="6",fontsize=8];
     3 -- 7 [label="7",fontsize=8];     7 -- 11 [label="8",fontsize=8];
     11 -- 16 [label="9",fontsize=8];   7 -- 8 [label="10",fontsize=8];
     11 -- 12 [label="11",fontsize=8];  8 -- 12 [label="12",fontsize=8];
     12 -- 17 [label="13",fontsize=8];  8 -- 9 [label="",fontsize=8];
     16 -- 17 [label="15",fontsize=8]; 15 -- 16 [label="16",fontsize=8];
     2 -- 4 [label="17",fontsize=8];   13 -- 14 [label="18",fontsize=8];

     1 [pos="0,2!"];       2 [pos="0.5,3.5!"];
     3 [pos="1,2!"];       4 [pos="2,3.5!"];
     5 [pos="2,0!"];       6 [pos="2,1!"];
     7 [pos="2,2!"];       8 [pos="2,3!"];
     9 [pos="2,4!"];      10 [pos="3,1!"];
     11 [pos="3,2!"];     12 [pos="3,3!"];
     13 [pos="3.5,2.3!"]; 14 [pos="3.5,4!"];
     15 [pos="4,1!"];     16 [pos="4,2!"];
     17 [pos="4,3!"];
   }

Parameters
-------------------------------------------------------------------------------

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Parameter
     - Type
     - Description
   * - `Edges SQL`_
     - ``TEXT``
     - `Edges SQL`_ as described below.
   * - **contraction Order**
     - ``ARRAY[`` **ANY-INTEGER** ``]``
     - Ordered contraction operations.

       - 1 = Dead end contraction
       - 2 = Linear contraction

Optional parameters
...............................................................................

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

Contraction optional parameters
...............................................................................

.. list-table::
   :width: 81
   :widths: 19 22 7 40
   :header-rows: 1

   * - Column
     - Type
     - Default
     - Description
   * - ``forbidden_vertices``
     - ``ARRAY[`` **ANY-INTEGER** ``]``
     - **Empty**
     - Identifiers of vertices forbidden for contraction.
   * - ``max_cycles``
     - ``INTEGER``
     - :math:`1`
     - Number of times the contraction operations on ``contraction_order`` will
       be performed.

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Result columns
-------------------------------------------------------------------------------

Returns set of |result-contract|

The function returns a single row. The columns of the row are:

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``type``
     - ``TEXT``
     - Value = ``e`` indicating the row is an edge.
   * - ``id``
     - ``BIGINT``
     - A pseudo `id` of the edge.

       * All numbers on this column are ``DISTINCT``
       * Decreasing sequence starting from **-1**.
   * - ``contracted_vertices``
     - ``ARRAY[BIGINT]``
     - Array of contracted vertex identifiers.
   * - ``source``
     - ``BIGINT``
     - Identifier of the source vertex of the current edge.
   * - ``target``
     - ``BIGINT``
     - Identifier of the target vertex of the current edge.
   * - ``cost``
     - ``FLOAT``
     - Weight of the current edge.

Additional Examples
-------------------------------------------------------------------------------

.. contents::
   :local:

Linear edges
................................................................................

.. rubric:: Undirected graph

A node connects two (or more) `linear` edges when

* The number of adjacent vertices is 2.

.. graphviz::

   graph G {
     label = "Linear edges"
     2 [shape=circle;style=filled;color=lightgreen;fontsize=8;width=0.3;fixedsize=true];
     1,3 [shape=circle;style=filled;color=cyan;fontsize=8;width=0.3;fixedsize=true];
     1 -- 2 -- 3 -- 2;
     1 [pos="0,2!"];   2 [pos="1,2!"];   3 [pos="2,2!"];
   }

.. graphviz::

   graph G {
     label = "Non linear edges"
     4,5,6,7 [shape=circle;style=filled;color=cyan;fontsize=8;width=0.3;fixedsize=true];
     4 -- 5 -- 6 -- 5; 5 --7;
     4 [pos="0,0!"];   5 [pos="1,0!"];   6 [pos="2,0!"];
                     7 [pos="1,1!"];
   }


In case of a directed graph, a node is considered a `linear` node when

* The number of adjacent vertices is 2.
* Linearity is symmetrical.

.. graphviz::

   digraph G {
     label = "Linearity is symmetrical."
     2 [shape=circle;style=filled;color=lightgreen;fontsize=8;width=0.3;fixedsize=true];
     1,3 [shape=circle;style=filled;color=cyan;fontsize=8;width=0.3;fixedsize=true];
     1 -> 2 -> 3 -> 2 -> 1;
     1 [pos="0,2!"];   2 [pos="1,2!"];   3 [pos="2,2!"];
   }

.. graphviz::

   digraph G {
     label = "Linearity is not symmetrical."
     2 [shape=circle;style=filled;color=lightgreen;fontsize=8;width=0.3;fixedsize=true];
     1,3 [shape=circle;style=filled;color=cyan;fontsize=8;width=0.3;fixedsize=true];
     1 -> 2 -> 3 -> 2;
     1 [pos="0,2!"];   2 [pos="1,2!"];   3 [pos="2,2!"];
   }

Linearity is not symmetrical
................................................................................

.. rubric:: Directed graph

Graph where linearity is not symmetrical.

.. graphviz::

   digraph G {
     2 [shape=circle;style=filled;color=lightgreen;fontsize=8;width=0.3;fixedsize=true];
     1,3 [shape=circle;style=filled;color=cyan;fontsize=8;width=0.3;fixedsize=true];

     1 -> 2 [label="1",fontsize=8];
     2 -> 3 [label="3",fontsize=8];
     3 -> 2 [label="4",fontsize=8];

     1 [pos="0,2!"];   2 [pos="1,2!"];   3 [pos="2,2!"];
   }

When the graph is processed as a directed graph, linearity is not symmetrical,
therefore the graph can not be contracted.

.. literalinclude:: contractionLinear.queries
   :start-after: -- q2
   :end-before: -- q3

.. rubric:: Undirected graph

When the same graph is processed as an undirected graph, linearity is symmetrical,
therefore the graph can be contracted.

.. graphviz::

   graph G {
     2 [shape=circle;style=filled;color=lightgreen;fontsize=8;width=0.3;fixedsize=true];
     1,3 [shape=circle;style=filled;color=cyan;fontsize=8;width=0.3;fixedsize=true];
     1 -- 2 [label="1",fontsize=8];
     2 -- 3 [label="3",fontsize=8];
     3 -- 2 [label="4",fontsize=8];
     1 [pos="0,2!"];   2 [pos="1,2!"];   3 [pos="2,2!"];
   }

.. literalinclude:: contractionLinear.queries
   :start-after: -- q3
   :end-before: -- q4

The three edges can be replaced by one undirected edge

- Edge :math:`1 - 3`.

  - With cost: :math:`4`.
  - Contracted vertices in the edge: :math:`\{2\}`.

.. graphviz::

   graph G {
     1,3 [shape=circle;style=filled;color=cyan;fontsize=8;width=0.3;fixedsize=true];
     1 -- 3 [label="4, {2}",fontsize=8;color=red];
     1 [pos="0,2!"]; 3 [pos="2,2!"];
   }


Linearity is symmetrical
................................................................................

.. rubric:: Directed graph

Graph where linearity is symmetrical.

.. graphviz::

   digraph G {
     2 [shape=circle;style=filled;color=lightgreen;fontsize=8;width=0.3;fixedsize=true];
     1,3 [shape=circle;style=filled;color=cyan;fontsize=8;width=0.3;fixedsize=true];

     1 -> 2 [label="1",fontsize=8];
     2 -> 1 [label="2",fontsize=8];
     2 -> 3 [label="3",fontsize=8];
     3 -> 2 [label="4",fontsize=8];

     1 [pos="0,2!"];   2 [pos="1,2!"];   3 [pos="2,2!"];
   }

When the graph is processed as a directed graph, linearity is not symmetrical,
therefore the graph can not be contracted.

.. literalinclude:: contractionLinear.queries
   :start-after: -- q4
   :end-before: -- q5

The four edges can be replaced by two directed edges.

- Edge :math:`1 - 3`.

  - With cost: :math:`4`.
  - Contracted vertices in the edge: :math:`\{2\}`.

- Edge :math:`3 - 1`.

  - With cost: :math:`6`.
  - Contracted vertices in the edge: :math:`\{2\}`.

.. graphviz::

   digraph G {
     1,3 [shape=circle;style=filled;color=cyan;fontsize=8;width=0.3;fixedsize=true];

     1 -> 3 [label="4, {2}",fontsize=8;color=red];
     3 -> 1 [label="6, {2}",fontsize=8;color=red];

     1 [pos="0,2!"]; 3 [pos="2,2!"];
   }

.. rubric:: Undirected graph

When the same graph is processed as an undirected graph, linearity is symmetrical,
therefore the graph can be contracted.

.. graphviz::

   graph G {
     2 [shape=circle;style=filled;color=lightgreen;fontsize=8;width=0.3;fixedsize=true];
     1,3 [shape=circle;style=filled;color=cyan;fontsize=8;width=0.3;fixedsize=true];
     1 -- 2 [label="1",fontsize=8];
     2 -- 1 [label="2",fontsize=8];
     2 -- 3 [label="3",fontsize=8];
     3 -- 2 [label="4",fontsize=8];
     1 [pos="0,2!"];   2 [pos="1,2!"];   3 [pos="2,2!"];
   }

.. literalinclude:: contractionLinear.queries
   :start-after: -- q5
   :end-before: -- q6

The four edges can be replaced by one undirected edge.

- Edge :math:`1 - 3`.

  - With cost: :math:`4`.
  - Contracted vertices in the edge: :math:`\{2\}`.

.. graphviz::

   graph G {
     1,3 [shape=circle;style=filled;color=cyan;fontsize=8;width=0.3;fixedsize=true];

     1 -- 3 [label="4, {2}",fontsize=8;color=red];

     1 [pos="0,2!"]; 3 [pos="2,2!"];
   }

Step by step linear contraction
................................................................................

The linear contraction will stop when there are no more linear edges.
For example from the following graph there are linear edges

.. graphviz::

    digraph G {
        1, 2, 3, 4, G [fontsize=8;fixedsize=true;style=filled];
        1, 2, 3, 4 [shape=circle];
        1, 4 [color=deepskyblue];
        2, 3 [color=green];
        G [shape=tripleoctagon;width=1.5;color=deepskyblue;label = "Rest of the Graph"];
        G -> {1, 4} [dir=none, weight=1, penwidth=3];
        1 -> 2 [label="1";fontsize=8;fixedsize=true];
        2 -> 3 [label="1";fontsize=8;fixedsize=true];
        3 -> 4 [label="1";fontsize=8;fixedsize=true];
        G [pos="1,1!"];
        1 [pos="0,0!"]; 2 [pos="1,0!"]; 3 [pos="2,0!"]; 4 [pos="3,0!"];
    }

Contracting vertex :math:`3`,

* The vertex :math:`3` is removed from the graph
* The edges :math:`2 \rightarrow 3` and :math:`w \rightarrow z` are removed
  from the graph.
* A new edge :math:`2 \rightarrow 4` is inserted represented with red color.

.. graphviz::

    digraph G {
        1, 2, 4, G [fontsize=8;fixedsize=true;style=filled];
        1, 2, 4 [shape=circle];
        1, 4 [color=deepskyblue];
        2 [color=green];
        G [shape=tripleoctagon;width=1.5;color=deepskyblue;label = "Rest of the Graph"];
        G -> {1, 4} [dir=none, weight=1, penwidth=3];
        1 -> 2 [label="1";fontsize=8;fixedsize=true];
        2 -> 4 [label="2, {3}";color=red;fontsize=8;fixedsize=true];
        G [pos="1,1!"];
        1 [pos="0,0!"]; 2 [pos="1,0!"]; 4 [pos="3,0!"];
    }

Contracting vertex :math:`2`:

* The vertex :math:`2` is removed from the graph
* The edges :math:`1 \rightarrow 2` and :math:`2 \rightarrow 3` are removed
  from the graph.
* A new edge :math:`1 \rightarrow 3` is inserted represented with red color.

.. graphviz::

    digraph G {
        1, 4, G [fontsize=8;fixedsize=true;style=filled];
        1, 4 [shape=circle];
        1, 4 [color=deepskyblue];
        G [shape=tripleoctagon;width=1.5;color=deepskyblue;label = "Rest of the Graph"];
        G -> {1, 4} [dir=none, weight=1, penwidth=3];
        1 -> 4 [label="3, {2,3}";color=red;fontsize=8;fixedsize=true]
        G [pos="1,1!"];
        1 [pos="0,0!"]; 4 [pos="3,0!"];
    }

Edge :math:`1 \rightarrow 3` has the information of cost and the nodes that were
contracted.

.. literalinclude:: contractionLinear.queries
   :start-after: -- q6
   :end-before: -- q7

Creating the contracted graph
...............................................................................

.. contents::
   :local:

Steps for the creation of the contracted graph
###############################################################################

Add additional columns.

.. literalinclude:: contractionLinear.queries
   :start-after: -- cg1
   :end-before: -- cg2

Save results into a table.

.. literalinclude:: contractionLinear.queries
   :start-after: -- cg2
   :end-before: -- cg3

Use ``is_contracted`` column to indicate the vertices that are contracted.

.. literalinclude:: contractionLinear.queries
   :start-after: -- cg3
   :end-before: -- cg4

The contracted vertices are not part of the contracted graph.

.. literalinclude:: contractionLinear.queries
   :start-after: -- cg4
   :end-before: -- cg5

Insert the new edges generated by pgr_contraction.

.. literalinclude:: contractionLinear.queries
   :start-after: -- cg5
   :end-before: -- cg6

Create the contracted graph.

.. literalinclude:: contractionLinear.queries
   :start-after: -- cg6
   :end-before: -- cg7

The contracted graph
###############################################################################

.. literalinclude:: contractionLinear.queries
   :start-after: -- cg7
   :end-before: -- cg8

.. graphviz::

   graph G {
     splines=false;
     1,2,4,5,6,7,8,9,10,11,12,13,14,16 [shape=circle;style=filled;color=cyan;fontsize=8;width=0.3;fixedsize=true];

     1 -- 7   [label="19, (2, {3})",fontsize=8];
     12 -- 16 [label="20, (2, {17})",fontsize=8];
     10 -- 16 [label="21, (2, {15})",fontsize=8];
     5 -- 6 [label="1",fontsize=8];     6 -- 10 [label="2",fontsize=8];
     6 -- 7 [label="4",fontsize=8];
     10 -- 11 [label="5",fontsize=8];
     7 -- 11 [label="8",fontsize=8];
     11 -- 16 [label="9",fontsize=8];   7 -- 8 [label="10",fontsize=8];
     11 -- 12 [label="11",fontsize=8];  8 -- 12 [label="12",fontsize=8];
     8 -- 9 [label="",fontsize=8];
     2 -- 4 [label="17",fontsize=8];   13 -- 14 [label="18",fontsize=8];

     1 [pos="0,2!"];       2 [pos="0.5,3.5!"];
     4 [pos="2,3.5!"];
     5 [pos="2,0!"];       6 [pos="2,1!"];
     7 [pos="2,2!"];       8 [pos="2,3!"];
     9 [pos="2,4!"];      10 [pos="3,1!"];
     11 [pos="3,2!"];     12 [pos="3,3!"];
     13 [pos="3.5,2.3!"]; 14 [pos="3.5,4!"];
     16 [pos="4,2!"];
   }

Using when departure and destination are in the contracted graph
...............................................................................

.. literalinclude:: contractionLinear.queries
   :start-after: -- cg8
   :end-before: -- cg9

.. graphviz::

   graph G {
     splines=false;
     1,2,4,5,6,7,8,9,10,11,12,13,14,16 [shape=circle;style=filled;color=cyan;fontsize=8;width=0.3;fixedsize=true];

     1 -- 7   [label="19, (2, {3})",fontsize=8];
     12 -- 16 [label="20, (2, {17})",fontsize=8];
     10 -- 16 [label="21, (2, {15})",fontsize=8];
     5 -- 6 [label="1",fontsize=8];     6 -- 10 [label="2",fontsize=8];
     6 -- 7 [label="4",fontsize=8];
     10 -- 11 [label="5",fontsize=8];
     7 -- 11 [label="8",fontsize=8;color=red];
     11 -- 16 [label="9",fontsize=8;color=red];   7 -- 8 [label="10",fontsize=8];
     11 -- 12 [label="11",fontsize=8];  8 -- 12 [label="12",fontsize=8];
     8 -- 9 [label="",fontsize=8];
     2 -- 4 [label="17",fontsize=8];   13 -- 14 [label="18",fontsize=8];

     1 [pos="0,2!"];       2 [pos="0.5,3.5!"];
     4 [pos="2,3.5!"];
     5 [pos="2,0!"];       6 [pos="2,1!"];
     7 [pos="2,2!"];       8 [pos="2,3!"];
     9 [pos="2,4!"];      10 [pos="3,1!"];
     11 [pos="3,2!"];     12 [pos="3,3!"];
     13 [pos="3.5,2.3!"]; 14 [pos="3.5,4!"];
     16 [pos="4,2!"];
   }

Using when departure/destination is not in the contracted graph
...............................................................................

.. literalinclude:: contractionLinear.queries
   :start-after: -- cg9
   :end-before: -- cg10

.. graphviz::

   graph G {
     17 [shape=circle;style=filled;color=lightgreen;fontsize=8;width=0.3;fixedsize=true];
     1,2,4,5,6,7,8,9,10,11,12,13,14,16 [shape=circle;style=filled;color=cyan;fontsize=8;width=0.3;fixedsize=true];

     1 -- 7   [label="19, (2, {3})",fontsize=8;color=red];
     12 -- 16 [label="20, (2, {17})",fontsize=8];
     10 -- 16 [label="21, (2, {15})",fontsize=8];
     5 -- 6 [label="1",fontsize=8];     6 -- 10 [label="2",fontsize=8];
     6 -- 7 [label="4",fontsize=8];
     10 -- 11 [label="5",fontsize=8];
     7 -- 11 [label="8",fontsize=8;color=red]; 12 -- 17 [label="13",fontsize=8];
     11 -- 16 [label="9",fontsize=8;color=red];   7 -- 8 [label="10",fontsize=8];
     11 -- 12 [label="11",fontsize=8];  8 -- 12 [label="12",fontsize=8];
     8 -- 9 [label="",fontsize=8]; 16 -- 17 [label="15",fontsize=8;color=red];
     2 -- 4 [label="17",fontsize=8];   13 -- 14 [label="18",fontsize=8];

     1 [pos="0,2!"];       2 [pos="0.5,3.5!"];
     4 [pos="2,3.5!"];
     5 [pos="2,0!"];       6 [pos="2,1!"];
     7 [pos="2,2!"];       8 [pos="2,3!"];
     9 [pos="2,4!"];      10 [pos="3,1!"];
     11 [pos="3,2!"];     12 [pos="3,3!"];
     13 [pos="3.5,2.3!"]; 14 [pos="3.5,4!"];
     16 [pos="4,2!"]; 17 [pos="4,3!"];
   }

See Also
-------------------------------------------------------------------------------

* :doc:`contraction-family`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

