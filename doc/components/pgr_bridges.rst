..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_bridges.html>`__
  (`3.6 <https://docs.pgrouting.org/3.6/en/pgr_bridges.html>`__)
  `3.5 <https://docs.pgrouting.org/3.5/en/pgr_bridges.html>`__
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_bridges.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_bridges.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_bridges.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_bridges.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_bridges.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_bridges.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_bridges.html>`__

``pgr_bridges``
===============================================================================

``pgr_bridges`` - Return the bridges of an undirected graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/connected_components.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.0.0

  * Return columns change: ``seq`` is removed
  * **Official** function

* Version 2.5.0

  * New **experimental** function

Description
-------------------------------------------------------------------------------

A bridge is an edge of an undirected graph whose deletion increases its number
of connected components.
This implementation can only be used with an undirected graph.

**The main characteristics are:**

- Works for **undirected** graphs.
- The returned values are ordered:

  - ``edge`` ascending

- Running time: :math:`O(E * (V + E))`

.. index::
    single: bridges

Signatures
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_bridges(`Edges SQL`_)

   | RETURNS SET OF |result-edge|
   | OR EMPTY SET

:Example: The bridges of the graph

.. literalinclude:: doc-pgr_bridges.queries
   :start-after: -- q1
   :end-before: -- q2

.. figure:: /images/bridge_sampledata.png
   :scale: 25%

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

Returns set of |result-edge|

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``edge``
     - ``BIGINT``
     - Identifier of the edge that is a bridge.

See Also
-------------------------------------------------------------------------------

* https://en.wikipedia.org/wiki/Bridge_%28graph_theory%29
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
