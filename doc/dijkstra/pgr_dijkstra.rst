..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_dijkstra
===============================================================================

``pgr_dijkstra`` — Returns the shortest path(s) using Dijkstra algorithm.
In particular, the Dijkstra algorithm implemented by Boost.Graph.

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/dijkstra_shortest_paths.html

   Boost Graph Inside

.. rubric:: Availability

* New on v2.1.0:

  * pgr_dijkstra(One to Many)
  * pgr_dijkstra(Many to One)
  * pgr_dijkstra(Many to Many)

* Signature change on v2.1.0:

  * pgr_dijkstra(One to One)

* New on v2.0.0:

  * pgr_dijkstra(One to One)

Description
-------------------------------------------------------------------------------

Dijkstra's algorithm, conceived by Dutch computer scientist Edsger Dijkstra in 1956.
It is a graph search algorithm that solves the shortest path problem for
a graph with non-negative edge path costs, producing a shortest path from
a starting vertex (``start_vid``) to an ending vertex (``end_vid``).
This implementation can be used with a directed graph and an undirected graph.

The main characteristics are:
  - Process is done only on edges with positive costs.
  - Values are returned when there is a path.

    - When the starting vertex and ending vertex are the same, there is no path.

      - The `agg_cost` the non included values `(v, v)` is `0`

    - When the starting vertex and ending vertex are the different and there is no path:

      - The `agg_cost` the non included values `(u, v)` is :math:`\infty`

  - For optimization purposes, any duplicated value in the `start_vids` or `end_vids` are ignored.

  - The returned values are ordered:

    - `start_vid` ascending
    - `end_vid` ascending

  - Running time: :math:`O(| start\_vids | * (V \log V + E))`

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_dijkstra(edges_sql, start_vid,  end_vid  [, directed])
    pgr_dijkstra(edges_sql, start_vid,  end_vids [, directed])
    pgr_dijkstra(edges_sql, start_vids, end_vid  [, directed])
    pgr_dijkstra(edges_sql, start_vids, end_vids [, directed])
    RETURNS SET OF (seq, path_seq [, start_vid] [, end_vid], node, edge, cost, agg_cost)
    OR EMPTY SET

.. rubric:: Using defaults

.. code-block:: none

    pgr_dijkstra(TEXT edges_sql, BIGINT start_vid, BIGINT end_vid)
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost) or EMPTY SET

:Example: From vertex :math:`2` to vertex  :math:`3` on a **directed** graph

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: dijkstra(One to One)

One to One
...............................................................................

.. code-block:: none

    pgr_dijkstra(TEXT edges_sql, BIGINT start_vid, BIGINT end_vid,
    BOOLEAN directed:=true);
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertex  :math:`3` on an **undirected** graph

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: dijkstra(One to Many)

One to many
...............................................................................

.. code-block:: none

    pgr_dijkstra(TEXT edges_sql, BIGINT start_vid, ARRAY[ANY_INTEGER] end_vids,
    BOOLEAN directed:=true);
    RETURNS SET OF (seq, path_seq, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertices :math:`\{3, 5\}` on an **undirected** graph

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: dijkstra(Many to One)

Many to One
...............................................................................

.. code-block:: none

    pgr_dijkstra(TEXT edges_sql, ARRAY[ANY_INTEGER] start_vids, BIGINT end_vid,
        BOOLEAN directed:=true);
    RETURNS SET OF (seq, path_seq, start_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertices :math:`\{2, 11\}` to vertex :math:`5` on a **directed** graph

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q4
   :end-before: -- q5

.. index::
    single: dijkstra(Many to Many)

Many to Many
...............................................................................

.. code-block:: none

    pgr_dijkstra(TEXT edges_sql, ARRAY[ANY_INTEGER] start_vids, ARRAY[ANY_INTEGER] end_vids,
        BOOLEAN directed:=true);
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertices :math:`\{2, 11\}` to vertices :math:`\{3, 5\}` on an **undirected** graph

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q5
   :end-before: -- q6

Parameters
-------------------------------------------------------------------------------

.. pgr_dijkstra_parameters_start

============== ================== ======== =================================================
Parameter      Type               Default     Description
============== ================== ======== =================================================
**edges_sql**  ``TEXT``                    Inner SQL query as described below.
**start_vid**  ``BIGINT``                  Identifier of the starting vertex of the path.
**start_vids** ``ARRAY[BIGINT]``           Array of identifiers of starting vertices.
**end_vid**    ``BIGINT``                  Identifier of the ending vertex of the path.
**end_vids**   ``ARRAY[BIGINT]``           Array of identifiers of ending vertices.
**directed**   ``BOOLEAN``        ``true`` - When ``true`` Graph is considered `Directed`
                                           - When ``false`` the graph is considered as `Undirected`.
============== ================== ======== =================================================

.. pgr_dijkstra_parameters_end

Inner query
-------------------------------------------------------------------------------

.. rubric::edges_sql

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Return Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_path_start
    :end-before: return_path_end


Additional Examples
-------------------------------------------------------------------------------

The examples of this section are based on the :doc:`sampledata` network.

The examples include combinations from starting vertices 2 and 11 to ending vertices 3 and 5 in a directed and
undirected graph with and with out reverse_cost.

:Examples: For queries marked as ``directed`` with ``cost`` and ``reverse_cost`` columns

The examples in this section use the following :ref:`fig1`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q7
   :end-before: -- q8

:Examples: For queries marked as ``undirected`` with ``cost`` and ``reverse_cost`` columns

The examples in this section use the following :ref:`fig2`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q9
   :end-before: -- q10

:Examples: For queries marked as ``directed`` with ``cost`` column

The examples in this section use the following :ref:`fig3`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q11
   :end-before: -- q12

:Examples: For queries marked as ``undirected`` with ``cost`` column

The examples in this section use the following :ref:`fig4`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q13
   :end-before: -- q14

Equvalences between signatures
...............................................................................

:Examples: For queries marked as ``directed`` with ``cost`` and ``reverse_cost`` columns

The examples in this section use the following:

* :ref:`fig1`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q15
   :end-before: -- q16

:Examples: For queries marked as ``undirected`` with ``cost`` and ``reverse_cost`` columns

The examples in this section use the following:

* :ref:`fig2`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q17
   :end-before: -- q18

See Also
-------------------------------------------------------------------------------

* http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

