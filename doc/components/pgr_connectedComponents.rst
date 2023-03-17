..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


pgr_connectedComponents
===============================================================================

``pgr_connectedComponents`` — Connected components of an undirected graph using
a DFS-based approach.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/connected_components.html

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

A connected component of an undirected graph is a set of vertices that are all
reachable
from each other.

**The main characteristics are:**

- Works for **undirected** graphs.
- Components are described by vertices

- The returned values are ordered:

  - ``component`` ascending
  - ``node`` ascending

- Running time: :math:`O(V + E)`

.. index::
    single: connectedComponents

Signatures
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_connectedComponents(`Edges SQL`_)

   | RETURNS SET OF |result-component-V|
   | OR EMPTY SET

:Example: The connected components of the graph

.. literalinclude:: doc-pgr_connectedComponents.queries
   :start-after: -- q1
   :end-before: -- q2

.. figure:: /images/cc_sampledata.png
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

.. return_componentsV_start

Returns set of |result-component-V|

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

       * Has the value of the minimum node identifier in the component.
   * - ``node``
     - ``BIGINT``
     - Identifier of the vertex that belongs to the ``component``.

.. return_componentsV_end

Additional Examples
-------------------------------------------------------------------------------

Connecting disconnected components
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: connecting_graph_start
    :end-before: connecting_graph_end

See Also
-------------------------------------------------------------------------------

* :doc:`components-family`
* The queries use the :doc:`sampledata` network.
* Boost: `Connected components
  <https://www.boost.org/libs/graph/doc/connected_components.html>`__
* wikipedia: `Connected component
  <https://en.wikipedia.org/wiki/Connected_component_(graph_theory)>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

