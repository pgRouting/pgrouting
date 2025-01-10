..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: Traversal Family ; pgr_binaryBreadthFirstSearch - Experimental
   single: Breadth First Search Category ; pgr_binaryBreadthFirstSearch - Experimental
   single: binaryBreadthFirstSearch - Experimental

|

``pgr_binaryBreadthFirstSearch`` - Experimental
===============================================================================

``pgr_binaryBreadthFirstSearch`` — Returns the shortest path in a binary
graph.

Any graph whose edge-weights belongs to the set {0,X}, where 'X' is any
non-negative integer, is termed as a 'binary graph'.

.. include:: experimental.rst
   :start-after: warning-begin
   :end-before: end-warning

.. rubric:: Availability

* Version 3.2.0

  * New experimental signature:

    * pgr_binaryBreadthFirstSearch(Combinations)

* Version 3.0.0

  * New experimental function.

Description
-------------------------------------------------------------------------------

It is well-known that the shortest paths between a single source and all other
vertices can be found using Breadth First Search in :math:`O(|E|)` in an
unweighted graph, i.e. the distance is the minimal number of edges that you
need to traverse from the source to another vertex. We can interpret such a
graph also as a weighted graph, where every edge has the weight :math:`1`.
If not alledges in graph have the same weight, that we need a more general
algorithm, like Dijkstra's Algorithm which runs in :math:`O(|E|log|V|)` time.

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

|Boost| Boost Graph Inside

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

   | Returns set of |old-generic-result|
   | OR EMPTY SET

**Note:** Using the :doc:`sampledata` Network as all weights are same (i.e
:math:`1``)

.. index::
    single: binaryBreadthFirstSearch - Experimental ; One to One - Experimental on v3.0

One to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_binaryBreadthFirstSearch(`Edges SQL`_, **start vid**, **end vid**, [``directed``])

   | Returns set of |result-1-1|
   | OR EMPTY SET

:Example: From vertex :math:`6` to vertex :math:`10` on a **directed** graph

.. literalinclude:: binaryBreadthFirstSearch.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: binaryBreadthFirstSearch - Experimental ; One to Many - Experimental on v3.0

One to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_binaryBreadthFirstSearch(`Edges SQL`_, **start vid**, **end vids**, [``directed``])

   | Returns set of |result-1-m|
   | OR EMPTY SET

:Example: From vertex :math:`6` to vertices :math:`\{10, 17\}` on a **directed**
          graph

.. literalinclude:: binaryBreadthFirstSearch.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: binaryBreadthFirstSearch - Experimental ; Many to One - Experimental on v3.0

Many to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_binaryBreadthFirstSearch(`Edges SQL`_, **start vids**, **end vid**, [``directed``])

   | Returns set of |result-m-1|
   | OR EMPTY SET

:Example: From vertices :math:`\{6, 1\}` to vertex :math:`17` on a **directed**
          graph

.. literalinclude:: binaryBreadthFirstSearch.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: binaryBreadthFirstSearch - Experimental ; Many to Many - Experimental on v3.0

Many to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_binaryBreadthFirstSearch(`Edges SQL`_, **start vids**, **end vids**, [``directed``])

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: From vertices :math:`\{6, 1\}` to vertices :math:`\{10, 17\}` on an
          **undirected** graph

.. literalinclude:: binaryBreadthFirstSearch.queries
   :start-after: -- q4
   :end-before: -- q5

.. index::
    single: binaryBreadthFirstSearch - Experimental ; Combinations - Experimental on v3.2

Combinations
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_binaryBreadthFirstSearch(`Edges SQL`_, `Combinations SQL`_, [``directed``])

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: Using a combinations table on an **undirected** graph

The combinations table:

.. literalinclude:: binaryBreadthFirstSearch.queries
   :start-after: -- q5
   :end-before: -- q51

The query:

.. literalinclude:: binaryBreadthFirstSearch.queries
   :start-after: -- q51
   :end-before: -- q6

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

Result columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_path_start
    :end-before: return_path_end


Additional Examples
-------------------------------------------------------------------------------

:Example: Manually assigned vertex combinations.

.. literalinclude:: binaryBreadthFirstSearch.queries
    :start-after: -- q6
    :end-before: -- q7

See Also
-------------------------------------------------------------------------------
* :doc:`sampledata`
* `Boost: Breadth First Search
  <https://www.boost.org/libs/graph/doc/breadth_first_search.html>`__
* https://cp-algorithms.com/graph/01_bfs.html
* https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm#Specialized_variants

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
