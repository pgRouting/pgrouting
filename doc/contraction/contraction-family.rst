..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

Contraction - Family of functions (Experimental)
===============================================================================


.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. index from here

* :doc:`pgr_contraction`

.. index to here

.. toctree::
    :hidden:

    pgr_contraction

Introduction
-------------------------------------------------------------------------------

In large graphs, like the road graphs, or electric networks, graph contraction
can be used to speed up some graph algorithms.
Contraction reduces the size of the graph by removing some of the vertices and
edges and, for example, might add edges that represent a sequence of original
edges decreasing the total time and space used in graph algorithms.

This implementation gives a flexible framework for adding contraction algorithms
in the future, currently, it supports two algorithms:

1. Dead end contraction
2. Linear contraction

Allowing the user to:

- Forbid contraction on a set of nodes.
- Decide the order of the contraction algorithms and set the maximum number of
  times they are to be executed.

.. note:: UNDER DISCUSSION:  Forbid contraction on a set of edges

Dead end contraction
-------------------------------------------------------------------------------
In the algorithm, dead end contraction is represented by 1.

Dead end
..............................................................................

In case of an undirected graph, a node is considered a `dead end` node when

* `The number of adjacent vertices is 1.`_

In case of a directed graph, a node is considered a `dead end` node when

* `The number of adjacent vertices is 1.`_

  * `There are no outgoing edges and has at least one incoming edge.`_
  * `There are no incomming edges and has at least one outgoing edge.`_


When the conditions are true then the `Operation: Dead End Contraction`_ can be done.

The number of adjacent vertices is 1.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

- The green nodes are `dead end`_ nodes
- The blue nodes have an unlimited number of incoming and outgoing edges.

.. rubric:: Directed graph

.. graphviz::

    digraph G {
        u, v [shape=circle;style=filled;width=.4;color=deepskyblue];
        w, z [style=filled; color=green];
        G [shape=tripleoctagon;width=1.5;style=filled;color=deepskyblue;label = "Rest of the Graph"];

        rankdir=LR;
        G -> {u, v} [dir=none, weight=1, penwidth=3];
        u -> w -> u;
        v -> z;
    }

.. rubric:: Undirected graph

.. graphviz::

    graph G {
        u, v [shape=circle;style=filled;width=.4;color=deepskyblue];
        w, z [style=filled; color=green];
        G [shape=tripleoctagon;width=1.5;style=filled;color=deepskyblue;label = "Rest of the Graph"];

        rankdir=LR;
        G -- {u, v} [dir=none, weight=1, penwidth=3];
        u -- w [color=black];
        u -- w [color=darkgray];
        v -- z;
    }


There are no outgoing edges and has at least one incoming edge.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

- The green nodes are `dead end`_ nodes
- The blue nodes have an unlimited number of incoming and outgoing edges.

.. rubric:: Directed graph

.. graphviz::

    digraph G {
        u, v [shape=circle;style=filled;width=.4;color=deepskyblue];
        w, z [style=filled; color=green];
        G [shape=tripleoctagon;width=1.5;style=filled;color=deepskyblue;label = "Rest of the Graph"];

        rankdir=LR;
        G -> {u, v} [dir=none, weight=1, penwidth=3];
        u -> w;
        v -> w;
        v -> z;
    }

There are no incomming edges and has at least one outgoing edge.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

- The green nodes are `dead end`_ nodes
- The blue nodes have an unlimited number of incoming and outgoing edges.
- Considering that the nodes are `dead starts` nodes

.. rubric:: Directed graph

.. graphviz::

    digraph G {
        u, v [shape=circle;style=filled;width=.4;color=deepskyblue];
        w, z [style=filled; color=green];
        G [shape=tripleoctagon;width=1.5;style=filled;color=deepskyblue;label = "Rest of the Graph"];

        rankdir=LR;
        {u, v} -> G [dir=none, weight=1, penwidth=3];
        w -> u;
        w -> v;
        z -> v;
    }


Operation: Dead End Contraction
...............................................................................

The dead end contraction will stop until there are no more dead end nodes.
For example from the following graph where ``w`` is the `dead end`_ node:

.. graphviz::

    digraph G {
        u, v [shape=circle;style=filled;width=.4;color=deepskyblue];
        w [style=filled; color=green];
        "G" [shape=tripleoctagon;style=filled;color=deepskyblue; label = "Rest of the Graph"];

        rankdir=LR;
        G -> u [dir=none, weight=1, penwidth=3];
        u -> v -> w;
    }


After contracting ``w``, node ``v`` is now a `dead end`_ node and is contracted:

.. graphviz::

    digraph G {
        u [shape=circle;style=filled;width=.4;color=deepskyblue];
        v [style=filled; color=green, label="v{w}"];
        "G" [shape=tripleoctagon;style=filled;color=deepskyblue; label = "Rest of the Graph"];

        rankdir=LR;
        G -> u [dir=none, weight=1, penwidth=3];
        u -> v;
    }

