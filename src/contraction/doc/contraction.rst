.. _contraction:

Contraction
===============================================================================

The documentation provides explanation on graph contraction.

*level 1* vertices have only one incoming edge.
*level 2* vertices have one incoming edge and one outgoing edge.

Notations
---------

* V: is the set of vertices
* E: is the set of edges
* G: is the graph
* :math:`L1_v`: is the set of *level 1* vertices 
* :math:`L2_v`: is the set of *level 2* vertices
* removed_vertices: is the set of removed vertices

The contracted graph will be represented with two parameters, the modified Graph, and the removed_vertices set.


removed_vertices = {(v,1):{2}, (e,-1):{3}}.


The above notation indicates:
vertex 2 is removed, and belongs to vertex 1 subgraph
vertex 3 is removed, and belongs to edge -1 subgraph



Procedure
---------

* For contracting, we are going to cycle as follows

.. code-block:: none


    do N times {

    repeat {

    < work with level 1 vertices >

    until L1_v is empty

    }

    repeat {

    < work with level 2 vertices >

    until L2_v is empty

    }

    }

* We work in the order of id's

* For simplicity all the edges in the examples have unit weight.


Examples
--------

Example 1: Two nodes,One edge
++++++++++++++++++++++

.. image:: images/twoNodesOneEdge.png

*Graph 1: Directed, with two nodes and one edge*

G = {V:{1, 2}, E:{(1, 2)}}

Before we start we havent't removed any vertices so, removed_vertices={}.

A *level 1* vertex has one and only one incoming edge, for this example vertex 2 is a *level 1* vertex, so :math:`L1_v = \{2\}`

A *level 2* vertex is one that has one incoming edge and one outgoing edge, for this example we don't have *level 2* vertices, so :math:`L2_v = \{\}`

For this example we will cycle only once.

    Cycle 1:

        Level 1:

            :math:`L1_v` is not empty, therefore on *level 1* contraction, vertex 2 gets deleted.

            After removal of vertex 2, vertex 1 loses an outgoing edge and the sets change as follows,
            :math:`L1_v = \{\}`
            :math:`L2_v = \{\}`
            G = {V:{1}, E:{}}
            removed_vertices = {(v, 1):{2}}.

            Since :math:`L1_v` is empty we stop *level 1* contraction in Cycle 1.

        Level 2:

            :math:`L2_v` is empty, so we do not perform any *level 2* contraction in Cycle 1.

        After Cycle 1, the sets change as follows,

        :math:`L1_v = \{\}`
        :math:`L2_v = \{\}`
        G = {V:{1}, E:{}}
        removed_vertices = {(v, 1):{2}}

        Since both :math:`L1_v` and :math:`L2_v` are empty, we cannot contract further and the contraction ends after Cycle 1.

So finally,the contracted graph is represented as 
    removed_vertices = {(v, 1):{2}}
    G = {V:{1}, E:{}}        

.. image:: images/resultingGraph1.png


Example 2: Three nodes,Two edges
+++++++++++++++++++++++++++++++++

.. image:: images/threeNodestwoEdges.png

*Graph 2: Directed, with three nodes and two edges*

G = {V:{1, 2, 3}, E:{(1, 2), (2, 3)}}

Before we start we havent't removed any vertices so,
removed_vertices={}


A *level 1* vertex has one and only one incoming edge, for this example vertex 3 is a *level 1* vertex, so 
:math:`L1_v = \{3\}`

A *level 2* vertex is one that has one incoming edge and one outgoing edge, for this example vertex 2 is a *level 2* vertex, so :math:`L2_v = \{2\}`

For this example we will cycle only once
    
    Cycle 1:

        Level 1:

            :math:`L1_v` is not empty,therefore on *level 1* contraction, vertex 3 gets deleted.

            After removal of vertex 3, vertex 2 loses an outgoing edge and the sets change as follows,
            :math:`L1_v = \{2\}`
            :math:`L2_v = \{\}`
            G = {V:{1, 2}, E:{(1, 2)}}
            removed_vertices = {(v, 2):{3}}

            :math:`L1_v` is not empty, therefore on *level 1* contraction, vertex 2 gets deleted.

            After removal of vertex 2, vertex 1 loses an outgoing edge and the sets change as follows,
            :math:`L1_v = \{\}`
            :math:`L2_v = \{\}`
            G = {V:{1}, E:{}}
            removed_vertices = {(v, 1):{2, 3}}

            Since :math:`L1_v` is empty we stop *level 1* contraction in Cycle 1.

        Level 2:

            :math:`L2_v` is empty, so we do not perform any *level 2* contraction in Cycle 1.

        After Cycle 1, the sets change as follows,

        :math:`L1_v = \{\}`
        :math:`L2_v = \{\}`
        G = {V:{1}, E:{}}
        removed_vertices = {(v, 1):{2, 3}}

        Since both of the above sets are empty, we cannot contract further and the contraction ends after Cycle 1.

