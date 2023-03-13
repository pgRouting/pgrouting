..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_binaryBreadthFirstSearch.html>`__
  (`3.6 <https://docs.pgrouting.org/3.6/en/pgr_binaryBreadthFirstSearch.html>`__)
  `3.5 <https://docs.pgrouting.org/3.5/en/pgr_binaryBreadthFirstSearch.html>`__
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_binaryBreadthFirstSearch.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_binaryBreadthFirstSearch.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_binaryBreadthFirstSearch.html>`__

``pgr_binaryBreadthFirstSearch`` - Experimental
===============================================================================

``pgr_binaryBreadthFirstSearch`` — Returns the shortest path(s) in a binary
graph.

Any graph whose edge-weights belongs to the set {0,X}, where 'X' is any
non-negative integer, is termed as a 'binary graph'.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/breadth_first_search.html

   Boost Graph Inside

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 3.2.0

  * New **experimental** signature:

    * pgr_binaryBreadthFirstSearch(`Combinations`_)

* Version 3.0.0

  * New **experimental** signatures:

    * pgr_binaryBreadthFirstSearch(`One to One`_)
    * pgr_binaryBreadthFirstSearch(`One to Many`_)
    * pgr_binaryBreadthFirstSearch(`Many to One`_)
    * pgr_binaryBreadthFirstSearch(`Many to Many`_)

Description
-------------------------------------------------------------------------------

It is well-known that the shortest paths between a single source and all other
vertices can be found using Breadth First Search in :math:`O(|E|)` in an
unweighted graph, i.e. the distance is the minimal number of edges that you
need to traverse from the source to another vertex. We can interpret such a
graph also as a weighted graph, where every edge has the weight :math:`1`.
If not alledges in graph have the same weight, that we need a more general
algorithm, like Dijkstra's Algorithm  which runs in :math:`O(|E|log|V|)` time.

However if the weights are more constrained, we can use a faster algorithm.
This algorithm, termed as 'Binary Breadth First Search' as well as '0-1 BFS',
is a variation of the standard Breadth First Search problem to solve the SSSP
(single-source shortest path) problem in :math:`O(|E|)`, if the weights of each
edge belongs to the set {0,X}, where 'X' is any non-negative real integer.


**The main Characteristics are:**

* Process is done only on 'binary graphs'. ('Binary Graph': Any graph whose
  edge-weights belongs to the set {0,X}, where 'X' is any non-negative real
  integer.)
* For optimization purposes, any duplicated value in the `start_vids` or
  `end_vids` are ignored.
* The returned values are ordered:

  * `start_vid` ascending
  * `end_vid` ascending

* Running time: :math:`O(| start\_vids | * |E|)`

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_binaryBreadthFirstSearch(`Edges SQL`_, **start vid**, **end vid**, [``directed``])
   | pgr_binaryBreadthFirstSearch(`Edges SQL`_, **start vid**, **end vids**, [``directed``])
   | pgr_binaryBreadthFirstSearch(`Edges SQL`_, **start vids**, **end vid**, [``directed``])
   | pgr_binaryBreadthFirstSearch(`Edges SQL`_, **start vids**, **end vids**, [``directed``])
   | pgr_binaryBreadthFirstSearch(`Edges SQL`_, `Combinations SQL`_, [``directed``])

   | RETURNS SET OF |old-generic-result|
   | OR EMPTY SET

**Note:** Using the :doc:`sampledata` Network as all weights are same (i.e
:math:`1``)

.. index::
    single: binaryBreadthFirstSearch(One to One) - Experimental on v3.0

One to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_binaryBreadthFirstSearch(`Edges SQL`_, **start vid**, **end vid**, [``directed``])

   | RETURNS SET OF |result-1-1|
   | OR EMPTY SET

:Example: From vertex :math:`6` to vertex  :math:`10` on a **directed** graph

.. literalinclude:: doc-pgr_binaryBreadthFirstSearch.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: binaryBreadthFirstSearch(One to Many) - Experimental on v3.0

One to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_binaryBreadthFirstSearch(`Edges SQL`_, **start vid**, **end vids**, [``directed``])

   | RETURNS SET OF |result-1-m|
   | OR EMPTY SET

:Example: From vertex :math:`6` to vertices :math:`\{10, 17\}` on a **directed**
          graph

.. literalinclude:: doc-pgr_binaryBreadthFirstSearch.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: binaryBreadthFirstSearch(Many to One) - Experimental on v3.0

Many to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_binaryBreadthFirstSearch(`Edges SQL`_, **start vids**, **end vid**, [``directed``])

   | RETURNS SET OF |result-m-1|
   | OR EMPTY SET

:Example: From vertices :math:`\{6, 1\}` to vertex :math:`17` on a **directed**
          graph

.. literalinclude:: doc-pgr_binaryBreadthFirstSearch.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: binaryBreadthFirstSearch(Many to Many) - Experimental on v3.0

Many to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_binaryBreadthFirstSearch(`Edges SQL`_, **start vids**, **end vids**, [``directed``])

   | RETURNS SET OF |short-generic-result|
   | OR EMPTY SET

:Example: From vertices :math:`\{6, 1\}` to vertices :math:`\{10, 17\}` on an
          **undirected** graph

.. literalinclude:: doc-pgr_binaryBreadthFirstSearch.queries
   :start-after: -- q4
   :end-before: -- q5

.. index::
    single: binaryBreadthFirstSearch(Combinations) - Experimental on v3.2

Combinations
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_binaryBreadthFirstSearch(`Edges SQL`_, `Combinations SQL`_, [``directed``])

   | RETURNS SET OF |short-generic-result|
   | OR EMPTY SET

:Example: Using a combinations table on an **undirected** graph

The combinations table:

.. literalinclude:: doc-pgr_binaryBreadthFirstSearch.queries
   :start-after: -- q5
   :end-before: -- q51

The query:

.. literalinclude:: doc-pgr_binaryBreadthFirstSearch.queries
   :start-after: -- q51
   :end-before: -- q6

Parameters
-------------------------------------------------------------------------------

.. include:: dijkstra-family.rst
    :start-after: dijkstra_parameters_start
    :end-before: dijkstra_parameters_end

Optional Parameters
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

Result Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_path_start
    :end-before: return_path_end


Additional Examples
-------------------------------------------------------------------------------

:Example: Manually assigned vertex combinations.

.. literalinclude:: doc-pgr_binaryBreadthFirstSearch.queries
    :start-after: -- q6
    :end-before: -- q7

See Also
-------------------------------------------------------------------------------
* :doc:`sampledata`
* https://cp-algorithms.com/graph/01_bfs.html
* https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm#Specialized_variants

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
