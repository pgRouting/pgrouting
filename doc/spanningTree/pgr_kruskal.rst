..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_kruskal.html>`__
  (`3.5 <https://docs.pgrouting.org/3.5/en/pgr_kruskal.html>`__)
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_kruskal.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_kruskal.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_kruskal.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_kruskal.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_kruskal.html>`__

``pgr_kruskal``
===============================================================================

``pgr_kruskal`` — Minimum spanning tree of a graph using Kruskal's algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/kruskal_min_spanning_tree.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.0.0

  * New **Official** function


Description
-------------------------------------------------------------------------------

This algorithm finds the minimum spanning forest in a possibly disconnected
graph using Kruskal's algorithm.

**The main Characteristics are:**

.. include:: kruskal-family.rst
   :start-after: kruskal-description-start
   :end-before: kruskal-description-end

- EMPTY SET is returned when there are no edges in the graph.

Signatures
-------------------------------------------------------------------------------

.. index::
    single: kruskal

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_kruskal(`Edges SQL`_)

   | RETURNS SET OF |result-mst|
   | OR EMPTY SET

:Example: Minimum spanning forest

.. literalinclude:: doc-pgr_kruskal.queries
   :start-after: -- q1
   :end-before: -- q2

Parameters
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
   :start-after: only_edge_param_start
   :end-before: only_edge_param_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
   :start-after: basic_edges_sql_start
   :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
   :start-after: r-edge-cost-start
   :end-before: r-edge-cost-end

See Also
-------------------------------------------------------------------------------

* :doc:`spanningTree-family`
* :doc:`kruskal-family`
* The queries use the :doc:`sampledata` network.
* `Boost: Kruskal's algorithm
  <https://www.boost.org/libs/graph/doc/kruskal_min_spanning_tree.html>`__
* `Wikipedia: Kruskal's algorithm
  <https://en.wikipedia.org/wiki/Kruskal's_algorithm>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