After contracting ``v``, stop. Node ``u`` has the information of nodes that were contrcted.

.. graphviz::

    digraph G {
        u [style=filled; color=green, label="u{v,w}"];
        "G" [shape=tripleoctagon;style=filled;color=deepskyblue; label = "Rest of the Graph"];

        rankdir=LR;
        G -> u [dir=none, weight=1, penwidth=3];
    }

Node ``u`` has the information of nodes that were contracted.


Linear contraction
-------------------------------------------------------------------------------
In the algorithm, linear contraction is represented by 2.

Linear
................................................................................

In case of an undirected graph, a node is considered a `linear` node when

* `The number of adjacent vertices is 2.`_

In case of a directed graph, a node is considered a `linear` node when

* `The number of adjacent vertices is 2.`_

  * `Linearity is symetrical`_

The number of adjacent vertices is 2.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

- The green nodes are `linear`_ nodes
- The blue nodes have an unlimited number of incoming and outgoing edges.

.. rubric:: Directed

.. graphviz::

    digraph G {
        u, c, a, w [shape=circle;style=filled;width=.4;color=deepskyblue];
        v, b [style=filled; color=green];
        G [shape=tripleoctagon;width=1.5;style=filled;color=deepskyblue;label = "Rest of the Graph"];

        rankdir=LR;
        {w, c} -> G -> {u, a} [dir=none, weight=1, penwidth=3];
        u -> v -> w;
        a -> b -> c;
        c -> b -> a[color=darkgray];
    }

.. rubric:: Undirected

.. graphviz::

    graph G {
        u, w [shape=circle;style=filled;width=.4;color=deepskyblue];
        v [style=filled; color=green];
        G [shape=tripleoctagon;width=1.5;style=filled;color=deepskyblue;label = "Rest of the Graph"];

        rankdir=LR;
        w -- G -- u [dir=none, weight=1, penwidth=3];
        u -- v -- w;
    }

Linearity is symetrical
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Using a contra example, vertex ``v`` is not linear because it's not possible to
go from ``w`` to ``u`` via ``v``.

.. graphviz::

    digraph G {
        u, w, v [shape=circle;style=filled;width=.4;color=deepskyblue];
        G [shape=tripleoctagon;width=1.5;style=filled;color=deepskyblue;label = "Rest of the Graph"];

        rankdir=LR;
        {w} -> G -> {u} [dir=none, weight=1, penwidth=3];
        u -> v -> w -> v;
    }

Operation: Linear Contraction
...............................................................................

The linear contraction will stop until there are no more linear nodes.
For example from the following graph where ``v`` and ``w`` are `linear`_ nodes:

.. graphviz::

    digraph G {
        u, z [shape=circle;style=filled;color=deepskyblue];
        v, w [style=filled; color=green];
        "G" [shape=tripleoctagon; style=filled;color=deepskyblue;label = "Rest of the Graph"];

        rankdir=LR;
        G -> {u, z} [dir=none, weight=1, penwidth=3];
        u -> v -> w -> z;
    }

After contracting ``w``,

* The vertex ``w`` is removed from the graph

  * The edges :math:`v \rightarrow w` and  :math:`w \rightarrow z` are removed from the graph.

* A new edge :math:`v \rightarrow z` is inserted represented with red color.

.. graphviz::

    digraph G {
        u, z [shape=circle;style=filled;color=deepskyblue];
        v [style=filled; color=green];
        "G" [shape=tripleoctagon; style=filled;color=deepskyblue;label = "Rest of the Graph"];

        rankdir=LR;
        G -> {u, z} [dir=none, weight=1, penwidth=3];
        u -> v;
        v -> z [label="{w}";color=red]
    }

Contracting ``v``:

* The vertex ``v`` is removed from the graph

  * The edges :math:`u \rightarrow v` and  :math:`v \rightarrow z` are removed from the graph.

* A new edge :math:`u \rightarrow z` is inserted represented with red color.


.. graphviz::

    digraph G {
        u, z [shape=circle;style=filled;color=deepskyblue];
        "G" [shape=tripleoctagon; style=filled;color=deepskyblue;label = "Rest of the Graph"];

        rankdir=LR;
        G -> {u, z} [dir=none, weight=1, penwidth=3];
        u -> z [label="{v, w}";color=red]
    }

Edge :math:`u \rightarrow z` has the information of nodes that were contracted.


The cycle
-------------------------------------------------------------------------------

Contracting a graph, can be done with more than one operation.
The order of the operations affect the resulting contracted graph, after
applying one operation, the set of vertices that can be contracted
by another operation changes.

