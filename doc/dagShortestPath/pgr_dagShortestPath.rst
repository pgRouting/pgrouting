..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: Shortest Path Category ; pgr_dagShortestPath - Experimental
   single: Directed Acyclic Graph Category ; pgr_dagShortestPath - Experimental
   single: dagShortestPath - Experimental

|

``pgr_dagShortestPath`` - Experimental
===============================================================================

``pgr_dagShortestPath`` — Returns the shortest path for weighted directed
acyclic graphs(DAG).
In particular, the DAG shortest paths algorithm implemented by Boost.Graph.

.. include:: experimental.rst
   :start-after: warning-begin
   :end-before: end-warning

.. rubric:: Availability

.. rubric:: Version 4.0.0

* Output columns standardized to |short-generic-result|

.. rubric:: Version 3.2.0

* New experimental function.

  * pgr_dagShortestPath(Combinations)

..rubric:: Version 3.0.0

* New experimental function.


Description
-------------------------------------------------------------------------------

Shortest Path for Directed Acyclic Graph(DAG) is a graph search algorithm that
solves the shortest path problem for weighted directed acyclic graph, producing
a shortest path from a starting vertex (``start_vid``) to an ending vertex
(``end_vid``).

This implementation can only be used with a **directed** graph with no cycles
i.e. directed acyclic graph.

The algorithm relies on topological sorting the dag to impose a linear ordering
on the vertices, and thus is more efficient for DAG's than either the Dijkstra
or Bellman-Ford algorithm.

The main characteristics are:
  - Process is valid for weighted directed acyclic graphs only. otherwise it
    will throw warnings.
  - Values are returned when there is a path.

    - When the starting vertex and ending vertex are the same, there is no path.

      - The `agg_cost` the non included values `(v, v)` is `0`

    - When the starting vertex and ending vertex are the different and there is
      no path:

      - The `agg_cost` the non included values `(u, v)` is :math:`\infty`

  - For optimization purposes, any duplicated value in the `start_vids` or
    `end_vids` are ignored.

  - The returned values are ordered:

    - `start_vid` ascending
    - `end_vid` ascending

  * Running time: :math:`O(| start\_vids | * (V + E))`

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_dagShortestPath(`Edges SQL`_, **start vid**, **end vid**)
   | pgr_dagShortestPath(`Edges SQL`_, **start vid**, **end vids**)
   | pgr_dagShortestPath(`Edges SQL`_, **start vids**, **end vid**)
   | pgr_dagShortestPath(`Edges SQL`_, **start vids**, **end vids**)
   | pgr_dagShortestPath(`Edges SQL`_, `Combinations SQL`_)

   | Returns set of |short-generic-result|
   | OR EMPTY SET


.. index::
    single: dagShortestPath - Experimental ; One to One - Experimental on v3.0

One to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_dagShortestPath(`Edges SQL`_, **start vid**, **end vid**)

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: From vertex :math:`5` to vertex :math:`11` on a **directed** graph

.. literalinclude:: dagShortestPath.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: dagShortestPath - Experimental ; One to Many - Experimental on v3.0

One to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_dagShortestPath(`Edges SQL`_, **start vid**, **end vids**)

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: From vertex :math:`5` to vertices :math:`\{7, 11\}`

.. literalinclude:: dagShortestPath.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: dagShortestPath - Experimental ; Many to One - Experimental on v3.0

Many to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_dagShortestPath(`Edges SQL`_, **start vids**, **end vid**)

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: From vertices :math:`\{5, 10\}` to vertex :math:`11`

.. literalinclude:: dagShortestPath.queries
   :start-after: -- q4
   :end-before: -- q5

.. index::
    single: dagShortestPath - Experimental ; Many to Many - Experimental on v3.0

Many to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_dagShortestPath(`Edges SQL`_, **start vids**, **end vids**)

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: From vertices :math:`\{5, 15\}` to vertices :math:`\{11, 17\}` on an
          **undirected** graph

.. literalinclude:: dagShortestPath.queries
   :start-after: -- q5
   :end-before: -- q51

.. index::
    single: dagShortestPath - Experimental ; Combinations - Experimental on v3.2

Combinations
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_dagShortestPath(`Edges SQL`_, `Combinations SQL`_)

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: Using a combinations table on an **undirected** graph

The combinations table:

.. literalinclude:: dijkstraCost.queries
    :start-after: -- q51
    :end-before: -- q52

The query:

.. literalinclude:: dagShortestPath.queries
    :start-after: -- q52
    :end-before: -- q6

Parameters
-------------------------------------------------------------------------------

.. include:: dijkstra-family.rst
    :start-after: dijkstra_parameters_start
    :end-before: dijkstra_parameters_end

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
    :start-after: return_path_complete_start
    :end-before: return_path_complete_end

Additional Examples
-------------------------------------------------------------------------------

:Example 1: Demonstration of repeated values are ignored, and result is sorted.

.. literalinclude:: dagShortestPath.queries
    :start-after: -- q6
    :end-before: -- q7

:Example 2: Making **start_vids** the same as **end_vids**

.. literalinclude:: dagShortestPath.queries
    :start-after: -- q7
    :end-before: -- q8

:Example 3: Manually assigned vertex combinations.

.. literalinclude:: dagShortestPath.queries
    :start-after: -- q8
    :end-before: -- q9

See Also
-------------------------------------------------------------------------------

* :doc:`sampledata`
* `Boost: DAG shortest paths
  <https://www.boost.org/libs/graph/doc/dag_shortest_paths.html>`__
* https://en.wikipedia.org/wiki/Topological_sorting

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

