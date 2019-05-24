..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_biconnectedComponents - Proposed
===============================================================================

``pgr_biconnectedComponents`` — Return the biconnected components of an undirected graph.
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
  * Return columns change: ``n_seq`` is removed

* New as experimental on v2.5.0

Description
-------------------------------------------------------------------------------

The biconnected components of an undirected graph are the maximal subsets of vertices such that the removal of a vertex from
particular component will not disconnect the component. Unlike connected components, vertices may belong to multiple biconnected
components. Vertices can be present in multiple biconnected components, but each edge can only be contained in a single biconnected
component.

**The main characteristics are:**

- The signature is for an **undirected** graph.
- Components are described by edges.
- The returned values are ordered:

  - `component` ascending.
  - `edge` ascending.

- Running time: :math:`O(V + E)`

Signatures
-------------------------------------------------------------------------------

.. index::
    single: biconnectedComponents -- Proposed

.. code-block:: none

    pgr_biconnectedComponents(Edges SQL)

    RETURNS SET OF (seq, component, edge)
    OR EMPTY SET

:Example: The biconnected components of the graph

.. literalinclude:: doc-pgr_biconnectedComponents.queries
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

.. return_componentsE_start

Returns set of ``(seq, component, edge)``

============== ========== =================================================
Column         Type       Description
============== ========== =================================================
**seq**        ``INT``    Sequential value starting from **1**.
**component**  ``BIGINT`` Component identifier. It is equal to the minimum edge identifier in the component.
**edge**       ``BIGINT`` Identifier of the edge.
============== ========== =================================================

.. return_componentsE_end

See Also
-------------------------------------------------------------------------------

* :doc:`components-family`
* The queries use the :doc:`sampledata` network.
* Boost: `Biconnected components <http://www.boost.org/libs/graph/doc/biconnected_components.html>`__
* wikipedia: `Biconnected component <http://en.wikipedia.org/wiki/Biconnected_component>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
