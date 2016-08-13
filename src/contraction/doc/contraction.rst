.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************


.. _contraction:

Contraction
===============================================================================


.. include:: ../../proposed.rst
   :start-after: begin-warning
   :end-before: end-warning

:ref:`pgr_contractGraph`

Graph Contraction, when talking about big graphs, like the road graphs, or electric networks, can be used to speed up, some graph algorithms.

The contraction level and contraction operations can become very complex, as the complexity
of the graphs grows.

For this proposal, we are making our contraction algorithm simple as possible so that
more contraction operations can be added in the future.

We are not aiming with this work to implement all the possible contraction operations
but to give a framework such that adding a contraction operation can be easily achieved.

For this contraction proposal I am only making 2 operations:

 1. dead end contraction: vertices have one incoming edge
 2. linear contraction: vertices have one incoming and one outgoing edge
   

And with the additional characteristics:

  - The user can forbid to contract a particular set of nodes or edges.
  - The user can decide how many times the cycle can be done.
  - If possible, the user can decide the order of the operations on a cycle.


The contraction skeleton
-------------------------------------------------------------------------------

In general we have an initial set up that may involve analizing the graph given as input and setting the
non contractable nodes or edges. We have a cycle that will go and perform a contraction operation
until while possible, and then move to the next contraction operation.
Adding a new operation then becomes an "easy" task; more things might be involved, because the
charachteristics of the graph change each time its contracted, so some interaction between contractions
has to be implemented also.

Currently, there are two implemented operation for contracting a graph

- Dead End contraction
- Linear contraction


Dead end contraction
-------------------------------------------------------------------------------


Dead end nodes
......................

.. note:: TODO: based on the code explain precisely what is considered a dead end node

.. rubric:: Examples

- The green node ``B`` represents a dead end node
- The node ``A`` is the only node connecting to ``B``.
- Node ``A`` is part of the rest of the graph and has an unlimited number of incoming and outgoing edges.
- Directed graph

.. graphviz::

    digraph G {
        A [style=filled;color=deepskyblue];
        B [style=filled; color=green];
        "G" [shape=tripleoctagon;
        style=filled;color=deepskyblue;
        label = "Rest of the Graph"];

        rankdir=LR;
        G -> A [dir=none, weight=1, penwidth=3];
        A -> B;
    }


Operation: Dead End Contraction
.....................................

The dead end contraction will stop until there are no more dead end nodes.
For example from the following graph:

- Node ``A`` is connected to the rest of the graph by an unlimited number of edges.
- Node ``B`` is connected to the rest of the graph with one incoming edge.
- The green node ``C`` represents a `Dead End` node

.. graphviz::

    digraph G {
        A [style=filled;color=deepskyblue];
        B [style=filled; color=deepskyblue];
        C [style=filled; color=green];
        "G" [shape=tripleoctagon;
        style=filled;color=deepskyblue;
        label = "Rest of the Graph"];

        rankdir=LR;
        G -> A [dir=none, weight=1, penwidth=3];
        A -> B;
        B -> C;
    }


After contracting ``B``, node ``C`` is now a `Dead End` and is contracted:

.. graphviz::

    digraph G {
        A [style=filled;color=deepskyblue];
        B [style=filled; color=green;label="B {C}";];
        "G" [shape=tripleoctagon;
        style=filled;color=deepskyblue;
        label = "Rest of the Graph"];

        rankdir=LR;
        G -> A [dir=none, weight=1, penwidth=3];
        A -> B;
    }

Node ``B`` gets contracted

.. graphviz::

    digraph G {
        A [style=filled;color=deepskyblue;label="A {B, C}";];
        "G" [shape=tripleoctagon;
        style=filled;color=deepskyblue;
        label = "Rest of the Graph"];

        rankdir=LR;
        G -> A [dir=none, weight=1, penwidth=3];
    }

Nodes ``B`` and ``C`` belong to node ``A``.



Not Dead End nodes
......................

In the next graph ``B`` is not a `dead end` node.

.. graphviz::

    digraph G {
        A [style=filled;color=deepskyblue];
        B [style=filled; color=red];
        "G" [shape=tripleoctagon;
        style=filled;color=deepskyblue;
        label = "Rest of the Graph"];

        G -> A [dir=none, weight=1, penwidth=3];
        B -> A;
    }





Linear contraction
-------------------------------------------------------------------------------

Linear nodes
......................

.. note:: TODO: based on the code explain precisely what is considered a linear node

.. rubric:: Examples

- The green node ``B`` represents a linear node
- The nodes ``A`` and ``C`` are the only nodes connecting to ``B``.
- Node ``A`` is part of the rest of the graph and has an unlimited number of incoming and outgoing edges.
- Node ``C`` is part of the rest of the graph and has an unlimited number of incoming and outgoing edges.
- Directed graph

