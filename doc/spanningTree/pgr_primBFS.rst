..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


``pgr_primBFS``
===============================================================================

``pgr_primBFS`` — Prim's algorithm for Minimum Spanning Tree with Depth First
Search ordering.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/prim_minimum_spanning_tree.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.0.0

  * New **Official** function


Description
-------------------------------------------------------------------------------

Visits and extracts the nodes information in Breath First Search ordering
of the Minimum Spanning Tree created with Prims's algorithm.

**The main Characteristics are:**

.. include:: prim-family.rst
   :start-after: prim-description-start
   :end-before: prim-description-end

- Returned tree nodes from a root vertex are on Breath First Search order
- Breath First Search Running time: :math:`O(E + V)`

Signatures
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_primBFS(`Edges SQL`_, **root vid**, [``max_depth``])
   | pgr_primBFS(`Edges SQL`_, **root vids**, [``max_depth``])

   | RETURNS SET OF |result-bfs|

.. index::
    single: primBFS(Single vertex)

Single vertex
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_primBFS(`Edges SQL`_, **root vid**, [``max_depth``])

   | RETURNS SET OF |result-bfs|

:Example: The Minimum Spanning Tree having as root vertex :math:`6`

.. literalinclude:: doc-pgr_primBFS.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: primBFS(Multiple vertices)

Multiple vertices
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_primBFS(`Edges SQL`_, **root vids**, [``max_depth``])

   | RETURNS SET OF |result-bfs|

:Example: The Minimum Spanning Tree starting on vertices :math:`\{9, 6\}` with
          :math:`depth \leq 3`

.. literalinclude:: doc-pgr_primBFS.queries
   :start-after: -- q2
   :end-before: -- q3

Parameters
-------------------------------------------------------------------------------

.. include:: BFS-category.rst
   :start-after: mst-bfs-dfs-params_start
   :end-before: mst-bfs-dfs-params_end

BFS optional parameters
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
* The queries use the :doc:`sampledata` network.
* `Boost: Prim's algorithm documentation
  <https://www.boost.org/libs/graph/doc/prim_minimum_spanning_tree.html>`__
* `Wikipedia: Prim's algorithm
  <https://en.wikipedia.org/wiki/Prim%27s_algorithm>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