This implementation, cycles ``max_cycles`` times through ``operations_order`` .

.. code-block:: none

    <input>
    do max_cycles times {
        for (operation in operations_order)
         { do operation }
    }
    <output>


Contracting Sample Data
-------------------------------------------------------------------------------

In this section, building and using a contracted graph will be shown by example.

- The :doc:`sampledata` for an undirected graph is used
- a dead end operation first followed by a linear operation.




Construction of the graph in the database
...............................................................................

.. rubric:: Original Data

The following query shows the original data involved in the contraction operation.

.. literalinclude:: doc-contraction-family.queries
   :start-after: -- q00
   :end-before: -- q01

The original graph:

.. image:: images/undirected_sampledata_a.png
   :scale: 50%

Contraction Results
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

The results do not represent the contracted graph.
They represent the changes done to the graph after applying the contraction algorithm.

Observe that vertices, for example, :math:`6` do not appear in the results because
it was not affected by the contraction algorithm.

.. literalinclude:: doc-contraction-family.queries
   :start-after: -- q2
   :end-before: -- q3

After doing the dead end contraction operation:

.. image:: images/undirected_sampledata_b.png
   :scale: 50%

After doing the linear contraction operation to the graph above:

.. image:: images/undirected_sampledata_c.png
   :scale: 50%

The process to create the contraction graph on the database:

* `Add additional columns`_
* `Store contraction information`_
* `Update the vertices and edge tables`_


Add additional columns
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Adding extra columns to the ``edge_table`` and ``edge_table_vertices_pgr`` tables, where:

=======================  ==================================================
Column                    Description
=======================  ==================================================
**contracted_vertices**  The vertices set belonging to the vertex/edge
**is_contracted**        On the `vertex` table

                         * when ``true`` the vertex is contracted, its not part of the contracted graph.
                         * when ``false`` the vertex is not contracted, its part of the contracted graph.

**is_new**               On the `edge` table:

                         * when ``true`` the edge was generated by the contraction algorithm. its part of the contracted graph.
                         * when ``false`` the edge is an original edgei, might be or not part of the contracted graph.

=======================  ==================================================

.. literalinclude:: doc-contraction-family.queries
   :start-after: -- q1
   :end-before: -- q2

Store contraction information
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Store the `contraction results`_ in a table

.. literalinclude:: doc-contraction-family.queries
   :start-after: -- q3
   :end-before: -- q4

Update the vertices and edge tables
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. rubric:: Update the `vertex` table using the contraction information

Use ``edge_table_vertices_pgr.is_contracted`` to indicate the vertices that are contracted.

.. literalinclude:: doc-contraction-family.queries
   :start-after: -- q4
   :end-before: -- q5

Add to ``edge_table_vertices_pgr.contracted_vertices`` the contracted vertices belonging to the vertices.

.. literalinclude:: doc-contraction-family.queries
   :start-after: -- q6
   :end-before: -- q7

The modified ``edge_table_vertices_pgr``.

.. literalinclude:: doc-contraction-family.queries
   :start-after: -- q7
   :end-before: -- q8

.. rubric:: Update the `edge` table using the contraction information

Insert the new edges generated by pgr_contraction.

.. literalinclude:: doc-contraction-family.queries
   :start-after: -- q8
   :end-before: -- q9

The modified ``edge_table``.

.. literalinclude:: doc-contraction-family.queries
   :start-after: -- q9
   :end-before: -- q10


The contracted graph
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Vertices that belong to the contracted graph.

.. literalinclude:: doc-contraction-family.queries
   :start-after: -- q10
   :end-before: -- q11

Edges that belong to the contracted graph.

.. literalinclude:: doc-contraction-family.queries
   :start-after: -- q11
   :end-before: -- case1

.. image:: images/undirected_sampledata_c.png
   :scale: 50%

Using the contracted graph
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Using the contracted graph with ``pgr_dijkstra``

There are five cases when calculating the shortest path between a given source and target.

- **Case 1**: Both source and target belong to the contracted graph.
- **Case 2**: Source belongs to a contracted graph, while target belongs to a edge subgraph.
- **Case 3**: Source belongs to a vertex subgraph, while target belongs to an edge subgraph.
- **Case 4**: Source belongs to a contracted graph, while target belongs to an vertex subgraph.
- **Case 5**: The path contains a new edge added by the contraction algorithm.

.. rubric:: case 1: Both source and target belong to the contracted graph.

Inspecting the contracted graph above, vertex 3 and vertex 11 are part of the contracted graph. In the following query:

 - vertices_in_graph hold the vertices that belong to the contracted graph.
 - when selecting the edges, only edges that have the source and the target in that set are the edges belonging to the contracted graph, that is done in the WHERE clause.

