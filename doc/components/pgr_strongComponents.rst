..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_strongComponents
===============================================================================

``pgr_strongComponents`` — Strongly connected components of a directed graph using Tarjan's algorithm based on DFS.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/strong_components.html

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
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_strongComponents.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_strongComponents.html>`__

* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_strongComponents.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_strongComponents.html>`__

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
    single: strongComponents

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
* Boost: `Strong components <https://www.boost.org/libs/graph/doc/strong_components.html>`__
* wikipedia: `Strongly connected component <https://en.wikipedia.org/wiki/Strongly_connected_component>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

