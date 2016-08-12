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


Procedure
---------

* For contracting, we are going to cycle as follows

.. code-block:: none

    input: G(V,E);
    removed_vertices = {};


    <initial set up>
    do N times {

        while ( <conditions for 1> ) {
            < contraction operation 1 >
        }
       
        while ( <conditions for 2> ) {
            < contraction operation 2>
        }
        .....
    }

    output: G'(V',E'), removed_vertices 

Sample Data
-------------------------------------------------------------

.. image:: images/undirected_sampledata_a.png

:procedure:

    
    
.. image:: images/undirected_sampledata_b.png


    
.. image:: images/undirected_sampledata_c.png


Detailed Procedure
............................
:Original Data:

.. literalinclude:: doc-contraction.queries
   :start-after: -- q1
   :end-before: -- q2

:Addition of new columns:

.. code-block:: sql

        ALTER TABLE edge_table ADD is_contracted BOOLEAN DEFAULT false;
        ALTER TABLE edge_table ADD contracted_vertices BIGINT[];

:Data After Adding Columns:

.. literalinclude:: doc-contraction.queries
   :start-after: -- q4
   :end-before: -- q5


:Addition of new edges by the algorithm:

.. code-block:: sql

        INSERT INTO edge_table(id, source, target, cost, reverse_cost, is_contracted, contracted_vertices) VALUES (-1, 3, 5, 2, 2, true, ARRAY[1, 2]);
        INSERT INTO edge_table(id, source, target, cost, reverse_cost, is_contracted, contracted_vertices) VALUES (-2, 3, 9, 2, 2, true, ARRAY[4]);
        INSERT INTO edge_table(id, source, target, cost, reverse_cost, is_contracted, contracted_vertices) VALUES (-3, 5, 11, 2, 2, true, ARRAY[10, 13]);
        INSERT INTO edge_table(id, source, target, cost, reverse_cost, is_contracted, contracted_vertices) VALUES (-4, 9, 11, 2, 2, true, ARRAY[12]);

:Data after addition of new edges:

.. literalinclude:: doc-contraction.queries
   :start-after: -- q9
   :end-before: -- q10

:Contracted Graph Data:



.. literalinclude:: doc-contraction.queries
   :start-after: -- q10
   :end-before: -- q11


Dijkstra on contracted graph
----------------------------------

There are five cases which arise when calculating the shortest path between a given source and target, 

**Case 1**: Both source and target belong to the contracted graph.

**Case 2**: Source belongs to a contracted graph, while target belongs to a vertex subgraph.

**Case 3**: Source belongs to a contracted graph, while target belongs to an edge subgraph.

**Case 4**: Source belongs to a vertex subgraph, while target belongs to an edge subgraph.

**Case 5**: The path contains a new edge added by the contraction algorithm.

:Examples:

**Case 1**: Routing from 3 to 11. Since 3 and 11 both are in the contracted graph it is not necessary expand the graph.


.. literalinclude:: doc-contraction.queries
   :start-after: -- q11
   :end-before: -- q12

**Case 2**: Routing from 3 to 7. Since 7 is in the contracted subgraph of vertex 5, it is necessary to expand that vertex by adding {7, 8} to the vertex set, so the vertex set becomes {3, 5, 6, 9, 11, 15, 17 , 7, 8}



.. literalinclude:: doc-contraction.queries
   :start-after: -- q12
   :end-before: -- q13  

**Case 3**: Routing from 3 to 13. Since 13 is in the contracted subgraph of edge (5, 11),  it is necessary to expand that edge by adding {10, 13} to the vertex set, so the vertex set becomes {3, 5, 6, 9, 10, 11, 13, 15, 17}.



.. literalinclude:: doc-contraction.queries
   :start-after: -- q13
   :end-before: -- q14


**Case 4**: Routing from 7 to 13. Since 13 is in the contracted subgraph of edge (5, 11), it is necessary to expand that edge by adding {10, 13} to the vertex set, and since 7 is in the contracted subgraph of vertex 5, it is necessary to expand that vertex by adding {7, 8} vertex set, so the vertex set becomes {3, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17}



.. literalinclude:: doc-contraction.queries
   :start-after: -- q14
   :end-before: -- q15


**Case 5**: Routing from 3 to 9. Since 3 and 9 both are in the contracted graph it is not necessary expand the graph.



.. literalinclude:: doc-contraction.queries
   :start-after: -- q15
   :end-before: -- q16


.. literalinclude:: doc-contraction.queries
   :start-after: -- q16
   :end-before: -- q17

This implies that it is a shortcut and should be expanded. The contracted subgraph of edge 20(3, 9, c=2) is {4}. It is necessary to expand the edge by adding {4} to the vertex set, so the vertex set becomes {3, 4, 5, 6, 9, 11, 15, 17}.


.. literalinclude:: doc-contraction.queries
   :start-after: -- q17
   :end-before: -- q18


References
-------------

* http://www.cs.cmu.edu/afs/cs/academic/class/15210-f12/www/lectures/lecture16.pdf
* http://algo2.iti.kit.edu/documents/routeplanning/geisberger_dipl.pdf

