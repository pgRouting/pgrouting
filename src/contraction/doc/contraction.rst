
.. _contraction:

Contraction
===============================================================================

Contracting a graph becomes a crucial operation when taking about big graphs like
the graphs involved in routing in cities, countries, continents or the whole world.

The contraction level, and contraction operations can become very complex, as the complexity
of the graphs grows.

For this proposal, we are making our contraction algorithm simple as possible so that
more contraction operations can be added in the future.

We are not aiming with this work to implement all the possible contraction operations
but to give a framework such that adding a contraction operation can be easily achieved

For this contraaction proposal I am only making 2 operations:

 - 1) dead end contraction: vertices have one incoming edge
 - 2) linear contraction: vertices have one incomming and one outgoing edge
   

And with the additional characteristics:

  - the user can forbid to contract a particular set of nodes or edges.
  - the user can decide how many times the cycle can be done.
  - If possible, the user can decide the order of the operations on a cycle.

.. note:: Work on progress in contraction branch


Refrences
++++++++++++++++++++++++++

* http://www.cs.cmu.edu/afs/cs/academic/class/15210-f12/www/lectures/lecture16.pdf
* http://algo2.iti.kit.edu/documents/routeplanning/geisberger_dipl.pdf

The contraction skeleton
-------------------------------------------------------------------------------

In general we have an initial set up that may involve analizing the graph given as input and setting the
non contractionable nodes or edges. We have a cycle that will go and perform a contraction operation
until while possible, and then move to the next contraction operation.
Adding a new operation then becomes an "easy" task; more things might be involved, because the
charachteristics of the graph change each time its contracted, so some interaction between contractions
has to be implemented also.

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



Contraction operations for this implementation
-------------------------------------------------------------------------------

Dead end contraction
+++++++++++++++++++++

Characteristics:

  - :math:`V_1`: set of vertices with 1 incoming edge in increasing order of id:

    - Edges with the same identifier are considered the same edge
      and if it has the `reverse_cost` valid the outgoing edge is ignored

.. code-block:: none

    while ( V_1 is not empty ) {

        delete vertex of V_1
        the deleted vertex add it to removed_vertices
        vertex that leads to removed vertex, inherits the removed vertex

        <adjust any conditions that might affect other contraction operation>

    }



Linear contraction
+++++++++++++++++++++

Characteristics:

  - :math:`V_2`: vertex with 1 incoming edge and 1 outgoing edge:

    - The outgoing edge must have different identifier of the incomming edge

.. code-block:: none

    while ( V_2 is not empty ) {

        delete vertex of V_2
        create edge (shortcut)
        the deleted vertex add it to removed_vertices
        inewly created edge, inherits the removed vertex

        <adjust any conditions that might affect other contraction operations>
    }



Notations
++++++++++

* V: is the set of vertices
* E: is the set of edges
* G: is the graph
* :math:`V1`: is the set of *dead end* vertices 
* :math:`V2`: is the set of *linear* vertices
* removed_vertices: is the set of removed vertices

The contracted graph will be represented with two parameters, the modified Graph, and the removed_vertices set.

removed_vertices = {(v,1):{2}, (e,-1):{3}}.


The above notation indicates:
  - Vertex 2 is removed, and belongs to vertex 1 subgraph
  - Vertex 3 is removed, and belongs to edge -1 subgraph






Examples
-------------------------------------------------------------------------------

For simplicity all the edges in the examples have unit weight.

Dead End
+++++++++++++++++

Perform dead end contraction coperation first and then linear contraction
1 cycle of contraction.

.. image:: images/twoNodesOneEdge.png

:Input:  G = {V:{1, 2}, E:{(1, 2)}}

:initial set up:

.. code-block:: none

    removed_vertices={}
    L1 = {2}


:procedure:

.. code-block:: none

    L1 is not empty

        :math:`L1_v` = {}
        :math:`L2_v` = {}
        G = {V:{1}, E:{}}
        removed_vertices = {(v, 1):{2}}.

    L1 is empty
    
            Since :math:`L1_v` is empty we stop *level 1* contraction in Cycle 1.

        Level 2:

            :math:`L2_v` is empty, so we do not perform any *level 2* contraction in Cycle 1.

        After Cycle 1, the sets change as follows,

        :math:`L1_v` = {}
        :math:`L2_v` = {}
        G = {V:{1}, E:{}}
        removed_vertices = {(v, 1):{2}}

        Since both :math:`L1_v` and :math:`L2_v` are empty, we cannot contract further and the contraction ends after Cycle 1.

