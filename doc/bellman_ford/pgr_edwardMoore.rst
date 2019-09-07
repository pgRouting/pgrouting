..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_edwardMoore - Experimental
===============================================================================

``pgr_edwardMoore`` —  Returns the shortest path(s) using Edward-Moore algorithm.
Edward Moore’s ​ Algorithm is an improvement of the Bellman-Ford Algorithm.


.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 3.0.0

  * New **experimental** function

Description
-------------------------------------------------------------------------------

Edward Moore’s Algorithm is an improvement of the Bellman-Ford Algorithm. It can compute
the shortest paths from a single source vertex to all other vertices in a weighted directed
graph. The main difference between Edward Moore's Algorithm and Bellman Ford's Algorithm lies in the run time.

The worst-case running time of the algorithm is :math:`O(| V | * | E |)` similar to the time complexity of
Bellman-Ford algorithm.
However, experiments suggest that this algorithm has an average running time complexity of
:math:`O( | E | )` for random graphs. This is significantly faster in terms of computation speed.

Thus, the algorithm is at-best, significantly faster than Bellman-Ford algorithm and is at-worst,
as good as Bellman-Ford algorithm

The main characteristics are:
  - Values are returned when there is a path.

    - When the starting vertex and ending vertex are the same, there is no path.

      - The `agg_cost` the non included values `(v, v)` is `0`

    - When the starting vertex and ending vertex are the different and there is no path:

      - The `agg_cost` the non included values `(u, v)` is :math:`\infty`

  - For optimization purposes, any duplicated value in the `start_vids` or `end_vids` are ignored.

  - The returned values are ordered:

    - `start_vid` ascending
    - `end_vid` ascending

  - Running time:
    - Worst case: :math:`O(| V | * | E |)`
    - Average case: :math:`O( | E | )`

Signatures
-------------------------------------------------------------------------------

.. code-block:: none

    pgr_edwardMoore(edges_sql, start_vid,  end_vid  [, directed])
    pgr_edwardMoore(edges_sql, start_vid,  end_vids [, directed])
    pgr_edwardMoore(edges_sql, start_vids, end_vid  [, directed])
    pgr_edwardMoore(edges_sql, start_vids, end_vids [, directed])
    RETURNS SET OF (seq, path_seq [, start_vid] [, end_vid], node, edge, cost, agg_cost)
    OR EMPTY SET

.. code-block:: none

    pgr_edwardMoore(TEXT edges_sql, BIGINT start_vid, BIGINT end_vid)
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost) or EMPTY SET

:Example: From vertex :math:`2` to vertex  :math:`3` on a **directed** graph

.. literalinclude:: doc-pgr_edwardMoore.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: edwardMoore(One to One) - Experimental

One to One
...............................................................................

.. code-block:: none

    pgr_edwardMoore(TEXT edges_sql, BIGINT start_vid, BIGINT end_vid,
    BOOLEAN directed:=true);
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertex  :math:`3` on an **undirected** graph

.. literalinclude:: doc-pgr_edwardMoore.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: edwardMoore(One to Many) - Experimental

One to many
...............................................................................

.. code-block:: none

    pgr_edwardMoore(TEXT edges_sql, BIGINT start_vid, ARRAY[ANY_INTEGER] end_vids,
    BOOLEAN directed:=true);
    RETURNS SET OF (seq, path_seq, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertices :math:`\{3, 5\}` on an **undirected** graph

.. literalinclude:: doc-pgr_edwardMoore.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: edwardMoore(Many to One) - Experimental

Many to One
...............................................................................

.. code-block:: none

    pgr_edwardMoore(TEXT edges_sql, ARRAY[ANY_INTEGER] start_vids, BIGINT end_vid,
        BOOLEAN directed:=true);
    RETURNS SET OF (seq, path_seq, start_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertices :math:`\{2, 11\}` to vertex :math:`5` on a **directed** graph

.. literalinclude:: doc-pgr_edwardMoore.queries
   :start-after: -- q4
   :end-before: -- q5

.. index::
    single: edwardMoore(Many to Many) - Experimental

Many to Many
...............................................................................

.. code-block:: none

    pgr_edwardMoore(TEXT edges_sql, ARRAY[ANY_INTEGER] start_vids, ARRAY[ANY_INTEGER] end_vids,
        BOOLEAN directed:=true);
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertices :math:`\{2, 11\}` to vertices :math:`\{3, 5\}` on an **undirected** graph

.. literalinclude:: doc-pgr_edwardMoore.queries
   :start-after: -- q5
   :end-before: -- q6

Parameters
-------------------------------------------------------------------------------

.. pgr_edwardMoore_parameters_start

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

.. pgr_edwardMoore_parameters_end

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


Example Application
-------------------------------------------------------------------------------

The examples of this section are based on the :doc:`sampledata` network.

The examples include combinations from starting vertices 2 and 11 to ending vertices 3 and 5 in a directed and
undirected graph with and with out reverse_cost.

:Examples: For queries marked as ``directed`` with ``cost`` and ``reverse_cost`` columns

The examples in this section use the following :ref:`fig1`

.. literalinclude:: doc-pgr_edwardMoore.queries
   :start-after: -- q7
   :end-before: -- q8

:Examples: For queries marked as ``undirected`` with ``cost`` and ``reverse_cost`` columns

The examples in this section use the following :ref:`fig2`

.. literalinclude:: doc-pgr_edwardMoore.queries
   :start-after: -- q9
   :end-before: -- q10

:Examples: For queries marked as ``directed`` with ``cost`` column

The examples in this section use the following :ref:`fig3`

.. literalinclude:: doc-pgr_edwardMoore.queries
   :start-after: -- q11
   :end-before: -- q12

:Examples: For queries marked as ``undirected`` with ``cost`` column

The examples in this section use the following :ref:`fig4`

.. literalinclude:: doc-pgr_edwardMoore.queries
   :start-after: -- q13
   :end-before: -- q14

See Also
-------------------------------------------------------------------------------

* https://en.wikipedia.org/wiki/Shortest_Path_Faster_Algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
