..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

Contraction - Family of functions
===============================================================================

.. index from here

* :doc:`pgr_contraction`

.. index to here

.. toctree::
    :hidden:

    pgr_contraction


.. rubric:: Previous versions of this page

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/contraction-family.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/contraction-family.html>`__
  `2.6 <https://docs.pgrouting.org/2.6/en/contraction-family.html>`__

* **Unsupported versions:**
  `2.5 <https://docs.pgrouting.org/2.5/en/contraction-family.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/contraction-family.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/contraction/doc/contraction.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/contraction/doc/contraction.html>`__


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
  * `There are no incoming edges and has at least one outgoing edge.`_


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

There are no incoming edges and has at least one outgoing edge.
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

  * `Linearity is symmetrical`_

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

Linearity is symmetrical
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

========================  ==================================================
Column                    Description
========================  ==================================================
**contracted_vertices**   The vertices set belonging to the vertex/edge
**is_contracted**         On the `vertex` table

                          * when ``true`` the vertex is contracted, its not part of the contracted graph.
                          * when ``false`` the vertex is not contracted, its part of the contracted graph.

**is_new**                On the `edge` table:

                          * when ``true`` the edge was generated by the contraction algorithm. its part of the contracted graph.
                          * when ``false`` the edge is an original edge, might be or not part of the contracted graph.

========================  ==================================================



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
...............................................................................

Vertices that belong to the contracted graph.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: doc-contraction-family.queries
   :start-after: -- q10
   :end-before: -- q11

Edges that belong to the contracted graph.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: doc-contraction-family.queries
   :start-after: -- q11
   :end-before: -- case1

.. image:: images/undirected_sampledata_c.png
   :scale: 50%

Using the contracted graph
...............................................................................

Using the contracted graph with ``pgr_dijkstra``

There are three cases when calculating the shortest path between a given source and target
in a contracted graph:

- Case 1: Both source and target belong to the contracted graph.
- Case 2: Source and/or target belong to an edge subgraph.
- Case 3: Source and/or target belong to a vertex.

Case 1: Both source and target belong to the contracted graph.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Using the `Edges that belong to the contracted graph.`_ on lines 10 to 19.

.. literalinclude:: doc-contraction-family.queries
   :emphasize-lines: 10-19
   :start-after: -- case1
   :end-before: -- use1
   :linenos:

.. rubric:: Case 1

When both source and target belong to the contracted graph, a path is found.

.. literalinclude:: doc-contraction-family.queries
   :start-after: -- use1
   :end-before: -- use1-1

.. rubric:: Case 2

When source and/or target belong to an edge subgraph then a path is not found.

In this case, the contracted graph do not have an edge connecting with
node :math:`4`.

.. literalinclude:: doc-contraction-family.queries
   :start-after: -- use1-1
   :end-before: -- use1-2

.. rubric:: Case 3

When source and/or target belong to a vertex then a path is not found.

In this case, the contracted graph do not have an edge connecting with
node :math:`7` and of node :math:`4` of the second case.

.. literalinclude:: doc-contraction-family.queries
   :start-after: -- use1-2
   :end-before: -- case2

Case 2: Source and/or target belong to an edge subgraph.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Refining the above function to include nodes that belong to an edge.

- The vertices that need to be expanded are calculated on lines 10 to 16.
- Adding to the contracted graph that additional section on lines 25 to 27.

.. literalinclude:: doc-contraction-family.queries
   :emphasize-lines: 10-16, 25-27
   :start-after: -- case2
   :end-before: -- use2
   :linenos:

.. rubric:: Case 1

When both source and target belong to the contracted graph, a path is found.

.. literalinclude:: doc-contraction-family.queries
   :start-after: -- use2
   :end-before: -- use2-1

.. rubric:: Case 2

When source and/or target belong to an edge subgraph, now, a path is found.

The routing graph now has an edge connecting with node :math:`4`.

.. literalinclude:: doc-contraction-family.queries
   :start-after: -- use2-1
   :end-before: -- use2-2

.. rubric:: Case 3

When source and/or target belong to a vertex then a path is not found.

In this case, the contracted graph do not have an edge connecting with
node :math:`7`.

.. literalinclude:: doc-contraction-family.queries
   :start-after: -- use2-2
   :end-before: -- case3

Case 3: Source and/or target belong to a vertex.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Refining the above function to include nodes that belong to an edge.

- The vertices that need to be expanded are calculated on lines 18 to 23.
- Adding to the contracted graph that additional section on lines 38 to 40.

.. literalinclude:: doc-contraction-family.queries
   :emphasize-lines: 18-23, 38-40
   :start-after: -- case3
   :end-before: -- use3
   :linenos:

.. rubric:: Case 1

When both source and target belong to the contracted graph, a path is found.

.. literalinclude:: doc-contraction-family.queries
   :start-after: -- use3
   :end-before: -- use3-1

.. rubric:: Case 2

The code change do not affect this case so when source and/or target belong
to an edge subgraph, a path is still found.

.. literalinclude:: doc-contraction-family.queries
   :start-after: -- use3-1
   :end-before: -- use3-2

.. rubric:: Case 3

When source and/or target belong to a vertex, now, a path is found.

Now, the routing graph has an edge connecting with node :math:`7`.

.. literalinclude:: doc-contraction-family.queries
   :start-after: -- use3-2
   :end-before: -- end

See Also
-------------

* https://www.cs.cmu.edu/afs/cs/academic/class/15210-f12/www/lectures/lecture16.pdf
* https://algo2.iti.kit.edu/documents/routeplanning/geisberger_dipl.pdf
* The queries use :doc:`pgr_contraction` function and the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

