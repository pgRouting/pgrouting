..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_biconnectedComponents.html>`__
  (`3.5 <https://docs.pgrouting.org/3.5/en/pgr_biconnectedComponents.html>`__)
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_biconnectedComponents.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_biconnectedComponents.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_biconnectedComponents.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_biconnectedComponents.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_biconnectedComponents.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_biconnectedComponents.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_biconnectedComponents.html>`__


``pgr_biconnectedComponents``
===============================================================================

``pgr_biconnectedComponents`` — Biconnected components of an undirected graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/biconnected_components.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.0.0

  * Return columns change:

    *  ``n_seq`` is removed
    * ``seq`` changed type to ``BIGINT``

  * **Official** function

* Version 2.5.0

  * New **experimental** function


Description
-------------------------------------------------------------------------------

The biconnected components of an undirected graph are the maximal subsets of
vertices such that the removal of a vertex from particular component will not
disconnect the component.
Unlike connected components, vertices may belong to multiple biconnected
components.
Vertices can be present in multiple biconnected components, but each edge can
only be contained in a single biconnected component.

**The main characteristics are:**

- Works for **undirected** graphs.
- Components are described by edges.
- The returned values are ordered:

  - ``component`` ascending.
  - ``edge`` ascending.

- Running time: :math:`O(V + E)`

.. index::
    single: biconnectedComponents

Signatures
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_biconnectedComponents(`Edges SQL`_)

   | RETURNS SET OF |result-component-E|
   | OR EMPTY SET

:Example: The biconnected components of the graph

.. literalinclude:: doc-pgr_biconnectedComponents.queries
   :start-after: -- q1
   :end-before: -- q2

.. figure:: /images/bcc_sampledata.png
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

Returns set of |result-component-E|

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``seq``
     - ``BIGINT``
     - Sequential value starting from **1**.
   * - ``component``
     - ``BIGINT``
     - Component identifier.

       * Has the value of the minimum edge identifier in the component.
   * - ``edge``
     - ``BIGINT``
     - Identifier of the edge that belongs to the ``component``.

See Also
-------------------------------------------------------------------------------

* :doc:`components-family`
* The queries use the :doc:`sampledata` network.
* Boost: `Biconnected components
  <https://www.boost.org/libs/graph/doc/biconnected_components.html>`__
* wikipedia: `Biconnected component
  <https://en.wikipedia.org/wiki/Biconnected_component>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
