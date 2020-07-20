..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_articulationPoints
===============================================================================

``pgr_articulationPoints`` - Return the articulation points of an undirected graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/biconnected_components.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.0.0

  * Return columns change: ``seq`` is removed
  * **Official** function

* Version 2.5.0

  * New **experimental** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_articulationPoints.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_articulationPoints.html>`__

* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_articulationPoints.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_articulationPoints.html>`__

Description
-------------------------------------------------------------------------------

Those vertices that belong to more than one biconnected component are called
articulation points or, equivalently, cut vertices. Articulation points are
vertices whose removal would increase the number of connected components in
the graph.
This implementation can only be used with an undirected graph.

**The main characteristics are:**

- The signature is for an **undirected** graph.
- The returned values are ordered:

  - `node` ascending

- Running time: :math:`O(V + E)`

Signatures
-------------------------------------------------------------------------------

.. index::
    single: articulationPoints

.. code-block:: none

    pgr_articulationPoints(Edges SQL)

    RETURNS SET OF (node)
    OR EMPTY SET

:Example: The articulation points of the graph

.. literalinclude:: doc-pgr_articulationPoints.queries
   :start-after: -- q1
   :end-before: -- q2

Parameters
-------------------------------------------------------------------------------

.. include:: components-family.rst
    :start-after: components_parameters_start
    :end-before: components_parameters_end

Inner query
-------------------------------------------------------------------------------

:edges SQL: an SQL query of an **undirected** graph, which should return a set of rows with the following columns:

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

.. return_articulation_start

Returns set of ``(node)``

============== ========== =================================================
Column         Type       Description
============== ========== =================================================
**node**       ``BIGINT`` Identifier of the vertex.
============== ========== =================================================

.. return_articulation_end

See Also
-------------------------------------------------------------------------------

* :doc:`components-family`
* The queries use the :doc:`sampledata` network.
* Boost: `Biconnected components & articulation points <https://www.boost.org/libs/graph/doc/biconnected_components.html>`__
* wikipedia: `Biconnected component <https://en.wikipedia.org/wiki/Biconnected_component>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

