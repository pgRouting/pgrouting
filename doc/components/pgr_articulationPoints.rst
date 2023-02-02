..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_articulationPoints.html>`__
  (`3.5 <https://docs.pgrouting.org/3.5/en/pgr_articulationPoints.html>`__)
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_articulationPoints.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_articulationPoints.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_articulationPoints.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_articulationPoints.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_articulationPoints.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_articulationPoints.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_articulationPoints.html>`__


``pgr_articulationPoints``
===============================================================================

``pgr_articulationPoints`` - Return the articulation points of an undirected
graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/biconnected_components.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.0.0

  * Return columns change: ``seq`` is removed
  * **Official** function

* Version 2.5.0

  * New **experimental** function

Description
-------------------------------------------------------------------------------

Those vertices that belong to more than one biconnected component are called
articulation points or, equivalently, cut vertices.
Articulation points are vertices whose removal would increase the number of
connected components in the graph.
This implementation can only be used with an undirected graph.

**The main characteristics are:**

- Works for **undirected** graphs.
- The returned values are ordered:

  - ``node`` ascending

- Running time: :math:`O(V + E)`

.. index::
    single: articulationPoints

Signatures
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_articulationPoints(`Edges SQL`_)

   | RETURNS SET OF |result-node|
   | OR EMPTY SET

:Example: The articulation points of the graph

.. literalinclude:: doc-pgr_articulationPoints.queries
   :start-after: -- q1
   :end-before: -- q2

Nodes in red are the articulation points.

.. figure:: /images/ap_sampledata.png
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

Returns set of |result-node|

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``node``
     - ``BIGINT``
     - Identifier of the vertex.

See Also
-------------------------------------------------------------------------------

* :doc:`components-family`
* The queries use the :doc:`sampledata` network.
* Boost: `Biconnected components & articulation points
  <https://www.boost.org/libs/graph/doc/biconnected_components.html>`__
* wikipedia: `Biconnected component
  <https://en.wikipedia.org/wiki/Biconnected_component>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

