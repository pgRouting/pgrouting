..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_strongComponents - Proposed
===============================================================================

``pgr_strongComponents`` — Strongly connected components of a directed graph using Tarjan's algorithm based on DFS.

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/strong_components.html

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

A strongly connected component of a directed graph is a set of vertices that are all reachable
from each other.

**The main characteristics are:**

- The signature is for a **directed** graph.
- Components are described by vertices
- The returned values are ordered:

  - `component` ascending
  - `node` ascending

- Running time: :math:`O(V + E)`

Signatures
-------------------------------------------------------------------------------

.. index::
    single: strongComponents -- Proposed

.. code-block:: none

    pgr_strongComponents(Edges SQL)

    RETURNS SET OF (seq, component, node)
    OR EMPTY SET

:Example: The strong components of the graph

.. literalinclude:: doc-pgr_strongComponents.queries
   :start-after: -- q1
   :end-before: -- q2

Parameters
-------------------------------------------------------------------------------


.. include:: components-family.rst
    :start-after: components_parameters_start
    :end-before: components_parameters_end

Inner query
-------------------------------------------------------------------------------

:edges SQL: an SQL query of a **directed** graph, which should return a set of rows with the following columns:

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end


Result Columns
-------------------------------------------------------------------------------

.. include:: pgr_connectedComponents.rst
    :start-after: return_componentsV_start
    :end-before: return_componentsV_end

See Also
-------------------------------------------------------------------------------

* :doc:`components-family`
* The queries use the :doc:`sampledata` network.
* Boost: `Strong components <http://www.boost.org/libs/graph/doc/strong_components.html>`__
* wikipedia: `Strongly connected component <http://en.wikipedia.org/wiki/Strongly_connected_component>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

