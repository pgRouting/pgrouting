..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_biconnectedComponents
===============================================================================

``pgr_biconnectedComponents`` — Return the biconnected components of an undirected graph.
In particular, the algorithm implemented by Boost.Graph.

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

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_biconnectedComponents.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_biconnectedComponents.html>`__

* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_biconnectedComponents.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_biconnectedComponents.html>`__

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
    single: biconnectedComponents

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
**seq**        ``BIGINT``    Sequential value starting from **1**.
**component**  ``BIGINT`` Component identifier. It is equal to the minimum edge identifier in the component.
**edge**       ``BIGINT`` Identifier of the edge.
============== ========== =================================================

.. return_componentsE_end

See Also
-------------------------------------------------------------------------------

* :doc:`components-family`
* The queries use the :doc:`sampledata` network.
* Boost: `Biconnected components <https://www.boost.org/libs/graph/doc/biconnected_components.html>`__
* wikipedia: `Biconnected component <https://en.wikipedia.org/wiki/Biconnected_component>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