So finally,the contracted graph is represented as 
    removed_vertices = {(v, 1):{2}}
    G = {V:{1}, E:{}}        


.. _fig2:

.. image:: images/threeNodestwoEdges.png

*Graph 2: Directed, with three nodes and two edges*

G = {V:{1, 2, 3}, E:{(1, 2), (2, 3)}}

Before we start we havent't removed any vertices so,
removed_vertices={}


A *level 1* vertex has one and only one incoming edge, for this example vertex 3 is a *level 1* vertex, so 
:math:`L1_v` = {3}

A *level 2* vertex is one that has one incoming edge and one outgoing edge, for this example vertex 2 is a *level 2* vertex, so :math:`L2_v` = {2}

For this example we will cycle only once
    
    Cycle 1:

        Level 1:

            :math:`L1_v` is not empty,therefore on *level 1* contraction, vertex 3 gets deleted.

            After removal of vertex 3, vertex 2 loses an outgoing edge and the sets change as follows,
            :math:`L1_v` = {2}
            :math:`L2_v` = {}
            G = {V:{1, 2}, E:{(1, 2)}}
            removed_vertices = {(v, 2):{3}}

            :math:`L1_v` is not empty, therefore on *level 1* contraction, vertex 2 gets deleted.

            After removal of vertex 2, vertex 1 loses an outgoing edge and the sets change as follows,
            :math:`L1_v` = {}
            :math:`L2_v` = {}
            G = {V:{1}, E:{}}
            removed_vertices = {(v, 1):{2, 3}}

            Since :math:`L1_v` is empty we stop *level 1* contraction in Cycle 1.

        Level 2:

            :math:`L2_v` is empty, so we do not perform any *level 2* contraction in Cycle 1.

        After Cycle 1, the sets change as follows,

        :math:`L1_v` = {}
        :math:`L2_v` = {}
        G = {V:{1}, E:{}}
        removed_vertices = {(v, 1):{2, 3}}

        Since both of the above sets are empty, we cannot contract further and the contraction ends after Cycle 1.

So finally,the contracted graph is represented as 
    removed_vertices = {(v, 1):{2, 3}}
    G = {V:{1}, E:{}}


.. _fig3:

.. image:: images/threeNodesthreeEdges.png

*Graph 3: Directed, with three nodes and three edges*

G = {V:{1, 2, 3},E:{(1, 2), (2, 3), (1, 3)}}

Before we start we havent't removed any vertices so,
removed_vertices = {}

A *level 1* vertex has one and only one incoming edge, for this example we don't have any *level 1* vertex, so 
:math:`L1_v` = {}

A *level 2* vertex is one that has one incoming edge and one outgoing edges, for this example vertex 2 is a *level 2* vertex, so :math:`L2_v` = {2}

For this example we will cycle only once.
    
    Cycle 1:

        Level 1:

            :math:`L1_v` is empty, so we do not perform any *level 1* contraction in Cycle 1.

        Level 2:

            :math:`L2_v` is not empty, therefore on *level 2* contraction, vertex 2 gets deleted, and will add a new edge(shortcut) with an id = -1 and cost = 2, between vertex 1 and vertex 3.

            After removal of vertex 2 the sets change as follows,
            :math:`L1_v` = {}
            :math:`L2_v` = {}
            G = {V:{1, 3}, E:{(1, 3), -1}}
            removed_vertices = {(e, -1):{2}}

            Since :math:`L2_v` is empty we stop *level 2* contraction in Cycle 1.

        After Cycle 1,the sets change as follows,

        :math:`L1_v` = {}
        :math:`L2_v` = {}
        G = {V:{1, 3}, E:{(1, 3), -1}}
        removed_vertices = {(e, -1):{2}}

        Since both of the above sets are empty, we cannot contract further and the contraction ends after Cycle 1.

So finally,the contracted graph is represented as 
    removed_vertices = {(e, -1):{2}}
    G = {V:{1, 3}, E:{(1, 3), -1}






