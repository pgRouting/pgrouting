..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_kruskalBFS - Experimental
===============================================================================

``pgr_kruskalBFS`` — Kruskal algorithm for Minimum Spanning Tree with Breath First
Search ordering.

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

Using the Kruskal algorithm, visits and extracts the nodes in Breath First Search
ordering

**The main Characteristics are:**

- It's implementation is only on **undirected** graph.
- Process is done only on edges with positive costs.
- Returned tree vertices are in Breath First Search order.
- Kruskal Running time: :math:`O(E * log E)`
- Breath First Search Running time: :math:`O(E + V)`

Signatures
-------------------------------------------------------------------------------

.. code-block:: none

    pgr_kruskalBFS(edges_sql, root_vid [, max_depth])
    pgr_kruskalBFS(edges_sql, root_vids [, max_depth])

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

.. index::
    single: kruskalBFS(Single vertex) - Experimental

Single vertex
...............................................................................

.. code-block:: none

    pgr_kruskalBFS(edges_sql, root_vid [, max_depth])

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

:Example: The Minimum Spanning Tree of the graph

.. literalinclude:: doc-pgr_kruskalBFS.queries
   :start-after: --q1
   :end-before: --q2

.. index::
    single: kruskal(Multiple vertices) - Experimental

Multiple vertices
...............................................................................

.. code-block:: none

    pgr_kruskalBFS(edges_sql, root_vids [, max_depth])

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

:Example: The Minimum Spanning Tree starting on vertices :math:`\{13, 2\}` with :math:`depth <= 3`

.. literalinclude:: doc-pgr_kruskalBFS.queries
   :start-after: --q2
   :end-before: --q3

.. parameters, inner query & return columns

.. include:: pgr_kruskalDFS.rst
   :start-after: mstfs-information-start
   :end-before: mstfs-information-end


See Also
-------------------------------------------------------------------------------

* `Boost: Kruskal's Algorithm  <https://www.boost.org/libs/graph/doc/kruskal_min_spanning_tree.html>`__
* `Wikipedia: Kruskal's Algorithm <https://en.wikipedia.org/wiki/Kruskal's_algorithm>`__
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
