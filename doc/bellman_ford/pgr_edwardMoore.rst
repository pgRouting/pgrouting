..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: Shortest Path Category; pgr_edwardMoore - Experimental
   single: edwardMoore - Experimental

|

``pgr_edwardMoore`` - Experimental
===============================================================================

``pgr_edwardMoore`` — Returns the shortest path using Edward-Moore algorithm.


.. include:: experimental.rst
   :start-after: warning-begin
   :end-before: end-warning

.. rubric:: Availability

.. rubric:: Version 4.0.0

* Output columns standardized to |short-generic-result|

.. rubric:: Version 3.2.0

* New experimental signature:

  * pgr_edwardMoore(Combinations)

.. rubric:: Version 3.0.0

* New experimental function.

Description
-------------------------------------------------------------------------------

Edward Moore’s Algorithm is an improvement of the Bellman-Ford Algorithm.
It can compute the shortest paths from a single source vertex to all other
vertices in a weighted directed graph.
The main difference between Edward Moore's Algorithm and Bellman Ford's
Algorithm lies in the run time.

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

* For optimization purposes, any duplicated value in the `start vids` or `end
  vids` are ignored.

* The returned values are ordered:

  * `start_vid` ascending
  * `end_vid` ascending

* Running time:

  * Worst case: :math:`O(| V | * | E |)`
  * Average case: :math:`O( | E | )`

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_edwardMoore(`Edges SQL`_, **start vid**, **end vid**, [``directed``])
   | pgr_edwardMoore(`Edges SQL`_, **start vid**, **end vids**, [``directed``])
   | pgr_edwardMoore(`Edges SQL`_, **start vids**, **end vid**, [``directed``])
   | pgr_edwardMoore(`Edges SQL`_, **start vids**, **end vids**, [``directed``])
   | pgr_edwardMoore(`Edges SQL`_, `Combinations SQL`_, [``directed``])

   | Returns set of |short-generic-result|
   | OR EMPTY SET

.. index::
    single: edwardMoore - Experimental ; One to One - Experimental on v3.0

One to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_edwardMoore(`Edges SQL`_, **start vid**, **end vid**, [``directed``])

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: From vertex :math:`6` to vertex :math:`10` on a **directed** graph

.. literalinclude:: edwardMoore.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: edwardMoore - Experimental ; One to Many - Experimental on v3.0

One to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_edwardMoore(`Edges SQL`_, **start vid**, **end vids**, [``directed``])

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: From vertex :math:`6` to vertices :math:`\{10, 17\}` on a **directed**
          graph

.. literalinclude:: edwardMoore.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: edwardMoore - Experimental ; Many to One - Experimental on v3.0

Many to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_edwardMoore(`Edges SQL`_, **start vids**, **end vid**, [``directed``])

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: From vertices :math:`\{6, 1\}` to vertex :math:`17` on a **directed**
          graph

.. literalinclude:: edwardMoore.queries
   :start-after: -- q4
   :end-before: -- q5

.. index::
    single: edwardMoore - Experimental ; Many to Many - Experimental on v3.0

Many to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_edwardMoore(`Edges SQL`_, **start vids**, **end vids**, [``directed``])

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: From vertices :math:`\{6, 1\}` to vertices :math:`\{10, 17\}` on an
          **undirected** graph

.. literalinclude:: edwardMoore.queries
   :start-after: -- q5
   :end-before: -- q51

.. index::
    single: edwardMoore - Experimental ; Combinations - Experimental on v3.2

Combinations
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_edwardMoore(`Edges SQL`_, `Combinations SQL`_, [``directed``])

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: Using a combinations table on an **undirected** graph.

The combinations table:

.. literalinclude:: edwardMoore.queries
   :start-after: -- q51
   :end-before: -- q52

The query:

.. literalinclude:: edwardMoore.queries
   :start-after: -- q52
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
    :start-after: return_path_complete_start
    :end-before: return_path_complete_end

Additional Examples
-------------------------------------------------------------------------------

:Example 1: Demonstration of repeated values are ignored, and result is sorted.

.. literalinclude:: edwardMoore.queries
    :start-after: -- q6
    :end-before: -- q7

:Example 2: Making **start vids** the same as **end vids**.

.. literalinclude:: edwardMoore.queries
    :start-after: -- q7
    :end-before: -- q8

:Example 3: Manually assigned vertex combinations.

.. literalinclude:: edwardMoore.queries
    :start-after: -- q8
    :end-before: -- q9

See Also
-------------------------------------------------------------------------------

* :doc:`sampledata`
* https://en.wikipedia.org/wiki/Shortest_Path_Faster_Algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
