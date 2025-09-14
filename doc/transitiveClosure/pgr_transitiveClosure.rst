..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: Miscellaneous Algorithms ; pgr_transitiveClosure
   single: transitiveClosure - Experimental on v3.0

|

``pgr_transitiveClosure`` - Experimental
===============================================================================

``pgr_transitiveClosure`` — Transitive closure graph of a directed graph.

.. include:: experimental.rst
   :start-after: warning-begin
   :end-before: end-warning

.. rubric:: Availability

.. rubric:: Version 4.0.0

* Standardized output to |result-closure|

.. rubric:: Version 3.0.0

* New experimental function.


Description
-------------------------------------------------------------------------------

Transforms the input directed graph into the transitive closure of the graph.


The main characteristics are:

* Process is valid for directed graphs.

  * The transitive closure of an undirected graph produces a cluster graph
  * Reachability between vertices on an undirected graph happens when they
    belong to the same connected component. (see :doc:`pgr_connectedComponents`)

* The returned values are not ordered
* The returned graph is compressed
* Running time: :math:`O(|V||E|)`

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

The pgr_transitiveClosure function has the following signature:

.. admonition:: \ \
   :class: signatures

   | pgr_transitiveClosure(`Edges SQL`_)

   | Returns set of |result-closure|

:Example: Rechability of a subgraph

.. literalinclude:: transitiveClosure.queries
   :start-after: -- q1
   :end-before: -- q2

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

Returns set of |result-closure|

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``seq``
     - ``INTEGER``
     - Sequential value starting from :math:`1`
   * - ``vid``
     - ``BIGINT``
     - Identifier of the source of the edges
   * - ``target_array``
     - ``BIGINT``
     - Identifiers of the targets of the edges

       * Identifiers of the vertices that are reachable from vertex v.

See Also
-------------------------------------------------------------------------------

* :doc:`sampledata`
* `Boost: transitive closure <https://www.boost.org/libs/graph/doc/transitive_closure.html>`__
* https://en.wikipedia.org/wiki/Transitive_closure

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
