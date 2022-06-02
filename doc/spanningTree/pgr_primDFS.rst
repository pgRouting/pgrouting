..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_primDFS.html>`__
  (`3.4 <https://docs.pgrouting.org/3.4/en/pgr_primDFS.html>`__)
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_primDFS.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_primDFS.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_primDFS.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_primDFS.html>`__

``pgr_primDFS``
===============================================================================

``pgr_primDFS`` — Prim algorithm for Minimum Spanning Tree with Depth First
Search ordering.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/prim_minimum_spanning_tree.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.0.0

  * New **Official** function


Description
-------------------------------------------------------------------------------

Visits and extracts the nodes information in Depth First Search ordering
of the Minimum Spanning Tree created using Prims's algorithm.

**The main Characteristics are:**

.. include:: prim-family.rst
   :start-after: prim-description-start
   :end-before: prim-description-end

- Returned tree nodes from a root vertex are on Depth First Search order
- Depth First Search Running time: :math:`O(E + V)`

Signatures
-------------------------------------------------------------------------------

.. parsed-literal::

    pgr_primDFS(`Edges SQL`_, **Root vid** [, max_depth])
    pgr_primDFS(`Edges SQL`_, **Root vids** [, max_depth])
    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

.. index::
    single: primDFS(Single vertex)

Single vertex
...............................................................................

.. parsed-literal::

    pgr_primDFS(`Edges SQL`_, **Root vid** [, max_depth])
    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

:Example: The Minimum Spanning Tree having as root vertex :math:`6`

.. literalinclude:: doc-pgr_primDFS.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: primDFS(Multiple vertices)

Multiple vertices
...............................................................................

.. parsed-literal::

    pgr_primDFS(`Edges SQL`_, **Root vids** [, max_depth])
    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

:Example: The Minimum Spanning Tree starting on vertices :math:`\{9, 6\}` with
          :math:`depth \leq 3`

.. literalinclude:: doc-pgr_primDFS.queries
   :start-after: -- q2
   :end-before: -- q3

Parameters
-------------------------------------------------------------------------------

.. include:: BFS-category.rst
   :start-after: mst-bfs-dfs-params_start
   :end-before: mst-bfs-dfs-params_end

DFS optional parameters
...............................................................................

.. include:: BFS-category.rst
   :start-after: max-depth-optional-start
   :end-before: max-depth-optional-end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
   :start-after: basic_edges_sql_start
   :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

.. include:: BFS-category.rst
   :start-after: mst-bfs-dfs-dd-result-columns-start
   :end-before: mst-bfs-dfs-dd-result-columns-end

See Also
-------------------------------------------------------------------------------

* :doc:`spanningTree-family`
* :doc:`prim-family`
* :doc:`sampledata`
* `Boost: Prim's algorithm documentation
  <https://www.boost.org/libs/graph/doc/prim_minimum_spanning_tree.html>`__
* `Wikipedia: Prim's algorithm
  <https://en.wikipedia.org/wiki/Prim%27s_algorithm>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
