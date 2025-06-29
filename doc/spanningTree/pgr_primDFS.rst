..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: Prim Family ; pgr_primDFS
   single: Spanning Tree Category ; pgr_primDFS
   single: Depth First Search Category ; pgr_primDFS
   single: primDFS

|

``pgr_primDFS``
===============================================================================

``pgr_primDFS`` — Prim algorithm for Minimum Spanning Tree with Depth First
Search ordering.

.. rubric:: Availability

:Version 3.7.0:

* Standardizing output columns to |result-spantree|

  * Added ``pred`` result columns.

:Version 3.0.0:

  * New official function.

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

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_primDFS(`Edges SQL`_, **root vid**, [``max_depth``])
   | pgr_primDFS(`Edges SQL`_, **root vids**, [``max_depth``])

   | Returns set of |result-spantree|

.. index::
   single: primDFS ; Single vertex

Single vertex
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_primDFS(`Edges SQL`_, **root vid**, [``max_depth``])

   | Returns set of |result-spantree|

:Example: The Minimum Spanning Tree having as root vertex :math:`6`

.. literalinclude:: primDFS.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: primDFS ; Multiple vertices

Multiple vertices
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_primDFS(`Edges SQL`_, **root vids**, [``max_depth``])

   | Returns set of |result-spantree|

:Example: The Minimum Spanning Tree starting on vertices :math:`\{9, 6\}` with
          :math:`depth \leq 3`

.. literalinclude:: primDFS.queries
   :start-after: -- q2
   :end-before: -- q3

Parameters
-------------------------------------------------------------------------------

.. include:: drivingDistance-category.rst
    :start-after: spantree-params_start
    :end-before: spantree-params_end

DFS optional parameters
...............................................................................

.. include:: BFS-category.rst
   :start-after: max-depth-optional-start
   :end-before: max-depth-optional-end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
..............................................................................

.. include:: pgRouting-concepts.rst
   :start-after: basic_edges_sql_start
   :end-before: basic_edges_sql_end

Result columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
   :start-after: spantree-result-columns-start
   :end-before: spantree-result-columns-end

See Also
-------------------------------------------------------------------------------

* :doc:`spanningTree-category`
* :doc:`prim-family`
* :doc:`sampledata`
* `Boost: Prim's algorithm
  <https://www.boost.org/libs/graph/doc/prim_minimum_spanning_tree.html>`__
* `Wikipedia: Prim's algorithm
  <https://en.wikipedia.org/wiki/Prim%27s_algorithm>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
