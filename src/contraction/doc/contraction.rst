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

Introduction
+++++++++++++

Contracting a graph becomes a crucial operation when talking about big graphs like the graphs involved in routing across cities, countries, continents or the whole world.

The contraction level and contraction operations can become very complex, as the complexity of the graph grows.

Currently, this implementation makes our contraction algorithm simple as possible so that more contraction operations can be added in the future.

We are not aiming with this work to implement all the possible contraction operations but to give a framework such that adding a contraction operation can be easily achieved.

Currently, this implementation supports two operations:

 1. Dead end contraction
 2. Linear contraction

And with the additional characteristics:

  - The user can forbid to contract a particular set of nodes or edges.
  - The user can decide how many times the cycle can be done.
  - The user can decide the order of the operations on a cycle.

Notation
++++++++++

* V: is the set of vertices
* E: is the set of edges
* G: is the graph
* :math:`V1`: is the set of *dead end* vertices 
* :math:`V2`: is the set of *linear* vertices
* removed_vertices: is the set of removed vertices

Dead end contraction
+++++++++++++++++++++

Characteristics:

  - :math:`V1`: set of vertices with no outgoing edges and more than one incoming edges in increasing order of id:

    - Edges with the same identifier are considered the same edge
      and if it has the `reverse_cost` valid the outgoing edge is ignored.

.. code-block:: none


    while ( V1 is not empty ) {

        delete vertex of V1
        add the deleted vertex to removed_vertices
        vertex that leads to removed vertex, inherits the removed vertex

        <adjust any conditions that might affect other contraction operation>
    }

Example:

* Perform 1 cycle of dead end contraction.

.. image:: images/twoNodesoneEdge_a.png

:Input:  G = {V:{1, 2}, E:{(1, 2)}}

:initial set up:

.. code-block:: none

    removed_vertices={}
    V1 = {2}
    V2 = {}


:procedure:

.. code-block:: none

    V1 = {2} is not empty

        V1 = {}
        V2 = {}
        G = {V:{1}, E:{}}
        removed_vertices = {(v, 1):{2}}.

    V1 is empty

Since V1 is empty we go on to the next contraction operation
    
.. code-block:: none

    V2 is empty

:Results:

.. code-block:: none

    G = {V:{1}, E:{}}
    removed_vertices = {(v, 1):{2}}

Visually the results are
    
.. image:: images/twoNodesoneEdge_b.png


Linear contraction
+++++++++++++++++++++

Characteristics:

  - :math:`V2`: vertex with more than one incoming edge and more than one outgoing edge:

    - The outgoing edge must have different identifier of the incoming edge

.. code-block:: none

    while ( V2 is not empty ) {

        delete vertex of V2
        create edge (shortcut)
        add the deleted vertex to removed_vertices
        the newly created edge, inherits the removed vertex

        <adjust any conditions that might affect other contraction operations>
    }

Example:

* Perform 1 cycle of linear contraction.

.. image:: images/threeNodestwoEdges_a.png


:Input: G = {V:{1, 2, 3}, E:{(1, 2), (2, 3)}}


:initial set up:

.. code-block:: none

    removed_vertices={}
    V1 = {3}
    V2 = {2}

:procedure:

.. code-block:: none

    V2 = {2} is not empty

        V1 = {3}
        removed_vertices = {(e, -1):{2}}
        V2 = {}
        G = {V:{1, 3}, E:{-1(1,3,c=2)}}

    V2 is empty

.. image:: images/threeNodestwoEdges_b.png

Since V2 is empty we go on to the next contraction operation
    
.. code-block:: none

    V1 = {3} is not empty

        V1 = {}
        V2 = {}
        removed_vertices = {(v, 1):{3, 2}}.
        G = {V:{1}, E:{}}

    V1 is empty


:Results:

.. code-block:: none

    removed_vertices = {(v, 1):{3, 2}}.
    G = {V:{1}, E:{}}

Visually the results are
    
.. image:: images/threeNodestwoEdges_c.png

The contraction skeleton
-------------------------------------------------------------------------------

In general we have an initial set up that may involve analyzing the graph given as input and setting the non contractable nodes or edges. We have a cycle that will go and perform a contraction operation
until while possible, and then move to the next contraction operation.
Adding a new operation then becomes an "easy" task but more things might be involved, because the characteristics of the graph change each time its contracted, so some interaction between contractions
is also implemented.

Procedure
---------

* For contracting, we are going to cycle as follows

.. code-block:: none

    input: G(V,E);
    removed_vertices = {};

    <input>
    do max_cycles times {
        for (operation in operations_order) ) {
            do operation 
        }
    }
    <output>


References
++++++++++++++++++++++++++

* http://www.cs.cmu.edu/afs/cs/academic/class/15210-f12/www/lectures/lecture16.pdf
* http://algo2.iti.kit.edu/documents/routeplanning/geisberger_dipl.pdf

