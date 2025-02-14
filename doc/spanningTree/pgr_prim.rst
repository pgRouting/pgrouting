..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: Prim Family ; pgr_prim
   single: Spanning Tree Category ; pgr_prim
   single: prim

|

``pgr_prim``
===============================================================================

``pgr_prim`` — Minimum spanning forest of a graph using Prim's algorithm.

.. rubric:: Availability

* Version 3.0.0

  * New official function.

Description
-------------------------------------------------------------------------------

This algorithm finds the minimum spanning forest in a possibly disconnected
graph using Prim's algorithm.

**The main characteristics are:**

.. include:: prim-family.rst
   :start-after: prim-description-start
   :end-before: prim-description-end

- EMPTY SET is returned when there are no edges in the graph.

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_prim(`Edges SQL`_)

   | Returns set of |result-mst|
   | OR EMPTY SET


:Example: Minimum spanning forest of a subgraph

.. literalinclude:: prim.queries
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

Result columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
   :start-after: r-edge-cost-start
   :end-before: r-edge-cost-end

See Also
-------------------------------------------------------------------------------

* :doc:`spanningTree-category`
* :doc:`prim-family`
* :doc:`sampledata`
* `Boost: Prim's algorithm documentation
  <https://www.boost.org/libs/graph/doc/prim_minimum_spanning_tree.html>`__
* `Wikipedia: Prim's algorithm
  <https://en.wikipedia.org/wiki/Prim%27s_algorithm>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