.. graphviz::

    digraph G {
        A [style=filled;color=deepskyblue];
        B [style=filled; color=green];
        C [style=filled;color=deepskyblue];
        "G" [shape=tripleoctagon;
        style=filled;color=deepskyblue;
        label = "Rest of the Graph"];

        rankdir=LR;
        G -> A [dir=none, weight=1, penwidth=3];
        G -> C [dir=none, weight=1, penwidth=3];
        A -> B;
        B -> C;
    }

Operation: Linear Contraction
.....................................

The linear contraction will stop until there are no more linear nodes.
For example from the following graph:

- Node ``A`` is connected to the rest of the graph by an unlimited number of edges.
- Node ``B`` is connected to the rest of the graph with one incoming edge and one outgoing edge.
- Node ``C`` is connected to the rest of the graph with one incoming edge and one outgoing edge.
- Node ``D`` is connected to the rest of the graph by an unlimited number of edges.
- The green nodes ``B`` and ``C`` represents `Linear` nodes.

.. graphviz::

    digraph G {
        A [style=filled;color=deepskyblue];
        B [style=filled; color=green];
        C [style=filled; color=green];
        D [style=filled; color=deepskyblue];
        "G" [shape=tripleoctagon;
        style=filled;color=deepskyblue;
        label = "Rest of the Graph"];

        rankdir=LR;
        G -> A [dir=none, weight=1, penwidth=3];
        G -> D [dir=none, weight=1, penwidth=3];
        A -> B;
        B -> C;
        C -> D;

    }

After contracting ``B``, a new edge gets inserted between ``A`` and ``C`` which is represented by red color.

.. graphviz::

    digraph G {
        A [style=filled;color=deepskyblue];
        C [style=filled; color=green];
        D [style=filled; color=deepskyblue];
        "G" [shape=tripleoctagon;
        style=filled;color=deepskyblue;
        label = "Rest of the Graph"];

        rankdir=LR;
        G -> A [dir=none, weight=1, penwidth=3];
        G -> D [dir=none, weight=1, penwidth=3];
        A -> C [label="{B}";color=red]
        C -> D;

    }

Node ``C`` is `linear node` and gets contracted.

.. graphviz::

    digraph G {
        A [style=filled;color=deepskyblue];
        D [style=filled; color=deepskyblue];
        "G" [shape=tripleoctagon;
        style=filled;color=deepskyblue;
        label = "Rest of the Graph"];

        rankdir=LR;
        G -> A [dir=none, weight=1, penwidth=3];
        G -> D [dir=none, weight=1, penwidth=3];
        A -> D [label="{B, C}";color=red];

    }

Nodes ``B`` and ``C`` belong to edge connecting ``A`` and ``D`` which is represented by red color.


Not Linear nodes
......................

In the next graph ``B`` is not a `linear` node.

.. graphviz::

    digraph G {
        A [style=filled;color=deepskyblue];
        B [style=filled; color=red];
        C [style=filled;color=deepskyblue];
        "G" [shape=tripleoctagon;
        style=filled;color=deepskyblue;
        label = "Rest of the Graph"];

        rankdir=LR;
        G -> A [dir=none, weight=1, penwidth=3];
        G -> C [dir=none, weight=1, penwidth=3];
        A -> B;
        C -> B;
    }


The cyle
---------

Contracting a graph, can be done with more than one operation.
The order of the operations affect the resulting contracted graph, and after applying one operation, new vertices can be contracted in another operation.

This implementation, cycles ``max_cycles`` times  through ``operations_order`` .

.. code-block:: none

    <input>
    do max_cycles times {
        for (operation in operations_order) 
         { do operation }
    }
    <output>


Contracting Sample Data
-------------------------------------------------------------

In this section, building an using a contracted graph will be shown by example.

- The :ref:`sampledata` for an undirected graph is used
- a dead end operation first followed by a linear linear operation.

The original graph:

.. image:: images/undirected_sampledata_a.png

After doing a dead end contraction operation:
    
.. image:: images/undirected_sampledata_b.png

Doing a linear contraction operation to the graph above
    
.. image:: images/undirected_sampledata_c.png


There are five cases, in these documentation, which arise when calculating the shortest path between a given source and target.
In this examples, pgr_dijkstra is used.

- **Case 1**: Both source and target belong to the contracted graph.
- **Case 2**: Source belongs to a contracted graph, while target belongs to a vertex subgraph.
- **Case 3**: Source belongs to a contracted graph, while target belongs to an edge subgraph.
- **Case 4**: Source belongs to a vertex subgraph, while target belongs to an edge subgraph.
- **Case 5**: The path contains a new edge added by the contraction algorithm.

Construction of the graph in the database
..........................................

.. rubric:: Original Data

The following query shows the original data involved in the contraction operation.

.. literalinclude:: doc-contraction.queries
   :start-after: -- q00
   :end-before: -- q01

