..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_prim.html>`__
  (`3.6 <https://docs.pgrouting.org/3.6/en/pgr_prim.html>`__)
  `3.5 <https://docs.pgrouting.org/3.5/en/pgr_prim.html>`__
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_prim.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_prim.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_prim.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_prim.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_prim.html>`__

``pgr_prim``
===============================================================================

``pgr_prim`` — Minimum spanning forest of a graph using Prim's algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/prim_minimum_spanning_tree.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.0.0

  * New **Official** function

Description
-------------------------------------------------------------------------------

This algorithm finds the minimum spanning forest in a possibly disconnected
graph using Prim's algorithm.

**The main characteristics are:**

.. include:: prim-family.rst
   :start-after: prim-description-start
   :end-before: prim-description-end

- EMPTY SET is returned when there are no edges in the graph.


Signatures
-------------------------------------------------------------------------------

.. index::
    single: prim

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_prim(`Edges SQL`_)

   | RETURNS SET OF |result-mst|
   | OR EMPTY SET


:Example: Minimum spanning forest of a subgraph

.. literalinclude:: doc-pgr_prim.queries
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
* :doc:`prim-family`
* The queries use the :doc:`sampledata` network.
* `Boost: Prim's algorithm documentation
  <https://www.boost.org/libs/graph/doc/prim_minimum_spanning_tree.html>`__
* `Wikipedia: Prim's algorithm
  <https://en.wikipedia.org/wiki/Prim%27s_algorithm>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

