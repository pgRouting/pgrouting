.. _contraction:

Contraction
===============================================================================

The documentation provides explanation on graph contraction.

*level 1* vertices have only one incoming edge.
*level 2* vertices have one incoming edge and one outgoing edge.

*Notations*

    V: is the set of vertices
    E: is the set of edges
    G: is the graph
    :math:`L1_v`: is the set of *level 1* vertices 
    :math:`L2_v`: is the set of *level 2* vertices
    removed_vertices: is the set of removed vertices

    removed_v={(v,1):{2}, (e,-1):{3}}
    This notation indicates:
        vertex 2 is removed, and belongs to vertex 1 subgraph
        vertex 3 is removed, and belongs to edge -1



*Procedure*

    For contracting, we are going to cycle as follows
    .. code_block:: none

    do N times {
    repeat {
    < work with level 1 vertices >
    until :math:`L1_v` is empty
    }
    repeat {
    < work with level 2 vertices >
    until :math:`L2_v` is empty
    }
    }

For simplicity all the edges in the examples have unit weight.


*Examples*

.. _fig1:

.. image:: images/twoNodesOneEdge.png

    *Graph 1: Directed, with two nodes and one edge*

    G={V:{1, 2}, E:{(1, 2)}}

    Before we start we did not remove any vertices so, removed_vertices={}

    A *level 1* vertex has one and only one incoming edge, for this example vertex 2 is a *level 1* vertex, so 
:math:`L1_v`={2}

    A *level 2* vertex is one that has one incoming edge and one outgoing edge, for this example we don't have *level 2* vertices, so
    
:math:`L2_v`={}

    For this example we will cycle only once

    Cycle 1:

        Level 1:

            :math:`L1_v` is not empty, therefore on *level 1* contraction, vertex 2 gets deleted.

            removed_vertices={(v, 1):{2}}

            After removal of vertex 2, vertex 1 loses an outgoing edge and the sets change as follows,
            :math:`L1_v`={}
            :math:`L2_v`={}
            G={V:{1}, E:{}}

            Since :math:`L1_v` is empty we stop *level 1* contraction in Cycle 1.

        Level 2:

            :math:`L2_v` is empty, so we do not perform any *level 2* contraction in Cycle 1.

        After Cycle 1, the sets change as follows,

        :math:`L1_v`={}
        :math:`L2_v`={}
        G={V:{1}, E:{}

        Since both of the above sets are empty, we cannot contract further and the contraction ends after Cycle 1.

    So finally,
        removed_vertices={(v, 1):{2}}
        G={V:{1}, E:{}}        


.. _fig2:

.. image:: images/threeNodestwoEdges.png

    *Graph 2: Directed, with three nodes and two edges*

    G={V:{1, 2, 3}, E:{(1, 2), (2, 3)}}

    Before we start we did not remove any vertices so,
    removed_vertices={}

    A *level 1* vertex has one and only one incoming edge, for this example vertex 3 is a *level 1* vertex, so 
:math:`L1_v`={3}

    A *level 2* vertex is one that has one incoming edge and one outgoing edge, for this example vertex 2 is a *level 2* vertex, so :math:`L2_v`={2}

    For this example we will cycle only once
    
    Cycle 1:

        Level 1:

            :math:`L1_v` is not empty,therefore on *level 1* contraction, vertex 3 gets deleted.

            removed_vertices={(v, 2):{3}}

            After removal of vertex 3, vertex 2 loses an outgoing edge and the sets change as follows,
            :math:`L1_v`={2}
            :math:`L2_v`={}
            G={V:{1, 2}, E:{(1, 2)}}

            :math:`L1_v` is not empty, therefore on *level 1* contraction, vertex 2 gets deleted.

            removed_vertices={(v, 1):{2, 3}}

            After removal of vertex 2, vertex 1 loses an outgoing edge and the sets change as follows,
            :math:`L1_v`={}
            :math:`L2_v`={}
            G={V:{1}, E:{}}

            Since :math:`L1_v` is empty we stop *level 1* contraction in Cycle 1.

        Level 2:

            :math:`L2_v` is empty, so we do not perform any *level 2* contraction in Cycle 1.

        After Cycle 1, the sets change as follows,

        :math:`L1_v`={}
        :math:`L2_v`={}
        G={V:{1}, E:{}}

        Since both of the above sets are empty, we cannot contract further and the contraction ends after Cycle 1.

    So finally,
        removed_vertices={(v, 1):{2, 3}}
        G={V:{1}, E:{}}


.. _fig3:

.. image:: images/threeNodesthreeEdges.png

    *Graph 3: Directed, with three nodes and three edges*

    G={V:{1, 2, 3},E:{(1, 2), (2, 3), (1, 3)}}

    Before we start we did not remove any vertices so,
    removed_vertices={}

    A *level 1* vertex has one and only one incoming edge, for this example we don't have any *level 1* vertex, so 
:math:`L1_v`={}

    A *level 2* vertex is one that has one incoming edge and one outgoing edge, for this example vertex 2 is a *level 2* vertex, so :math:`L2_v`={2}

    For this example we will cycle only once
    
    Cycle 1:

        Level 1:

            :math:`L1_v` is empty, so we do not perform any *level 1* contraction in Cycle 1.

        Level 2:

            :math:`L2_v` is not empty,therefore on *level 2* contraction, vertex 2 gets deleted, and will add a new edge(shortcut) with an id=-1 and cost=2,between vertex 1 and vertex 3.

            removed_vertices={(e, -1):{2}}

            After removal of vertex 2 the sets change as follows,
            :math:`L1_v`={}
            :math:`L2_v`={}
            G={V:{1, 3}, E:{(1, 3), -1}}

            Since :math:`L2_v` is empty we stop *level 2* contraction in Cycle 1.

        After Cycle 1,the sets change as follows,

        :math:`L1_v`={}
        :math:`L2_v`={}
        G={V:{1, 3}, E:{(1, 3), -1}}

        Since both of the above sets are empty, we cannot contract further and the contraction ends after Cycle 1.

    So finally,
        removed_vertices={(e, -1):{2}}
        G={V:{1, 3}, E:{(1, 3), -1}






