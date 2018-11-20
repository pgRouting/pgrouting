..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_primBFS - Experimental
===============================================================================

``pgr_primBFS`` — Prim algorithm for Minimum Spanning Tree with Breath First
Search ordering.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/prim_min_spanning_tree.html

   Boost Graph Inside

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Experimental

  * v3.0.0

Description
-------------------------------------------------------------------------------

Using the Prim algorithm, visits and extracts the nodes in Breath First Search
ordering

**The main Characteristics are:**

- It's implementation is only on **undirected** graph.
- Process is done only on edges with positive costs.
- Returned tree vertices are in Breath First Search order.
- Prim Running time: :math:`O(E * log E)`
- Breath First Search Running time: :math:`O(E + V)`

Signatures
-------------------------------------------------------------------------------

.. code-block:: none

    pgr_primBFS(edges_sql, root_vid [, max_depth])
    pgr_primBFS(edges_sql, root_vids [, max_depth])

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

.. index::
    single: primBFS(Single vertex) - Experimental

Single vertex
...............................................................................

.. code-block:: none

    pgr_primBFS(edges_sql, root_vid [, max_depth])

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

:Example: The Minimum Spanning Tree of the graph

.. literalinclude:: doc-pgr_primBFS.queries
   :start-after: --q1
   :end-before: --q2

.. index::
    single: primBFS(Multiple vertices) - Experimental

Multiple vertices
...............................................................................

.. code-block:: none

    pgr_primBFS(edges_sql, root_vids [, max_depth])

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

:Example: The Minimum Spanning Tree starting on vertices :math:`\{13, 2\}` with :math:`depth <= 3`

.. literalinclude:: doc-pgr_primBFS.queries
   :start-after: --q2
   :end-before: --q3

.. Parameters, Inner query & result columns

.. include:: pgr_kruskalDFS.rst
   :start-after: mstfs-information-start
   :end-before: mstfs-information-end


See Also
-------------------------------------------------------------------------------

* `Boost: Prim's documentation <https://www.boost.org/libs/graph/doc/prim_minimum_spanning_tree.html>`__
* `Wikipedia: Prim's algorithm <https://en.wikipedia.org/wiki/Prim'_algorithm>`__
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
