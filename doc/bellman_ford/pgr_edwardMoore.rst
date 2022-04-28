..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_edwardMoore.html>`__
  (`3.3 <https://docs.pgrouting.org/3.3/en/pgr_edwardMoore.html>`__)
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_edwardMoore.html>`__

``pgr_edwardMoore - Experimental``
===============================================================================

``pgr_edwardMoore`` —  Returns the shortest path(s) using Edward-Moore algorithm.
Edward-Moore algorithm is an improvement of the Bellman-Ford Algorithm.


.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 3.2.0

  * New **experimental** signature:

    * ``pgr_edwardMoore`` (`Combinations`_)

* Version 3.0.0

  * New **experimental** signatures:

    * ``pgr_edwardMoore`` (`One to One`_)
    * ``pgr_edwardMoore`` (`One to Many`_)
    * ``pgr_edwardMoore`` (`Many to One`_)
    * ``pgr_edwardMoore`` (`Many to Many`_)

Description
-------------------------------------------------------------------------------

Edward Moore’s Algorithm is an improvement of the Bellman-Ford Algorithm. It 
can compute the shortest paths from a single source vertex to all other vertices
in a weighted directed graph. The main difference between Edward Moore's Algorithm
and Bellman Ford's Algorithm lies in the run time.

The worst-case running time of the algorithm is :math:`O(| V | * | E |)` similar
to the time complexity of Bellman-Ford algorithm.
However, experiments suggest that this algorithm has an average running time 
complexity of :math:`O( | E | )` for random graphs. This is significantly faster
in terms of computation speed.

Thus, the algorithm is at-best, significantly faster than Bellman-Ford algorithm 
and is at-worst,as good as Bellman-Ford algorithm

**The main characteristics are:**
  * Values are returned when there is a path.

    * When the starting vertex and ending vertex are the same, there is no path.

      * The `agg_cost` the non included values `(v, v)` is :math:`0`

    * When the starting vertex and ending vertex are the different and there is 
      no path:

      * The `agg_cost` the non included values `(u, v)` is :math:`\infty`

  * For optimization purposes, any duplicated value in the `start vids` or `end vids`
    are ignored.

  * The returned values are ordered:

    * `start vid` ascending
    * `end vid` ascending

  * Running time:
    * Worst case: :math:`O(| V | * | E |)`
    * Average case: :math:`O( | E | )`

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. parsed-literal:: 

    pgr_edwardMoore(`Edges SQL`_, **start vid**,  **end vid**  [, directed])
    pgr_edwardMoore(`Edges SQL`_, **start vid**,  **end vids** [, directed])
    pgr_edwardMoore(`Edges SQL`_, **start vids**, **end vid**  [, directed])
    pgr_edwardMoore(`Edges SQL`_, **start vids**, **end vids** [, directed])
    pgr_edwardMoore(`Edges SQL`_, `Combinations SQL`_ [, directed])
    RETURNS SET OF (seq, path_seq [, start_vid] [, end_vid], node, edge, cost, agg_cost)
    OR EMPTY SET

.. index::
    single: edwardMoore(One to One) - Experimental on v3.0

One to One
...............................................................................

.. parsed-literal::

    pgr_edwardMoore(`Edges SQL`_, **start vid**, **end vid** [, directed]);
    RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertex  :math:`3` on an **undirected** graph

.. literalinclude:: doc-pgr_edwardMoore.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: edwardMoore(One to Many) - Experimental on v3.0

One to Many
...............................................................................

.. parsed-literal::

    pgr_edwardMoore(`Edges SQL`_, **start vid**, **end vids** [, directed]);
    RETURNS SET OF (seq, path_seq, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertices :math:`\{3, 5\}` on an **undirected** graph

.. literalinclude:: doc-pgr_edwardMoore.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: edwardMoore(Many to One) - Experimental on v3.0

Many to One
...............................................................................

.. parsed-literal::

    pgr_edwardMoore(`Edges SQL`_, **start vids**, **end vid** [, directed]);
    RETURNS SET OF (seq, path_seq, start_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertices :math:`\{2, 11\}` to vertex :math:`5` on a **directed** graph

.. literalinclude:: doc-pgr_edwardMoore.queries
   :start-after: -- q4
   :end-before: -- q5

.. index::
    single: edwardMoore(Many to Many) - Experimental on v3.0

Many to Many
...............................................................................

.. parsed-literal::

    pgr_edwardMoore(`Edges SQL`_, **start vids**, **end vids** [, directed]);
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertices :math:`\{2, 11\}` to vertices :math:`\{3, 5\}` on an **undirected** graph

.. literalinclude:: doc-pgr_edwardMoore.queries
   :start-after: -- q5
   :end-before: -- q51

.. index::
    single: edwardMoore(Combinations) - Experimental on v3.2

Combinations
...............................................................................

.. parsed-literal::

    pgr_edwardMoore(`Edges SQL`_, `Combinations SQL`_ [, directed]);
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

The combinations table:

.. literalinclude:: doc-pgr_edwardMoore.queries
   :start-after: -- q51
   :end-before: -- q6

:Example: Using the combinations table on an **undirected** graph.

.. literalinclude:: doc-pgr_edwardMoore.queries
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

Inner queries
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

Return Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_path_start
    :end-before: return_path_end


Additional Examples
-------------------------------------------------------------------------------

The examples of this section are based on the :doc:`sampledata` network.
The examples include combinations from starting vertices 2 and 11 to ending 
vertices 3 and 5 in a directed and undirected graph with and with out reverse_cost.

**Examples:** For queries marked as ``directed`` with ``cost`` and ``reverse_cost`` columns

The examples in this section use the following :ref:`fig1`

.. literalinclude:: doc-pgr_edwardMoore.queries
   :start-after: -- q8
   :end-before: -- q9

**Examples:** For queries marked as ``undirected`` with ``cost`` and ``reverse_cost`` columns

The examples in this section use the following :ref:`fig2`

.. literalinclude:: doc-pgr_edwardMoore.queries
   :start-after: -- q10
   :end-before: -- q11

**Examples:** For queries marked as ``directed`` with ``cost`` column

The examples in this section use the following :ref:`fig3`

.. literalinclude:: doc-pgr_edwardMoore.queries
   :start-after: -- q12
   :end-before: -- q13

**Examples:** For queries marked as ``undirected`` with ``cost`` column

The examples in this section use the following :ref:`fig4`

.. literalinclude:: doc-pgr_edwardMoore.queries
   :start-after: -- q14
   :end-before: -- q15

See Also
-------------------------------------------------------------------------------

* :doc:`sampledata`
* https://en.wikipedia.org/wiki/Shortest_Path_Faster_Algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
