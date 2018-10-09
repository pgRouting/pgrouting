..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_kruskal
===============================================================================

``pgr_kruskal`` — Returns the minimum spanning tree of graph using Kruskal algorithm.
In particular, the Kruskal algorithm implemented by Boost.Graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/doc/libs/1_64_0/libs/graph/doc/kruskal_min_spanning_tree.html

   Boost Graph Inside

Synopsis
-------------------------------------------------------------------------------

Kruskal's algorithm is a minimum-spanning-tree algorithm which finds an edge 
of the least possible weight that connects any two trees in the forest. It is 
a greedy algorithm in graph theory as it finds a minimum spanning tree for a 
connected weighted undirected graph adding increasing cost arcs at each step. 
This means it finds a subset of the edges that forms a tree that includes every 
vertex, where the total weight of all the edges in the tree is minimized. If the 
graph is not connected, then it finds a minimum spanning forest (a minimum 
spanning tree for each connected component).

Characteristics
-------------------------------------------------------------------------------

The main Characteristics are:

  - Process is done only on edges with positive costs.
 
  - It's implementation is only on undirected graph.
  
  - Edges are in ascending order by weight for each component.
   
  - Values are returned when there is a minimum spanning tree.

    - When there is no edge in graph then EMPTY SET is return.
  
  - Running time: :math:`O(E*log E)`


Signatures
-------------------------------------------------------------------------------

.. index::
    single: kruskal

.. code-block:: none

    pgr_kruskal(edges_sql)

    RETURNS SET OF (seq, component, edge, cost, tree_cost) 
       or EMPTY SET

The signature is for a **undirected** graph.

:Example:

.. code-block:: none

    pgr_kruskal(TEXT edges_sql);
    RETURNS SET OF (seq, component, edge, cost, tree_cost) or EMPTY SET

.. literalinclude:: doc-pgr_kruskal.queries
   :start-after: -- q1
   :end-before: -- q2

Additional example:

.. literalinclude:: doc-pgr_kruskal.queries
   :start-after: -- q2
   :end-before: -- q3

Description of the edges_sql query for kruskal functions
...............................................................................

:edges_sql: an SQL query, which should return a set of rows with the following columns:

================= =================== ======== =================================================
Column            Type                 Default  Description
================= =================== ======== =================================================
**id**            ``ANY-INTEGER``                Identifier of the edge.
**source**        ``ANY-INTEGER``                Identifier of the first end point vertex of the edge.
**target**        ``ANY-INTEGER``                Identifier of the second end point vertex of the edge.
**cost**          ``ANY-NUMERICAL``              Weight of the edge  `(source, target)`

                                                 - When negative: edge `(source, target)` does not exist, therefore it's not part of the graph.

**reverse_cost**  ``ANY-NUMERICAL``       -1     Weight of the edge `(target, source)`,

                                                 - When negative: edge `(target, source)` does not exist, therefore it's not part of the graph.

================= =================== ======== =================================================

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, REAL, FLOAT


Description of the parameters of the signatures
...............................................................................

=================== ====================== ========= =================================================
Parameter           Type                   Default   Description
=================== ====================== ========= =================================================
**edges_sql**       ``TEXT``                         SQL query as described above.
=================== ====================== ========= =================================================

Description of the return values for kruskal algorithms
.............................................................................................................................

Returns set of ``(seq, component, edge, cost, tree_cost)``

===============  =========== ====================================================
Column           Type        Description
===============  =========== ====================================================
**seq**          ``INT``     Sequential value starting from **1**.
**component**    ``BIGINT``  Lowest number of the node in the component.
**edge**         ``BIGINT``  Identifier of the edge of lightest weight.
**cost**         ``FLOAT``   Cost to traverse of edge.
**tree_cost**    ``FLOAT``   Aggregate cost of edges that is covered in spanning.
===============  =========== ====================================================
 
See Also
-------------------------------------------------------------------------------

* https://en.wikipedia.org/wiki/Kruskal%27s_algorithm
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