Visually, looking at the original graph, going from 3 to 11: 3 -> 6 -> 11, and in the contracted graph, it is also 3 -> 6 -> 11.
The results, on the contracted graph match the results as if it was done on the original graph.

.. literalinclude:: doc-contraction-family.queries
   :start-after: -- case1
   :end-before: -- case2

.. rubric:: case 2: Source belongs to the contracted graph, while target belongs to a edge subgraph.

Inspecting the contracted graph above, vertex 3 is part of the contracted graph and vertex 1 belongs to the contracted subgraph of edge 19. In the following query:
  - expand1 holds the contracted vertices of the edge where vertex 1 belongs. (belongs to edge 19).
  - vertices_in_graph hold the vertices that belong to the contracted graph and also the contracted vertices of edge 19.
  - when selecting the edges, only edges that have the source and the target in that set are the edges belonging to the contracted graph, that is done in the WHERE clause.

Visually, looking at the original graph, going from 3 to 1: 3 -> 2 -> 1, and in the contracted graph, it is also 3 -> 2 -> 1.
The results, on the contracted graph match the results as if it was done on the original graph.

.. literalinclude:: doc-contraction-family.queries
   :start-after: -- case2
   :end-before: -- case3


.. rubric:: case 3: Source belongs to a vertex subgraph, while target belongs to an edge subgraph.

Inspecting the contracted graph above, vertex 7 belongs to the contracted subgraph of vertex 5 and vertex 13 belongs to the contracted subgraph of edge 21. In the following query:

 - expand7 holds the contracted vertices of vertex where vertex 7 belongs. (belongs to vertex 5)
 - expand13 holds the contracted vertices of edge where vertex 13 belongs. (belongs to edge 21)
 - vertices_in_graph hold the vertices that belong to the contracted graph, contracted vertices of vertex 5 and contracted vertices of edge 21.
 - when selecting the edges, only edges that have the source and the target in that set are the edges belonging to the contracted graph, that is done in the WHERE clause.

Visually, looking at the original graph, going from 7 to 13: 7 -> 8 -> 5 -> 10 -> 13, and in the contracted graph, it is also 7 -> 8 -> 5 -> 10 -> 13.
The results, on the contracted graph match the results as if it was done on the original graph.

.. literalinclude:: doc-contraction-family.queries
   :start-after: -- case3
   :end-before: -- case4


.. rubric:: case 4: Source belongs to the contracted graph, while target belongs to an vertex subgraph.

Inspecting the contracted graph above, vertex 3 is part of the contracted graph and vertex 7 belongs to the contracted subgraph of vertex 5. In the following query:

 - expand7 holds the contracted vertices of vertex where vertex 7 belongs. (belongs to vertex 5)
 - vertices_in_graph hold the vertices that belong to the contracted graph and the contracted vertices of vertex 5.
 - when selecting the edges, only edges that have the source and the target in that set are the edges belonging to the contracted graph, that is done in the WHERE clause.

Visually, looking at the original graph, going from 3 to 7: 3 -> 2 -> 5 -> 8 -> 7, but in the contracted graph, it is 3 -> 5 -> 8 -> 7.
The results, on the contracted graph do not match the results as if it was done on the original graph. This is because the path contains edge 19 which is added by the contraction algorithm.

.. literalinclude:: doc-contraction-family.queries
   :start-after: -- case4
   :end-before: -- case5q1

.. rubric:: case 5: The path contains an edge added by the contraction algorithm.

In the previous example we can see that the path from vertex 3 to vertex 7 contains an edge which is added by the contraction algorithm.

.. literalinclude:: doc-contraction-family.queries
   :start-after: -- case5q1
   :end-before: -- case5q2

Inspecting the contracted graph above, edge 19 should be expanded. In the following query:

 - first_dijkstra holds the results of the dijkstra query.
 - edges_to_expand holds the edges added by the contraction algorithm and included in the path.
 - vertices_in_graph hold the vertices that belong to the contracted graph, vertices of the contracted solution and the contracted vertices of the edges added by the contraction algorithm and included in the contracted solution.
 - when selecting the edges, only edges that have the source and the target in that set are the edges belonging to the contracted graph, that is done in the WHERE clause.

Visually, looking at the original graph, going from 3 to 7: 3 -> 2 -> 5 -> 8 -> 7, and in the contracted graph, it is also 3 -> 2 -> 5 -> 8 -> 7.
The results, on the contracted graph match the results as if it was done on the original graph.

.. literalinclude:: doc-contraction-family.queries
   :start-after: -- case5q2
   :end-before: -- end

See Also
-------------

* http://www.cs.cmu.edu/afs/cs/academic/class/15210-f12/www/lectures/lecture16.pdf
* http://algo2.iti.kit.edu/documents/routeplanning/geisberger_dipl.pdf
* The queries use :doc:`pgr_contraction` function and the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

