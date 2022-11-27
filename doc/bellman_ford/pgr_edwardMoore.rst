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
  (`3.5 <https://docs.pgrouting.org/3.5/en/pgr_edwardMoore.html>`__)
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_edwardMoore.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_edwardMoore.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_edwardMoore.html>`__

``pgr_edwardMoore - Experimental``
===============================================================================

``pgr_edwardMoore`` —  Returns the shortest path using Edward-Moore algorithm.


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

  * `start vid` ascending
  * `end vid` ascending

* Running time:

  * Worst case: :math:`O(| V | * | E |)`
  * Average case: :math:`O( | E | )`

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

   | RETURNS SET OF |old-generic-result|
   | OR EMPTY SET

.. index::
    single: edwardMoore(One to One) - Experimental on v3.0

One to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_edwardMoore(`Edges SQL`_, **start vid**, **end vid**, [``directed``])

   | RETURNS SET OF |result-1-1|
   | OR EMPTY SET

:Example: From vertex :math:`6` to vertex :math:`10` on a **directed** graph

.. literalinclude:: doc-pgr_edwardMoore.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: edwardMoore(One to Many) - Experimental on v3.0

One to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_edwardMoore(`Edges SQL`_, **start vid**, **end vids**, [``directed``])

   | RETURNS SET OF |result-1-m|
   | OR EMPTY SET

:Example: From vertex :math:`6` to vertices :math:`\{ 10, 17\}` on a
          **directed** graph

.. literalinclude:: doc-pgr_edwardMoore.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: edwardMoore(Many to One) - Experimental on v3.0

Many to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_edwardMoore(`Edges SQL`_, **start vids**, **end vid**, [``directed``])

   | RETURNS SET OF |result-m-1|
   | OR EMPTY SET

:Example: From vertices :math:`\{6, 1\}` to vertex :math:`17` on a **directed**
          graph

.. literalinclude:: doc-pgr_edwardMoore.queries
   :start-after: -- q4
   :end-before: -- q5

.. index::
    single: edwardMoore(Many to Many) - Experimental on v3.0

Many to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_edwardMoore(`Edges SQL`_, **start vids**, **end vids**, [``directed``])

   | RETURNS SET OF |short-generic-result|
   | OR EMPTY SET

:Example: From vertices :math:`\{6, 1\}` to vertices :math:`\{10, 17\}` on an
          **undirected** graph

.. literalinclude:: doc-pgr_edwardMoore.queries
   :start-after: -- q5
   :end-before: -- q51

.. index::
    single: edwardMoore(Combinations) - Experimental on v3.2

Combinations
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_edwardMoore(`Edges SQL`_, `Combinations SQL`_, [``directed``])

   | RETURNS SET OF |short-generic-result|
   | OR EMPTY SET

:Example: Using a combinations table on an **undirected** graph.

The combinations table:

.. literalinclude:: doc-pgr_edwardMoore.queries
   :start-after: -- q51
   :end-before: -- q52

The query:

.. literalinclude:: doc-pgr_edwardMoore.queries
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

Return columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_path_short_start
    :end-before: return_path_short_end

Additional Examples
-------------------------------------------------------------------------------

:Example 1: Demonstration of repeated values are ignored, and result is sorted.

.. literalinclude:: doc-pgr_edwardMoore.queries
    :start-after: -- q6
    :end-before: -- q7

:Example 2: Making **start vids** the same as **end vids**.

.. literalinclude:: doc-pgr_edwardMoore.queries
    :start-after: -- q7
    :end-before: -- q8

:Example 3: Manually assigned vertex combinations.

.. literalinclude:: doc-pgr_edwardMoore.queries
    :start-after: -- q8
    :end-before: -- q9

See Also
-------------------------------------------------------------------------------

* :doc:`sampledata`
* https://en.wikipedia.org/wiki/Shortest_Path_Faster_Algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