.. rubric:: Contraction Results

.. literalinclude:: doc-contraction.queries
   :start-after: -- q2
   :end-before: -- q3

.. note:: TODO write a paragraph comparing the results with the last graph above.

.. rubric:: step 1

Adding extra columns to the ``edge_table`` and ``edge_table_vertices_pgr`` tables:

=======================  ==================================================
Column                    Description
=======================  ==================================================
**contracted_vertices**    The vertices set belonging to the vertex/edge
**is_contracted**          On a `vertex` table: when ``true`` the vertex is contracted, so is not part of the contracted graph.
**is_contracted**          On an `edge` table: when ``true`` the edge was generated by the contraction algorithm.
=======================  ==================================================

Using the following queries:

.. literalinclude:: doc-contraction.queries
   :start-after: -- q1
   :end-before: -- q2


.. rubric:: step 2

For simplicity, in this documentation, store the results of the call to pgr_contractGraph in a temporary table

.. literalinclude:: doc-contraction.queries
   :start-after: -- q3
   :end-before: -- q4


.. rubric:: step 3

Update the `vertex` and `edge` tables using the results of the call to pgr_contraction

- In `edge_table_vertices_pgr.is_contracted` indicate the vertices that are contracted.

.. literalinclude:: doc-contraction.queries
   :start-after: -- q4
   :end-before: -- q5

- Add to `edge_table_vertices_pgr.contracted_vertices`  the contracted vertices belonging to the vertices.

.. literalinclude:: doc-contraction.queries
   :start-after: -- q6
   :end-before: -- q7

- Insert the new edges generated by  pgr_contractGraph.

.. literalinclude:: doc-contraction.queries
   :start-after: -- q8
   :end-before: -- q9

.. rubric:: step 3.1

Verify visually the updates.

- On the `edge_table_vertices_pgr`

.. literalinclude:: doc-contraction.queries
   :start-after: -- q7
   :end-before: -- q8

- On the `edge_table`

.. literalinclude:: doc-contraction.queries
   :start-after: -- q9
   :end-before: -- q10

- vertices that belong to the contracted graph are the non contracted vertices 

.. literalinclude:: doc-contraction.queries
   :start-after: -- q10
   :end-before: -- case1

.. rubric:: case 1: Both source and target belong to the contracted graph. 

Routing from 3 to 11.

Since both 3 and 11 both are in the contracted graph it is not necessary expand the graph.

.. literalinclude:: doc-contraction.queries
   :start-after: -- case1
   :end-before: -- case2

.. rubric:: case 2: Source belongs to the contracted graph, while target belongs to a edge subgraph.

Routing from 3 to 1.

Since 1 is in the contracted subgraph of edge (3, 5), it is necessary to expand that edge by adding {1, 2} to the vertex set, so the vertex set becomes {3, 5, 6, 9, 11, 15, 17   , 1, 2}

.. literalinclude:: doc-contraction.queries
   :start-after: -- case2
   :end-before: -- case3

.. rubric:: case 3: Source belongs to a vertex subgraph, while target belongs to an edge subgraph.

Routing from 7 to 13.

Since 13 is in the contracted subgraph of edge (5, 11), it is necessary to expand that edge by adding {10, 13} to the vertex set, and since 7 is in the contracted subgraph of vertex 5, it is necessary to expand that vertex by adding {7, 8} vertex set, so the vertex set becomes {3, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17}

.. literalinclude:: doc-contraction.queries
   :start-after: -- case3
   :end-before: -- case4

.. rubric:: case 4: Source belongs to the contracted graph, while target belongs to an vertex subgraph.

Routing from 3 to 7.

Since 7 is in the contracted subgraph of vertex 5, it is necessary to expand that vertex by adding {7, 8} to the vertex set, so the vertex set becomes {3, 5, 6, 9, 11, 15, 17   , 7, 8}.

.. literalinclude:: doc-contraction.queries
   :start-after: -- case4
   :end-before: -- case5q1

.. rubric:: case 5: The path contains an edge added by the contraction algorithm.

In the previous example we can see that the path from vertex 3 to vertex 7 contains an edge which is added by the contraction algorithm.


.. literalinclude:: doc-contraction.queries
   :start-after: -- case5q1
   :end-before: -- case5q2

This implies that it is a shortcut and should be expanded. We can see that the contracted subgraph of the added edge is {1, 2}. It is necessary to expand the edge by adding {1, 2} to the vertex set, so the vertex set becomes {3, 4, 5, 6, 9, 11, 15, 17   , 1, 2}.

.. literalinclude:: doc-contraction.queries
   :start-after: -- case5q2
   :end-before: -- end



References
-------------

* http://www.cs.cmu.edu/afs/cs/academic/class/15210-f12/www/lectures/lecture16.pdf
* http://algo2.iti.kit.edu/documents/routeplanning/geisberger_dipl.pdf

