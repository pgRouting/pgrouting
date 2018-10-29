..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_kruskal - Experimental
===============================================================================

``pgr_kruskal`` — Returns the minimum spanning tree of graph using Kruskal algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/doc/libs/1_64_0/libs/graph/doc/kruskal_min_spanning_tree.html

   Boost Graph Inside

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Experimental on v3.0.0

Description
-------------------------------------------------------------------------------

Kruskal's algorithm is a greedy minimum spanning tree algorithm that in each
cycle finds and adds the edge
of the least possible weight that connects any two trees in the forest.
It is a greedy algorithm, as it finds a minimum spanning tree for a
connected weighted undirected graph by adding increasing cost arcs at each step.

**The main Characteristics are:**

- It's implementation is only on **undirected** graph.
- Process is done only on edges with positive costs.
- The total weight of all the edges in the tree or forest is minimized.
- If the graph is connected

  - The resulting edges make up a tree

- If the graph is not connected,

  - Finds a minimum spanning tree for each connected component.
  - The resulting edges make up a forest.

- EMPTY SET is returned when there are no edges in the graph.
- Returned edges are in the order they were added to the tree.
- Running time: :math:`O(E * log E)`

Signatures
-------------------------------------------------------------------------------

.. index::
    single: kruskal - Experimental

.. code-block:: none

    pgr_kruskal(edges_sql)

    RETURNS SET OF (seq, edge, cost)
    OR EMPTY SET


:Example: A minimum Spanning Tree

.. literalinclude:: doc-pgr_kruskal.queries
   :start-after: -- q1
   :end-before: -- q2

Parameters
-------------------------------------------------------------------------------

=================== ====================== ========= =================================================
Parameter           Type                   Default   Description
=================== ====================== ========= =================================================
**edges_sql**       ``TEXT``                         SQL query described in `Inner query`_.
=================== ====================== ========= =================================================

Inner query
-------------------------------------------------------------------------------

.. rubric::edges_sql

.. include:: pgRouting-concepts.rst
   :start-after: basic_edges_sql_start
   :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

Returns set of ``(seq, component, edge, cost, tree_cost)``

===============  =========== ====================================================
Column           Type        Description
===============  =========== ====================================================
**seq**          ``BIGINT``  Sequential value starting from **1**.
**edge**         ``BIGINT``  Identifier of the edge.
**cost**         ``FLOAT``   Cost to traverse of edge.
===============  =========== ====================================================


See Also
-------------------------------------------------------------------------------

* https://en.wikipedia.org/wiki/Kruskal%27s_algorithm
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

