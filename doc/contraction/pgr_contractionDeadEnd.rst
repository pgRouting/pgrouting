..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************


.. index::
   single: Contraction Family ; pgr_contractionDeadEnd - Proposed
   single: contractionDeadEnd - Proposed

|

``pgr_contractionDeadEnd`` - Proposed
===============================================================================

``pgr_contractionDeadEnd`` — Performs graph contraction and returns the contracted
vertices and edges.

.. include:: proposed.rst
   :start-after: warning-begin
   :end-before: end-warning

.. rubric:: Availability

* Version 3.8.0

  * New proposed function.

Description
-------------------------------------------------------------------------------

.. include:: pgr_contraction.rst
   :start-after: characteristics_start
   :end-before: characteristics_end

A node is considered a dead end node when:

* On undirected graphs:

  * The number of adjacent vertices is 1.

* On directed graphs:

  * When there is only one adjacent vertex or
  * When all edges are incoming regardless of the number of adjacent vertices.

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_contractionDeadEnd(`Edges SQL`_, [**options**])

   | **options:** ``[directed, forbidden_vertices]``
   | Returns set of |result-contract|

:Example: Dead end contraction on an undirected graph.

.. literalinclude:: contractionDeadEnd.queries
   :start-after: -- q1
   :end-before: -- q2

- The green nodes are dead end nodes.

  - Node :math:`3` is a dead end node after node :math:`1` is contracted.