So finally,the contracted graph is represented as 
    removed_vertices = {(v, 1):{2, 3}}
    G = {V:{1}, E:{}}

.. image:: images/resultingGraph2.png


Example 3: Three nodes,Three edges
+++++++++++++++++++++++++++++++++++

.. image:: images/threeNodesthreeEdges.png

*Graph 3: Directed, with three nodes and three edges*

G = {V:{1, 2, 3},E:{(1, 2), (2, 3), (1, 3)}}

Before we start we havent't removed any vertices so,
removed_vertices = {}

A *level 1* vertex has one and only one incoming edge, for this example we don't have any *level 1* vertex, so 
:math:`L1_v = \{\}`

A *level 2* vertex is one that has one incoming edge and one outgoing edges, for this example vertex 2 is a *level 2* vertex, so :math:`L2_v = {2}`

For this example we will cycle only once.
    
    Cycle 1:

        Level 1:

            :math:`L1_v` is empty, so we do not perform any *level 1* contraction in Cycle 1.

        Level 2:

            :math:`L2_v` is not empty, therefore on *level 2* contraction, vertex 2 gets deleted, and will add a new edge(shortcut) with an id = -1 and cost = 2, between vertex 1 and vertex 3.

            After removal of vertex 2 the sets change as follows,
            :math:`L1_v = \{\}`
            :math:`L2_v = \{\}`
            G = {V:{1, 3}, E:{(1, 3), -1}}
            removed_vertices = {(e, -1):{2}}

            Since :math:`L2_v` is empty we stop *level 2* contraction in Cycle 1.

        After Cycle 1,the sets change as follows,

        :math:`L1_v = \{\}`
        :math:`L2_v = \{\}`
        G = {V:{1, 3}, E:{(1, 3), -1}}
        removed_vertices = {(e, -1):{2}}

        Since both of the above sets are empty, we cannot contract further and the contraction ends after Cycle 1.

So finally,the contracted graph is represented as 
    removed_vertices = {(e, -1):{2}}
    G = {V:{1, 3}, E:{(1, 3), -1}

.. image:: images/resultingGraph3.png






Example 4: Sample Data
++++++++++++++++++++++++

.. image:: images/sampledataGraph2.png

*Graph 3* :Undirected graph with cost and reverse cost

Before we start we havent't removed any vertices so, removed_vertices = {}

:math:`V_1 = {1,7,13,14,15,16,17}`

:math:`V_2 = {4,8,12}`


For this example we will cycle only once
    
    Cycle 1:

        Level 1:

            :math:`V_1` is not empty,therefore on *dead end* contraction, vertices 1,7,8,13,14,16 gets deleted in the order.

            After the *dead end* contraction the sets change as follows
            :math:`V_1 = \{\}`
            :math:`V_2 = \{2,4,10,12\}`
            removed_vertices = {(v, 1):{2}, (v,5):{7,8}, (v,10):{13}, (v,15):{14}, (v,17):{16}}

            Since :math:`V_1` is empty we stop *dead end* contraction in Cycle 1.

        Level 2:

            :math:`V_2` is not empty, therefore on *linear* contraction, vertices 2,4,10,12 gets deleted in the order, and adds edges(shortcut) with ids -1,-2,-3,-4 respectively with each of the added edge having a cost = 2

            After the *linear* contraction the sets change as follows
            :math:`V_1 = \{\}`
            :math:`V_2 = \{\}`
            removed_vertices = {(e, -1):{1,2}, (e,-2):{4}, (e,-3):{10,13}, (e,-4):{12}, (v,5):{7,8}, (v,15):{14}, (v,17):{16}}

            Since :math:`V_2` is empty we stop *level 2* contraction in Cycle 1.

    Since both of the above sets are empty, we cannot contract further and the contraction ends after Cycle 1.