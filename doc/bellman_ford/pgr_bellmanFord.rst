..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_bellmanFord.html>`__
  (`3.3 <https://docs.pgrouting.org/3.3/en/pgr_bellmanFord.html>`__)
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_bellmanFord.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_bellmanFord.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_bellmanFord.html>`__

``pgr_bellmanFord - Experimental``
===============================================================================

``pgr_bellmanFord`` — Shortest path(s) using Bellman-Ford algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/bellman_ford_shortest.html

   Boost Graph Inside

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 3.2.0

  * New **experimental** signature:

    * ``pgr_bellmanFord`` (`Combinations`_)

* Version 3.0.0

  * New **experimental** signatures:
  
    * ``pgr_bellmanFord`` (`One to One`_)
    * ``pgr_bellmanFord`` (`One to Many`_)
    * ``pgr_bellmanFord`` (`Many to One`_)
    * ``pgr_bellmanFord`` (`Many to Many`_)

Description
-------------------------------------------------------------------------------

Bellman-Ford's algorithm, is named after Richard Bellman and Lester Ford, who 
first published it in 1958 and 1956, respectively.It is a graph search algorithm
that computes shortest paths from a starting vertex (``start_vid``) to an ending
vertex (``end_vid``) in a graph where some of the edge weights may be negative.
Though it is more versatile, it is slower than Dijkstra's algorithm.This 
implementation can be used with a directed graph and an undirected graph.

**The main characteristics are:**
  * Process is valid for edges with both positive and negative edge weights.
  * Values are returned when there is a path.

    * When the start vertex and the end vertex are the same, there is no path.
      The agg_cost would be :math:`0`.
    * When the start vertex and the end vertex are different, and there exists
      a path between them without having a *negative cycle*. The agg_cost would
      be some finite value denoting the shortest distance between them.
    * When the start vertex and the end vertex are different, and there exists
      a path between them, but it contains a *negative cycle*. In such case,
      agg_cost for those vertices keep on decreasing furthermore, Hence agg_cost
      can’t be defined for them.
    * When the start vertex and the end vertex are different, and there is no path.
      The agg_cost is :math:`\infty`.

  * For optimization purposes, any duplicated value in the `start_vids` or 
    `end_vids` are ignored.

  * The returned values are ordered:

    * `start_vid` ascending
    * `end_vid` ascending

  * Running time: :math:`O(| start\_vids | * ( V * E))`


Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. parsed-literal::

    pgr_bellmanFord(`Edges SQL`_, **start vid**, **end vid**  [, directed])
    pgr_bellmanFord(`Edges SQL`_, **start vid**, **end vids** [, directed])
    pgr_bellmanFord(`Edges SQL`_, **start vids**, **end vid**  [, directed])
    pgr_bellmanFord(`Edges SQL`_, **start vids**, **end vids** [, directed])
    pgr_bellmanFord(`Edges SQL`_, `Combinations SQL`_ [, directed])

    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)
    OR EMPTY SET

.. rubric:: Using defaults

.. parsed-literal::

    pgr_bellmanFord(`Edges SQL`_, **start vid**, **end vid**)
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertex :math:`3` on a **directed** graph

.. literalinclude:: doc-pgr_bellmanFord.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: bellman_ford(One to One) - Experimental on v3.0

One to One
...............................................................................

.. parsed-literal::

    pgr_bellmanFord(`Edges SQL`_, **start vid**,  **end vid**  [, directed])
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertex :math:`3` on an **undirected** graph

.. literalinclude:: doc-pgr_bellmanFord.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: bellmanFord(One to Many) - Experimental on v3.0

One to many
...............................................................................

.. parsed-literal::

    pgr_bellmanFord(`Edges SQL`_, **start vid**,  **end vids** [, directed])
    RETURNS SET OF (seq, path_seq, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertices :math:`\{ 3, 5\}` on an **undirected**
          graph

.. literalinclude:: doc-pgr_bellmanFord.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: bellman_ford(Many to One) - Experimental on v3.0

Many to One
...............................................................................

.. parsed-literal::

    pgr_bellmanFord(`Edges SQL`_, **start vids**, **end vid**  [, directed])
    RETURNS SET OF (seq, path_seq, start_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertices :math:`\{2, 11\}` to vertex :math:`5` on a **directed**
          graph

.. literalinclude:: doc-pgr_bellmanFord.queries
   :start-after: -- q4
   :end-before: -- q5

.. index::
    single: bellmanFord(Many to Many) - Experimental on v3.0

Many to Many
...............................................................................

.. parsed-literal::

    pgr_bellmanFord(`Edges SQL`_, **start vids**, **end vids** [, directed])
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertices :math:`\{2, 11\}` to vertices :math:`\{3, 5\}` on an 
          **directed** graph

.. literalinclude:: doc-pgr_bellmanFord.queries
   :start-after: -- q5
   :end-before: -- q51

.. index::
    single: bellmanFord(Combinations) - Experimental on v3.2

Combinations
...............................................................................

.. parsed-literal::

    pgr_bellmanFord(`Edges SQL`_, `Combinations SQL`_ [, directed])
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: Using a combinations table on an **directed** graph.

The combinations table:

.. literalinclude:: doc-pgr_bellmanFord.queries
   :start-after: -- q51
   :end-before: -- q6

The query:

.. literalinclude:: doc-pgr_bellmanFord.queries
   :start-after: -- q6
   :end-before: -- q7

Parameters
-------------------------------------------------------------------------------

.. include:: dijkstra-family.rst
    :start-after: dijkstra_parameters_start
    :end-before: dijkstra_parameters_end

Optional parameters
-------------------------------------------------------------------------------

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Combinations SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_combinations_sql_start
    :end-before: basic_combinations_sql_end

Results Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_path_short_start
    :end-before: return_path_short_end

See Also
-------------------------------------------------------------------------------

* https://en.wikipedia.org/wiki/Bellman%E2%80%93Ford_algorithm
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
