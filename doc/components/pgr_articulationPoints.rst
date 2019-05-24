..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_articulationPoints - Proposed
===============================================================================

``pgr_articulationPoints`` - Return the articulation points of an undirected graph.
In particular, the algorithm implemented by Boost.Graph.

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/biconnected_components.html

   Boost Graph Inside

.. include:: proposed.rst
   :start-after: stable-begin-warning
   :end-before: stable-end-warning

.. rubric:: Availability

* On v3.0.0

  * Set as `proposed`
  * Return columns change: ``seq`` is removed

* New as experimental on v2.5.0

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
    single: articulationPoints -- Proposed

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
* Boost: `Biconnected components & articulation points <http://www.boost.org/libs/graph/doc/biconnected_components.html>`__
* wikipedia: `Biconnected component <http://en.wikipedia.org/wiki/Biconnected_component>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