.. graphviz::

   graph G {
     splines=false;
     1,2,3,5,9,13 [shape=circle;style=filled;color=lightgreen;fontsize=8;width=0.3;fixedsize=true];
     4,6,7,8,10,11,12,14,15,16,17 [shape=circle;style=filled;color=cyan;fontsize=8;width=0.3;fixedsize=true];

     4 [label="4,{2}"];
     6 [label="6,{5}"];
     7 [label="7,{1,3}"];
     8 [label="8,{9}"];
     14 [label="14,{13}"];
     6 -- 10 [label="2",fontsize=8];
     10 -- 15 [label="3",fontsize=8];   6 -- 7 [label="4",fontsize=8];
     10 -- 11 [label="5",fontsize=8];
     7 -- 11 [label="8",fontsize=8];
     11 -- 16 [label="9",fontsize=8];   7 -- 8 [label="10",fontsize=8];
     11 -- 12 [label="11",fontsize=8];  8 -- 12 [label="12",fontsize=8];
     12 -- 17 [label="13",fontsize=8];
     16 -- 17 [label="15",fontsize=8]; 15 -- 16 [label="16",fontsize=8];

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

Optional parameters
...............................................................................

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

Contraction optional parameters
...............................................................................

.. contraction_optional_start

.. list-table::
   :width: 81
   :widths: 19 22 7 40
   :header-rows: 1

   * - Column
     - Type
     - Default
     - Description
   * - ``forbidden_vertices``
     - ``ARRAY[`` |ANY-INTEGER| ``]``
     - **Empty**
     - Identifiers of vertices forbidden for contraction.

.. contraction_optional_end

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

Dead end vertex on undirected graph
...............................................................................

The green nodes are dead end nodes.

- They have only one adjacent node.

.. graphviz::

   graph G {
     G [shape=record;style=filled;fillcolor=deepskyblue;
      label="{Rest of the Graph | {<5> 5 | <6> 6}}"; pos="2.5,1!"];
     1,2,3,4 [shape=circle;fontsize=8;fixedsize=true;style=filled];
     2,3 [color=deepskyblue];
     1,4 [color=green];
     {G:5,G:6} -- {2,3} [weight=1, penwidth=3];
     1 -- 2 -- 1;
     3 -- 4;
     1 [pos="1,0!"]; 2 [pos="2,0!"]; 3 [pos="3,0!"]; 4 [pos="4,0!"];
   }

.. literalinclude:: contractionDeadEnd.queries
   :start-after: -- q2
   :end-before: -- q3

.. graphviz::

   graph G {
     G [shape=record;style=filled;fillcolor=deepskyblue;
      label="{Rest of the Graph | {<5> 5 | <6> 6}}"; pos="2.5,1!"];
     2,3 [shape=circle;fontsize=8;fixedsize=true;style=filled];
     2,3 [color=deepskyblue];
     2 [label="2, {1}"]; 3 [label="3, {4}"];
     {G:5,G:6} -- {2,3} [weight=1, penwidth=3];
     2 [pos="2,0!"]; 3 [pos="3,0!"];
   }

Dead end vertex on directed graph
...............................................................................

- The green nodes are dead end nodes
- The blue nodes have an unlimited number of incoming and/or outgoing edges.

.. graphviz::

   digraph G {
     G [shape=record;style=filled;fillcolor=deepskyblue;
      label="{Rest of the Graph | { | | | | }}"; pos="2.5,3!"];
     1,2,3,4,5,6,7,8,9,10 [shape=circle;fontsize=8;fixedsize=true;style=filled];
     1,2,3,4,5,10 [color=deepskyblue];
     6,7,8,9 [color=green];

        {1,2,3,4,5} -> G [dir=both;weight=1, penwidth=3];
        1 -> 6 -> 1;
        2 -> 7;
        {3, 2} -> 8;
        9 -> 4;
        10 -> {4, 5};

     2 [pos="1,4!"]; 3 [pos="2,4!"];
     G [pos="2.5,3!"];
     1 [pos="1,1!"]; 2 [pos="2,1!"]; 3 [pos="3,1!"]; 4 [pos="4,1!"]; 5 [pos="5,1!"];
     6 [pos="1,0!"]; 7 [pos="2,0!"]; 8 [pos="3,0!"]; 9 [pos="4,0!"]; 10 [pos="5,0!"];
    }

.. list-table::
   :width: 80
   :widths: auto
   :header-rows: 1

   * - Node
     - Adjacent nodes
     - Dead end
     - Reason
   * - :math:`6`
     - :math:`\{1\}`
     - Yes
     - Has only one adjacent node.
   * - :math:`7`
     - :math:`\{2\}`
     - Yes
     - Has only one adjacent node.
   * - :math:`8`
     - :math:`\{2, 3\}`
     - Yes
     - Has more than one adjacent node and all edges are incoming.
   * - :math:`9`
     - :math:`\{4\}`
     - Yes
     - Has only one adjacent node.
   * - :math:`10`
     - :math:`\{4, 5\}`
     - No
     - Has more than one adjacent node and all edges are outgoing.
   * - :math:`1,2,3,4,5`
     - Many adjacent nodes.
     - No
     - Has more than one adjacent node and some edges are incoming and some are
       outgoing.


From above, nodes :math:`\{6, 7, 9\}` are dead ends because the
total number of adjacent vertices is one.

When there are more than one adjacent vertex, all edges need to be
all incoming edges otherwise it is not a dead end.

.. literalinclude:: contractionDeadEnd.queries
   :start-after: -- q3
   :end-before: -- q4

.. graphviz::

   digraph G {
     G [shape=record;style=filled;fillcolor=deepskyblue;
      label="{Rest of the Graph | { | | | | }}"; pos="2.5,3!"];
     1,2,3,4,5,10 [shape=circle;fontsize=8;fixedsize=true;style=filled];
     1,2,3,4,5,10 [color=deepskyblue];

     {1,2,3,4,5} -> G [dir=both;weight=1, penwidth=3];
     10 -> {4, 5};

     2 [pos="1,4!"]; 3 [pos="2,4!"];
     G [pos="2.5,3!"];
     1 [label="1, {6}";pos="1,1!"]; 2 [label="2, {7,8}";pos="2,1!"];
     3 [label="3, {8}";pos="3,1!"]; 4 [label="4, {9}";pos="4,1!"]; 5 [pos="5,1!"];
     10 [pos="5,0!"];
    }

Step by step dead end contraction
...............................................................................

The dead end contraction will stop until there are no more dead end nodes.
For example, from the following graph where :math:`3` is the dead end node:

.. graphviz::

    digraph G {
     G [shape=record;style=filled;fillcolor=deepskyblue;
      label="{Rest of the Graph | { | | | | }}"; pos="2,3!"];
     1,2,3 [shape=circle;fontsize=8;fixedsize=true;style=filled];
     1,2 [color=deepskyblue];
     3 [color=green];

     {1} -> G [dir=both;weight=1, penwidth=3];
     1 -> 2 -> 3;

     G [pos="2.5,3!"];
     1 [pos="1,1!"]; 2 [pos="2,1!"]; 3 [pos="3,1!"];
    }


After contracting :math:`3`, node :math:`2` is now a dead end node and is
contracted:

.. graphviz::

    digraph G {
     G [shape=record;style=filled;fillcolor=deepskyblue;
      label="{Rest of the Graph | { | | | | }}"; pos="2,3!"];
     1,2 [shape=circle;fontsize=8;fixedsize=true;style=filled];
     1 [color=deepskyblue];
     2 [color=green];

     {1} -> G [dir=both;weight=1, penwidth=3];
     1 -> 2;

     G [pos="2.5,3!"];
     1 [pos="1,1!"]; 2 [label="2, {3}";pos="2,1!"];
    }

After contracting :math:`2`, stop. Node :math:`1` has the information of nodes
that were contracted.

.. graphviz::

    digraph G {
     G [shape=record;style=filled;fillcolor=deepskyblue;
      label="{Rest of the Graph | { | | | | }}"; pos="2,3!"];
     1 [shape=circle;fontsize=8;fixedsize=true;style=filled];
     1 [color=deepskyblue];

     {1} -> G [dir=both;weight=1, penwidth=3];

     G [pos="2.5,3!"];
     1 [label="1, {2,3}";pos="2,1!"];
    }

.. literalinclude:: contractionDeadEnd.queries
   :start-after: -- q4
   :end-before: -- q5

Creating the contracted graph
...............................................................................

.. contents::
   :local:

Steps for the creation of the contracted graph
###############################################################################

Add additional columns.

.. literalinclude:: contractionDeadEnd.queries
   :start-after: -- cg1
   :end-before: -- cg2

Save results into a table.

.. literalinclude:: contractionDeadEnd.queries
   :start-after: -- cg2
   :end-before: -- cg3

Use ``is_contracted`` column to indicate the vertices that are contracted.

.. literalinclude:: contractionDeadEnd.queries
   :start-after: -- cg3
   :end-before: -- cg4

Fill ``contracted_vertices`` with the information from the results that belong
to the vertices.

.. literalinclude:: contractionDeadEnd.queries
   :start-after: -- cg4
   :end-before: -- cg5

The contracted vertices are not part of the contracted graph.

.. literalinclude:: contractionDeadEnd.queries
   :start-after: -- cg5
   :end-before: -- cg6

The contracted graph
###############################################################################

.. literalinclude:: contractionDeadEnd.queries
   :start-after: -- cg6
   :end-before: -- cg7


.. graphviz::

   graph G {
     splines=false;
     4,6,7,8,10,11,12,14,15,16,17 [shape=circle;style=filled;color=cyan;fontsize=8;width=0.3;fixedsize=true];

     4 [label="4,{2}"];
     6 [label="6,{5}"];
     7 [label="7,{1,3}"];
     8 [label="8,{9}"];
     14 [label="14,{13}"];
     6 -- 10 [label="2",fontsize=8];
     10 -- 15 [label="3",fontsize=8];   6 -- 7 [label="4",fontsize=8];
     10 -- 11 [label="5",fontsize=8];
     7 -- 11 [label="8",fontsize=8];
     11 -- 16 [label="9",fontsize=8];   7 -- 8 [label="10",fontsize=8];
     11 -- 12 [label="11",fontsize=8];  8 -- 12 [label="12",fontsize=8];
     12 -- 17 [label="13",fontsize=8];
     16 -- 17 [label="15",fontsize=8]; 15 -- 16 [label="16",fontsize=8];

     4 [pos="2,3.5!"];
     6 [pos="2,1!"];
     7 [pos="2,2!"];       8 [pos="2,3!"];
     10 [pos="3,1!"];
     11 [pos="3,2!"];     12 [pos="3,3!"];
     14 [pos="3.5,4!"];
     15 [pos="4,1!"];     16 [pos="4,2!"];
     17 [pos="4,3!"];
     }

Using when departure and destination are in the contracted graph
...............................................................................

.. literalinclude:: contractionDeadEnd.queries
   :language: sql
   :start-after: -- cg7
   :end-before: -- cg8

.. graphviz::

   graph G {
     splines=false;
     4,6,7,8,10,11,12,14,15,16,17 [shape=circle;style=filled;color=cyan;fontsize=8;width=0.3;fixedsize=true];

     4 [label="4,{2}"];
     6 [label="6,{5}"];
     7 [label="7,{1,3}"];
     8 [label="8,{9}"];
     14 [label="14,{13}"];
     6 -- 10 [label="2",fontsize=8];
     10 -- 15 [label="3",fontsize=8];   6 -- 7 [color=red;label="4",fontsize=8];
     10 -- 11 [label="5",fontsize=8];
     7 -- 11 [color=red;label="8",fontsize=8];
     11 -- 16 [label="9",fontsize=8];   7 -- 8 [label="10",fontsize=8];
     11 -- 12 [color=red;label="11",fontsize=8];  8 -- 12 [label="12",fontsize=8];
     12 -- 17 [color=red;label="13",fontsize=8];
     16 -- 17 [label="15",fontsize=8]; 15 -- 16 [label="16",fontsize=8];

     4 [pos="2,3.5!"];
     6 [pos="2,1!"];
     7 [pos="2,2!"];       8 [pos="2,3!"];
     10 [pos="3,1!"];
     11 [pos="3,2!"];     12 [pos="3,3!"];
     14 [pos="3.5,4!"];
     15 [pos="4,1!"];     16 [pos="4,2!"];
     17 [pos="4,3!"];
   }

Using when departure/destination is not in the contracted graph
...............................................................................

.. literalinclude:: contractionDeadEnd.queries
   :language: sql
   :start-after: -- cg8
   :end-before: -- cg9

.. graphviz::

   graph G {
     splines=false;
     1,3 [shape=circle;style=filled;color=lightgreen;fontsize=8;width=0.3;fixedsize=true];
     4,6,7,8,10,11,12,14,15,16,17 [shape=circle;style=filled;color=cyan;fontsize=8;width=0.3;fixedsize=true];

     4 [label="4,{2}"];
     6 [label="6,{5}"];
     7 [label="7,{1,3}"];
     8 [label="8,{9}"];
     14 [label="14,{13}"];
     1 -- 3 [color=red;label="6",fontsize=8];
     3 -- 7 [color=red;label="7",fontsize=8];
     6 -- 10 [label="2",fontsize=8];
     10 -- 15 [label="3",fontsize=8];   6 -- 7 [label="4",fontsize=8];
     10 -- 11 [label="5",fontsize=8];
     7 -- 11 [color=red;label="8",fontsize=8];
     11 -- 16 [color=red;label="9",fontsize=8];   7 -- 8 [label="10",fontsize=8];
     11 -- 12 [label="11",fontsize=8];  8 -- 12 [label="12",fontsize=8];
     12 -- 17 [label="13",fontsize=8];
     16 -- 17 [color=red;label="15",fontsize=8]; 15 -- 16 [label="16",fontsize=8];

     1 [pos="0,2!"];
     3 [pos="1,2!"];       4 [pos="2,3.5!"];
     6 [pos="2,1!"];
     7 [pos="2,2!"];       8 [pos="2,3!"];
     10 [pos="3,1!"];
     11 [pos="3,2!"];     12 [pos="3,3!"];
     14 [pos="3.5,4!"];
     15 [pos="4,1!"];     16 [pos="4,2!"];
     17 [pos="4,3!"];
     }

Using when departure and destination are not in the contracted graph
...............................................................................

.. literalinclude:: contractionDeadEnd.queries
   :start-after: -- cg9
   :end-before: -- cg10

.. graphviz::

   graph G {
     splines=false;
     1,3,9 [shape=circle;style=filled;color=lightgreen;fontsize=8;width=0.3;fixedsize=true];
     4,6,7,8,10,11,12,14,15,16,17 [shape=circle;style=filled;color=cyan;fontsize=8;width=0.3;fixedsize=true];

     4 [label="4,{2}"];
     6 [label="6,{5}"];
     7 [label="7,{1,3}"];
     8 [label="8,{9}"];
     14 [label="14,{13}"];
     1 -- 3 [color=red;label="6",fontsize=8];
     3 -- 7 [color=red;label="7",fontsize=8];
     8 -- 9 [color=red;label="7",fontsize=8];
     6 -- 10 [label="2",fontsize=8];
     10 -- 15 [label="3",fontsize=8];   6 -- 7 [label="4",fontsize=8];
     10 -- 11 [label="5",fontsize=8];
     7 -- 11 [label="8",fontsize=8];
     11 -- 16 [label="9",fontsize=8];   7 -- 8 [label="10",fontsize=8];
     11 -- 12 [label="11",fontsize=8];  8 -- 12 [label="12",fontsize=8];
     12 -- 17 [label="13",fontsize=8];
     16 -- 17 [label="15",fontsize=8]; 15 -- 16 [label="16",fontsize=8];

     1 [pos="0,2!"];
     3 [pos="1,2!"];       4 [pos="2,3.5!"];
     6 [pos="2,1!"];
     7 [pos="2,2!"];       8 [pos="2,3!"];
     9 [pos="2,4!"];      10 [pos="3,1!"];
     11 [pos="3,2!"];     12 [pos="3,3!"];
     14 [pos="3.5,4!"];
     15 [pos="4,1!"];     16 [pos="4,2!"];
     17 [pos="4,3!"];
     }

See Also
-------------------------------------------------------------------------------

* :doc:`contraction-family`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

