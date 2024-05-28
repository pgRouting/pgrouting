..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


``pgr_strongComponents``
===============================================================================

``pgr_strongComponents`` — Strongly connected components of a directed graph
using Tarjan's algorithm based on DFS.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/strong_components.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.0.0

  * Result columns change:

    * ``n_seq`` is removed
    * ``seq`` changed type to ``BIGINT``

  * **Official** function

* Version 2.5.0

  * New **experimental** function


Description
-------------------------------------------------------------------------------

A strongly connected component of a directed graph is a set of vertices that are
all reachable from each other.

**The main characteristics are:**

- Works for **directed** graphs.
- Components are described by vertices identifiers.
- The returned values are ordered:

  - ``component`` ascending
  - ``node`` ascending

- Running time: :math:`O(V + E)`

.. index::
    single: strongComponents

Signatures
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_strongComponents(`Edges SQL`_)

   | Returns set of |result-component-V|
   | OR EMPTY SET

:Example: The strong components of the graph

.. literalinclude:: strongComponents.queries
   :start-after: -- q1
   :end-before: -- q2

.. figure:: /images/scc_sampledata.png
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


Result columns
-------------------------------------------------------------------------------

.. include:: pgr_connectedComponents.rst
    :start-after: return_componentsV_start
    :end-before: return_componentsV_end

See Also
-------------------------------------------------------------------------------

* :doc:`components-family`
* The queries use the :doc:`sampledata` network.
* Boost: `Strong components
  <https://www.boost.org/libs/graph/doc/strong_components.html>`__
* wikipedia: `Strongly connected component
  <https://en.wikipedia.org/wiki/Strongly_connected_component>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

