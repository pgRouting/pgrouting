..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


``pgr_transitiveClosure`` - Experimental
===============================================================================

``pgr_transitiveClosure`` — Transitive closure graph of a directed graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/transitive_closure.html

   Boost Graph Inside

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 3.0.0

  * New **experimental** function


Description
-------------------------------------------------------------------------------

Transforms the input directed graph into the transitive closure of the graph.


The main characteristics are:

* Process is valid for directed graphs.

  * The transitive closure of an undirected graph produces a cluster graph
  * Reachability between vertices on an undirected graph happens when they
    belong to the same connected component. (see :doc:`pgr_connectedComponents`)

* The returned values are not ordered
* The returned graph is compresed
* Running time: :math:`O(|V||E|)`


Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

The pgr_transitiveClosure function has the following signature:

.. index::
   single: transitiveClosure - Experimental on v3.0

.. admonition:: \ \
   :class: signatures

   | pgr_transitiveClosure(`Edges SQL`_)

   | RETURNS SET OF |result-closure|

:Example: Rechability of a subgraph

.. literalinclude:: doc-transitiveClosure.queries
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

Result Columns
-------------------------------------------------------------------------------

RETURNS SET OF |result-closure|

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
* https://en.wikipedia.org/wiki/Transitive_closure

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
