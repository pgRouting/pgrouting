..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_dijkstraCost
===============================================================================

``pgr_dijkstraCost``

Using Dijkstra algorithm implemented by Boost.Graph, and extract only the
aggregate cost of the shortest path(s) found, for the combination of vertices given.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/dijkstra_shortest_paths.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.1.0

  * New **Proposed** functions:

    * pgr_dijkstraCost(combinations)

* Version 2.2.0

  * New **Official** function

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_dijkstraCost.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_dijkstraCost.html>`__
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_dijkstraCost.html>`__

* **Unsupported versions:**
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_dijkstraCost.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_dijkstraCost.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/dijkstra/doc/pgr_dijkstraCost.html#pgr-dijkstracost>`__
  `2.3 <https://docs.pgrouting.org/2.2/en/src/dijkstra/doc/pgr_dijkstraCost.html#pgr-dijkstracost>`__


Description
-------------------------------------------------------------------------------

The ``pgr_dijkstraCost`` algorithm, is a good choice to calculate the sum of the costs
of the shortest path for a subset of pairs of nodes of the graph.
We make use of the Boost's implementation of dijkstra which runs in
:math:`O(V \log V + E)` time.

The main characteristics are:
  - It does not return a path.
  - Returns the sum of the costs of the shortest path for pair combination of nodes in the graph.
  - Process is done only on edges with positive costs.
  - Values are returned when there is a path.

    - The returned values are in the form of a set of `(start_vid, end_vid, agg_cost)`.

    - When the starting vertex and ending vertex are the same, there is no path.

      - The `agg_cost` int the non included values `(v, v)` is `0`

    - When the starting vertex and ending vertex are the different and there is no path.

      - The `agg_cost` in the non included values `(u, v)` is :math:`\infty`

  - Let be the case the values returned are stored in a table, so the unique index would be the pair:
    `(start_vid, end_vid)`.

  - For undirected graphs, the results are symmetric.

    - The  `agg_cost` of `(u, v)` is the same as for `(v, u)`.

  - Any duplicated value in the `start_vids` or `end_vids` is ignored.

  - The returned values are ordered:

    - `start_vid` ascending
    - `end_vid` ascending

  - Running time: :math:`O(| start\_vids | * (V \log V + E))`

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

     pgr_dijkstraCost(edges_sql, from_vid,  to_vid  [, directed])
     pgr_dijkstraCost(edges_sql, from_vid,  to_vids [, directed])
     pgr_dijkstraCost(edges_sql, from_vids, to_vid  [, directed])
     pgr_dijkstraCost(edges_sql, from_vids, to_vids [, directed])
     pgr_dijkstraCost(edges_sql, combinations_sql   [, directed]) -- Proposed on v3.1
     RETURNS SET OF (start_vid, end_vid, agg_cost)
     OR EMPTY SET

.. rubric:: Using defaults

.. code-block:: none

     pgr_dijkstraCost(edges_sql, from_vid,  to_vid)
     RETURNS SET OF (start_vid, end_vid, agg_cost)
     OR EMPTY SET

:Example: From vertex :math:`2` to vertex  :math:`3` on a **directed** graph

.. literalinclude:: doc-pgr_dijkstraCost.queries
   :start-after: --q1
   :end-before: --q2

.. index::
	single: dijkstraCost(One to One)

One to One
...............................................................................

.. code-block:: none

    pgr_dijkstraCost(edges_sql, from_vid,  to_vid  [, directed])
    RETURNS SET OF (start_vid, end_vid, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertex  :math:`3` on an **undirected** graph

.. literalinclude:: doc-pgr_dijkstraCost.queries
    :start-after: --q2
    :end-before: --q3

.. index::
    single: dijkstraCost(One to Many)

One to Many
...............................................................................

.. code-block:: none

    pgr_dijkstraCost(edges_sql, from_vid,  to_vids [, directed])
    RETURNS SET OF (start_vid, end_vid, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertices :math:`\{3, 11\}` on a **directed** graph

.. literalinclude:: doc-pgr_dijkstraCost.queries
   :start-after: --q4
   :end-before: --q5

.. index::
	single: dijkstraCost(Many to One)

Many to One
...............................................................................

.. code-block:: none

    pgr_dijkstraCost(edges_sql, from_vids, to_vid  [, directed])
    RETURNS SET OF (start_vid, end_vid, agg_cost)
    OR EMPTY SET

:Example: From vertices :math:`\{2, 7\}` to vertex :math:`3` on a **directed** graph

.. literalinclude:: doc-pgr_dijkstraCost.queries
    :start-after: --q3
    :end-before: --q4

.. index::
	single: dijkstraCost(Many to Many)

Many to Many
...............................................................................

.. code-block:: none

    pgr_dijkstraCost(edges_sql, from_vids, to_vids [, directed])
    RETURNS SET OF (start_vid, end_vid, agg_cost)
    OR EMPTY SET

:Example: From vertices :math:`\{2, 7\}` to vertices :math:`\{3, 11\}` on a **directed** graph

.. literalinclude:: doc-pgr_dijkstraCost.queries
   :start-after: --q5
   :end-before: --q6

.. index::
    single: dijkstraCost(Combinations) -- Proposed on v3.1

Combinations
...............................................................................

.. code-block:: none

    pgr_dijkstraCost(TEXT edges_sql, TEXT combination_sql, BOOLEAN directed:=true);
    RETURNS SET OF (start_vid, end_vid, agg_cost)
    OR EMPTY SET

:Example: Using a combinations table on an **undirected** graph

.. literalinclude:: doc-pgr_dijkstraCost.queries
   :start-after: --q8
   :end-before: --q9



Parameters
-------------------------------------------------------------------------------

.. include:: pgr_dijkstra.rst
    :start-after: pgr_dijkstra_parameters_start
    :end-before: pgr_dijkstra_parameters_end

Inner query
-------------------------------------------------------------------------------

Edges query
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Combinations query
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_combinations_sql_start
    :end-before: basic_combinations_sql_end

Return Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_cost_start
    :end-before: return_cost_end


Additional Examples
-------------------------------------------------------------------------------

:Example 1: Demonstration of repeated values are ignored, and result is sorted.

.. literalinclude:: doc-pgr_dijkstraCost.queries
    :start-after: --q6
    :end-before: --q7

:Example 2: Making `start_vids` the same as `end_vids`

.. literalinclude:: doc-pgr_dijkstraCost.queries
    :start-after: --q7
    :end-before: --q8

:Example 3: Four manually assigned (source, target) vertex combinations

.. literalinclude:: doc-pgr_dijkstraCost.queries
   :start-after: --q9
   :end-before: --q10

See Also
-------------------------------------------------------------------------------

* https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
* :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

