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

The documentation provides explanation on graph contraction using small examples.

level1 vertices have only one incoming edge.
level2 vertices have one incoming edge and one outgoing edge.

For simplicity all the edges in the examples have unit weight.

.. _fig1:

.. figure:: images/twoNodesOneEdge.png

    *Graph 1: Directed, with two nodes and one edge*

    level1_vertices={2}
    level2_vertices={0}

    Cycle 1:

        Level 1:

            level1_vertices is not empty,therefore on level1 contraction, vertex 2 gets deleted.

            removed_vertices={(v,1):{2}}

            After removal of vertex 2,vertex 1 loses an outgoing edge and the sets change as follows,
            level1_vertices={}
            level2_vertices={}

            Since level1_vertices is empty we stop level1 contraction in Cycle 1.

        Level 2:

            level2_vertices is empty, so we do not perform any level2 contraction in Cycle 1.

        After Cycle 1,the sets change as follows,

        level1_vertices={}
        level2_vertices={}

        Since both of the above sets are empty, we cannot contract further and the contraction ends after Cycle 1.


.. _fig2:

.. figure:: images/threeNodestwoEdges.png

    *Graph 1: Directed, with three nodes and two edges*

    level1_vertices={3}
    level2_vertices={2}

    Cycle 1:

        Level 1:

            level1_vertices is not empty,therefore on level1 contraction, vertex 3 gets deleted.

            removed_vertices={(v,2):{3}}

            After removal of vertex 3,vertex 2 loses an outgoing edge and the sets change as follows,
            level1_vertices={2}
            level2_vertices={}

            level1_vertices is not empty,therefore on level1 contraction, vertex 2 gets deleted.

            removed_vertices={(v,1):{2,3}}

            After removal of vertex 2,vertex 1 loses an outgoing edge and the sets change as follows,
            level1_vertices={}
            level2_vertices={}

            Since level1_vertices is empty we stop level1 contraction in Cycle 1.

        Level 2:

            level2_vertices is empty, so we do not perform any level2 contraction in Cycle 1.

        After Cycle 1,the sets change as follows,

        level1_vertices={}
        level2_vertices={}

        Since both of the above sets are empty, we cannot contract further and the contraction ends after Cycle 1.



.. _fig3:

.. figure:: images/threeNodesthreeEdges.png

    *Graph 1: Directed, with three nodes and three edges*

    level1_vertices={}
    level2_vertices={2}

    Cycle 1:

        Level 1:

            level1_vertices is empty, so we do not perform any level1 contraction in Cycle 1.

        Level 2:

            level2_vertices is not empty,therefore on level2 contraction, vertex 2 gets deleted, and will add a new edge(shortcut) with and id=-1 and cost=2,between vertex 1 and vertex 3.

            removed_vertices={(e,-1):{2}}

            After removal of vertex 2 the sets change as follows,
            level1_vertices={}
            level2_vertices={}

            Since level2_vertices is empty we stop level2 contraction in Cycle 1.

        After Cycle 1,the sets change as follows,

        level1_vertices={}
        level2_vertices={}

        Since both of the above sets are empty, we cannot contract further and the contraction ends after Cycle 1.







