..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_connectedComponents - Proposed
===============================================================================

``pgr_connectedComponents`` — Connected components of an undirected graph using a DFS-based approach.

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/connected_components.html

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

A connected component of an undirected graph is a set of vertices that are all reachable
from each other.

**The main characteristics are:**

- The signature is for an **undirected** graph.
- Components are described by vertices

- The returned values are ordered:

  - `component` ascending
  - `node` ascending

- Running time: :math:`O(V + E)`

Signatures
-------------------------------------------------------------------------------

.. index::
    single: connectedComponents -- Proposed

.. code-block:: none

    pgr_connectedComponents(edges_sql)

    RETURNS SET OF (seq, component, node)
    OR EMPTY SET

:Example: The connected components of the graph

.. literalinclude:: doc-pgr_connectedComponents.queries
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

.. return_componentsV_start

Returns set of ``(seq, component, node)``

============== ========== =================================================
Column         Type       Description
============== ========== =================================================
**seq**        ``INT``    Sequential value starting from **1**.
**component**  ``BIGINT`` Component identifier. It is equal to the minimum node identifier in the component.
**node**       ``BIGINT`` Identifier of the vertex that belongs to **component**.
============== ========== =================================================

.. return_componentsV_end

See Also
-------------------------------------------------------------------------------

* :doc:`components-family`
* The queries use the :doc:`sampledata` network.
* Boost: `Connected components <http://www.boost.org/libs/graph/doc/connected_components.html>`__
* wikipedia: `Connected component <http://en.wikipedia.org/wiki/Connected_component_(graph_theory)>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

