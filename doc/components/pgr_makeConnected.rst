..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


``pgr_makeConnected`` - Experimental
===============================================================================

``pgr_makeConnected`` — Set of edges that will connect the graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/make_connected.html

   Boost Graph Inside

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 3.2.0

  * New **experimental** function


Description
-------------------------------------------------------------------------------

Adds the minimum number of edges needed to make the input graph connected. The
algorithm first identifies
all of the connected components in the graph, then adds edges to connect those
components together in a path.
For example, if a graph contains three connected components A, B, and C,
make_connected will add two edges.
The two edges added might consist of one connecting a vertex in A with a vertex
in B and one connecting a vertex in B with a vertex in C.

The main characteristics are:

- Works for **undirected** graphs.
- It will give a minimum list of all edges which are needed in the graph to
  make connect it.
- The algorithm does not considers traversal costs in the calculations.
- The algorithm does not considers geometric topology in the calculations.
- Running time: :math:`O(V + E)`


.. index::
    single: makeConnected - Experimental on v3.2

Signatures
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_makeConnected(`Edges SQL`_)

   | RETURNS SET OF |result-component-make|
   | OR EMPTY SET

:Example: Query done on :doc:`sampledata` network gives the list of edges that
          are needed to connect the graph.

.. literalinclude:: doc-pgr_makeConnected.queries
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

Returns set of |result-component-make|

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
   * - ``start_vid``
     - ``BIGINT``
     - Identifier of the first end point vertex of the edge.
   * - ``end_vid``
     - ``BIGINT``
     - Identifier of the second end point vertex of the edge.

See Also
-------------------------------------------------------------------------------

* https://www.boost.org/libs/graph/doc/make_connected.html
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
