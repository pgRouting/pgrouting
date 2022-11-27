..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_primDD.html>`__
  (`3.5 <https://docs.pgrouting.org/3.5/en/pgr_primDD.html>`__)
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_primDD.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_primDD.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_primDD.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_primDD.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_primDD.html>`__

``pgr_primDD``
===============================================================================

``pgr_primDD`` — Catchament nodes using Prim's algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/prim_minimum_spanning_tree.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.0.0

  * New **Official** function

Description
-------------------------------------------------------------------------------

Using Prim's algorithm, extracts the nodes that have aggregate costs less than
or equal to a **distance** from a **root** vertex (or vertices) within the
calculated minimum spanning tree.

**The main Characteristics are:**

.. include:: prim-family.rst
   :start-after: prim-description-start
   :end-before: prim-description-end

.. include:: drivingDistance-category.rst
   :start-after: dd_traits_start
   :end-before: dd_traits_end

- Returned tree nodes from a root vertex are on Depth First Search order.
- Depth First Search running time: :math:`O(E + V)`

Signatures
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_primDD(`Edges SQL`_, **root vid**, **distance**)
   | pgr_primDD(`Edges SQL`_, **root vids**, **distance**)

   | RETURNS SET OF |result-bfs|

.. index::
    single: primDD(Single vertex)

Single vertex
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_primDD(`Edges SQL`_, **root vid**, **distance**)

   | RETURNS SET OF |result-bfs|

:Example: The Minimum Spanning Tree starting on vertex :math:`6` with
          :math:`distance \leq 3.5`

.. literalinclude:: doc-pgr_primDD.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: primDD(Multiple vertices)

Multiple vertices
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_primDD(`Edges SQL`_, **root vids**, **distance**)

   | RETURNS SET OF |result-bfs|

:Example: The Minimum Spanning Tree starting on vertices :math:`\{9, 6\}` with
          :math:`distance \leq 3.5`

.. literalinclude:: doc-pgr_primDD.queries
   :start-after: -- q2
   :end-before: -- q3

Parameters
-------------------------------------------------------------------------------

.. include:: drivingDistance-category.rst
   :start-after: mst-dd-params_start
   :end-before: mst-dd-params_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
..............................................................................

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

