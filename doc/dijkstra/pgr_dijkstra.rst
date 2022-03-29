..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_dijkstra.html>`__
  (`3.3 <https://docs.pgrouting.org/3.3/en/pgr_dijkstra.html>`__)
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_dijkstra.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_dijkstra.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_dijkstra.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_dijkstra.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_dijkstra.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_dijkstra.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/dijkstra/doc/pgr_dijkstra.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/dijkstra/doc/pgr_dijkstra.html>`__
  `2.1 <https://docs.pgrouting.org/2.1/en/src/dijkstra/doc/index.html>`__
  `2.0 <https://docs.pgrouting.org/2.0/en/src/dijkstra/doc/index.html>`__

pgr_dijkstra
===============================================================================

``pgr_dijkstra`` — Returns the shortest path(s) using Dijkstra algorithm.
In particular, the Dijkstra algorithm implemented by Boost.Graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/dijkstra_shortest_paths.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.1.0

  * New **Proposed** functions:

    * ``pgr_dijkstra`` (`Combinations`_)

* Version 3.0.0

  * **Official** functions

* Version 2.2.0

  * New **proposed** functions:

    * ``pgr_dijkstra`` (`One to Many`_)
    * ``pgr_dijkstra`` (`Many to One`_)
    * ``pgr_dijkstra`` (`Many to Many`_)

* Version 2.1.0

  * Signature change on ``pgr_dijkstra`` (`One to One`_)

* Version 2.0.0

  * **Official** ``pgr_dijkstra`` (`One to One`_)


Description
-------------------------------------------------------------------------------

.. include:: dijkstra-family.rst
    :start-after: dijkstra_description_start
    :end-before: dijkstra_description_end

.. include:: dijkstra-family.rst
    :start-after: dijkstra_details_start
    :end-before: dijkstra_details_end

Dijkstra's algorithm, conceived by Dutch computer scientist Edsger Dijkstra in
1956.
It is a graph search algorithm that solves the shortest path problem for a graph
with non-negative edge path costs, producing a shortest path from a starting
vertex (**start vid**) to an ending vertex (**end vid**).
This implementation can be used with a directed graph and an undirected graph.

The main characteristics are:
  - Process is done only on edges with positive costs.
  - Values are returned when there is a path.

    - When the starting vertex and ending vertex are the same, there is no path.

      - The `agg_cost` the non included values `(v, v)` is `0`

    - When the starting vertex and ending vertex are the different and there is no path:

      - The `agg_cost` the non included values `(u, v)` is :math:`\infty`

  - For optimization purposes, any duplicated value in the `start vids` or `end vids` are ignored.

  - The returned values are ordered:

    - ``start_vid`` ascending
    - ``end vid`` ascending

  - Running time: :math:`O(| start\_vids | * (V \log V + E))`

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. parsed-literal::

    pgr_dijkstra(`Edges SQL`_, **start vid**, **end vid**  [, directed])
    pgr_dijkstra(`Edges SQL`_, **start vid**, **end vids** [, directed])
    pgr_dijkstra(`Edges SQL`_, **start vids**, **end vid**  [, directed])
    pgr_dijkstra(`Edges SQL`_, **start vids**, **end vids** [, directed])
    pgr_dijkstra(`Edges SQL`_, `Combinations SQL`_ [, directed])
    RETURNS SET OF (seq, path_seq [, start vid] [, end vid], node, edge, cost, agg_cost)
    OR EMPTY SET

.. index::
    single: dijkstra(One to One)

One to One
...............................................................................

.. parsed-literal::

    pgr_dijkstra(Edges SQL, start vid,  end vid  [, directed])
    pgr_dijkstra(`Edges SQL`_, **start vid**, **end vid**  [, directed])
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

.. parsed-literal::

    pgr_dijkstra(`Edges SQL`_, **start vid**, **end vids** [, directed])
    pgr_dijkstra(`Edges SQL`_, `Combinations SQL`_ [, directed])
    RETURNS SET OF (seq, path_seq, end vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertices :math:`\{3, 5\}` on an **undirected** graph

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: dijkstra(Many to One)

Many to One
...............................................................................

.. parsed-literal::

    pgr_dijkstra(`Edges SQL`_, **start vids**, **end vids** [, directed])
    RETURNS SET OF (seq, path_seq, start vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertices :math:`\{2, 11\}` to vertex :math:`5` on a **directed** graph

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q4
   :end-before: -- q5

.. index::
    single: dijkstra(Many to Many)

Many to Many
...............................................................................

.. parsed-literal::

    pgr_dijkstra(`Edges SQL`_, **start vids**, **end vids** [, directed])
    RETURNS SET OF (seq, path_seq, start vid, end vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertices :math:`\{2, 11\}` to vertices :math:`\{3, 5\}` on an **undirected** graph

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q5
   :end-before: -- q6

.. index::
    single: dijkstra(Combinations) - Proposed on v3.1

Combinations
...............................................................................

.. parsed-literal::

    pgr_dijkstra(`Edges SQL`_, `Combinations SQL`_ [, directed])
    RETURNS SET OF (seq, path_seq, start vid, end vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: Using a combinations table on an **undirected** graph

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q6
   :end-before: -- q61


Parameters
-------------------------------------------------------------------------------

.. include:: dijkstra-family.rst
    :start-after: dijkstra_parameters_start
    :end-before: dijkstra_parameters_end

Optional parameters
...............................................................................

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

.. contents::
   :local:

For **directed** graphs with ``cost`` and ``reverse_cost`` columns
...............................................................................

.. figure:: /images/Fig1-originalData.png
   :scale: 50%

   Directed graph with cost and reverse cost columns

:Example 1: Path from :math:`2` to :math:`3`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q7
   :end-before: -- q71

:Example 2: Path from :math:`2` to :math:`5`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q71
   :end-before: -- q72

:Example 3: Path from :math:`11` to :math:`3`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q72
   :end-before: -- q73

:Example 4: Path from :math:`11` to :math:`5`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q73
   :end-before: -- q74

:Example 5: Using `One to Many`_ to get the solution of examples 1 and 2

Paths :math:`\{2\}\rightarrow\{3, 5\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q74
   :end-before: -- q75

:Example 6: Using `Many to One`_ to get the solution of examples 2 and 4

Paths :math:`\{2, 11\}\rightarrow\{5\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q75
   :end-before: -- q76

:Example 7: Using `Many to Many`_ to get the solution of examples 1 to 4

Paths :math:`\{2, 11\}\rightarrow\{3, 5\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q76
   :end-before: -- q77

:Example 8: Using `Combinations`_ to get the solution of examples 1 to 3

Paths :math:`\{2\}\rightarrow\{3, 5\}\cup\{11\}\rightarrow\{3\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q77
   :end-before: -- q8

For **undirected** graphs with ``cost`` and ``reverse_cost`` columns
...............................................................................

.. figure:: /images/Fig6-undirected.png
   :scale: 50%

   Undirected graph with cost and reverse cost columns

:Example 9: Path from :math:`2` to :math:`3`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q9
   :end-before: -- q91

:Example 10: Path from :math:`2` to :math:`5`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q91
   :end-before: -- q92

:Example 11: Path from :math:`11` to :math:`3`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q92
   :end-before: -- q93

:Example 12: Path from :math:`11` to :math:`5`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q93
   :end-before: -- q94

:Example 13: Using `One to Many`_ to get the solution of examples 9 and 10

Paths :math:`\{2\}\rightarrow\{3, 5\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q94
   :end-before: -- q95

:Example 14: Using `Many to One`_ to get the solution of examples 10 and 12

Paths :math:`\{2, 11\}\rightarrow\{5\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q95
   :end-before: -- q96

:Example 15: Using `Many to Many`_ to get the solution of examples 9 to 12

Paths :math:`\{2, 11\}\rightarrow\{3, 5\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q96
   :end-before: -- q97

:Example 16: Using `Combinations`_ to get the solution of examples 9 to 11

Paths :math:`\{2\}\rightarrow\{3, 5\}\cup\{11\}\rightarrow\{3\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q97
   :end-before: -- q10

For **directed** graphs only with ``cost`` column
...............................................................................

.. figure:: /images/Fig2-cost.png
   :scale: 50%

   Directed graph only with cost column

:Example 17: Path from :math:`2` to :math:`3`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q11
   :end-before: -- q111

:Example 18: Path from :math:`2` to :math:`5`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q111
   :end-before: -- q112

:Example 19: Path from :math:`11` to :math:`3`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q112
   :end-before: -- q113

:Example 20: Path from :math:`11` to :math:`5`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q113
   :end-before: -- q114

:Example 21: Using `One to Many`_ to get the solution of examples 17 and 18

Paths :math:`\{2\}\rightarrow\{3, 5\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q114
   :end-before: -- q115

:Example 22: Using `Many to One`_ to get the solution of examples 18 and 20

Paths :math:`\{2, 11\}\rightarrow\{5\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q115
   :end-before: -- q116

:Example 23: Using `Many to Many`_ to get the solution of examples 17 to 20

Paths :math:`\{2, 11\}\rightarrow\{3, 5\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q116
   :end-before: -- q117

:Example 24: Using `Combinations`_ to get the solution of examples 17 to 19

Paths :math:`\{2\}\rightarrow\{3, 5\}\cup\{11\}\rightarrow\{3\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q117
   :end-before: -- q12

For **undirected** graphs only with ``cost`` column
...............................................................................

.. figure:: /images/Fig4-costUndirected.png
   :scale: 50%

   Undirected graph only with cost column

:Example 25: Path from :math:`2` to :math:`3`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q13
   :end-before: -- q131

:Example 26: Path from :math:`2` to :math:`5`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q131
   :end-before: -- q132

:Example 27: Path from :math:`11` to :math:`3`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q132
   :end-before: -- q133

:Example 28: Path from :math:`11` to :math:`5`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q133
   :end-before: -- q134

:Example 29: Using `One to Many`_ to get the solution of examples 17 and 18

Paths :math:`\{2\}\rightarrow\{3, 5\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q134
   :end-before: -- q135

:Example 30: Using `Many to One`_ to get the solution of examples 18 and 20

Paths :math:`\{2, 11\}\rightarrow\{5\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q135
   :end-before: -- q136

:Example 31: Using `Many to Many`_ to get the solution of examples 17 to 20

Paths :math:`\{2, 11\}\rightarrow\{3, 5\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q136
   :end-before: -- q137

:Example 32: Using `Combinations`_ to get the solution of examples 17 to 19

Paths :math:`\{2\}\rightarrow\{3, 5\}\cup\{11\}\rightarrow\{3\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q137
   :end-before: -- q14


Equvalences between signatures
...............................................................................

The following examples find the path for :math:`\{2\}\rightarrow\{3\}`

:Example 33: Using `One to One`_

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q15
   :end-before: -- q151

:Example 34: Using `One to Many`_

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q151
   :end-before: -- q152

:Example 35: Using `Many to One`_

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q152
   :end-before: -- q153

:Example 36: Using `Many to Many`_

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q153
   :end-before: -- q154

:Example 37: Using `Combinations`_

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q154
   :end-before: -- q16

See Also
-------------------------------------------------------------------------------

* https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

