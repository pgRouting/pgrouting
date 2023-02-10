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
  (`3.5 <https://docs.pgrouting.org/3.5/en/pgr_dijkstra.html>`__)
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_dijkstra.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_dijkstra.html>`__
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

``pgr_dijkstra``
===============================================================================

``pgr_dijkstra`` — Shortest path(s) using Dijkstra algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/dijkstra_shortest_paths.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.5.0

  * Standarizing output columns to |short-generic-result|

    * ``pgr_dijkstra`` (`One to One`_) added `start_vid` and `end_vid` columns.
    * ``pgr_dijkstra`` (`One to Many`_) added `end_vid` column.
    * ``pgr_dijkstra`` (`Many to One`_) added `start_vid` column.

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


Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_dijkstra(`Edges SQL`_, **start vid**, **end vid**, [``directed``])
   | pgr_dijkstra(`Edges SQL`_, **start vid**, **end vids**, [``directed``])
   | pgr_dijkstra(`Edges SQL`_, **start vids**, **end vid**, [``directed``])
   | pgr_dijkstra(`Edges SQL`_, **start vids**, **end vids**, [``directed``])
   | pgr_dijkstra(`Edges SQL`_, `Combinations SQL`_, [``directed``])

   | RETURNS SET OF |short-generic-result|
   | OR EMPTY SET


.. Warning:: Breaking change on 3.5.0

   Read the :doc:`migration` about how to migrate from the old result columns to
   the new result columns.

.. index::
    single: dijkstra(One to One)

One to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_dijkstra(`Edges SQL`_, **start vid**, **end vid**, [``directed``])

   | RETURNS SET OF |short-generic-result|
   | OR EMPTY SET

:Example: From vertex :math:`6` to vertex  :math:`10` on a **directed** graph

.. literalinclude:: doc-pgr_dijkstra.queries
    :start-after: -- q2
    :end-before: -- q3

.. index::
    single: dijkstra(One to Many)

One to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_dijkstra(`Edges SQL`_, **start vid**, **end vids**, [``directed``])

   | RETURNS SET OF |short-generic-result|
   | OR EMPTY SET

:Example: From vertex :math:`6` to vertices :math:`\{10, 17\}` on a **directed**

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: dijkstra(Many to One)

Many to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_dijkstra(`Edges SQL`_, **start vids**, **end vid**, [``directed``])

   | RETURNS SET OF |short-generic-result|
   | OR EMPTY SET

:Example: From vertices :math:`\{6, 1\}` to vertex :math:`17` on a **directed**
          graph

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q4
   :end-before: -- q5

.. index::
    single: dijkstra(Many to Many)

Many to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_dijkstra(`Edges SQL`_, **start vids**, **end vids**, [``directed``])

   | RETURNS SET OF |short-generic-result|
   | OR EMPTY SET

:Example: From vertices :math:`\{6, 1\}` to vertices :math:`\{10, 17\}` on an
          **undirected** graph

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q5
   :end-before: -- q51

.. index::
    single: dijkstra(Combinations) - Proposed on v3.1

Combinations
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_dijkstra(`Edges SQL`_, `Combinations SQL`_, [``directed``])

   | RETURNS SET OF |short-generic-result|
   | OR EMPTY SET

:Example: Using a combinations table on an **undirected** graph

The combinations table:

.. literalinclude:: doc-pgr_dijkstra.queries
    :start-after: -- q51
    :end-before: -- q52

The query:

.. literalinclude:: doc-pgr_dijkstra.queries
    :start-after: -- q52
    :end-before: -- q6

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
    :start-after: return_path_short_start
    :end-before: return_path_short_end

Additional Examples
-------------------------------------------------------------------------------

:Example: Demonstration of repeated values are ignored, and result is sorted.

.. literalinclude:: doc-pgr_dijkstra.queries
    :start-after: -- q6
    :end-before: -- q7

:Example 2: Making **start_vids** the same as **end_vids**

.. literalinclude:: doc-pgr_dijkstra.queries
    :start-after: -- q7
    :end-before: -- q8

:Example: Manually assigned vertex combinations.

.. literalinclude:: doc-pgr_dijkstra.queries
    :start-after: -- q8
    :end-before: -- q9


The examples of this section are based on the :doc:`sampledata` network.

.. contents::
   :local:

For **directed** graphs with ``cost`` and ``reverse_cost`` columns
...............................................................................

.. figure:: /images/Fig1-originalData.png
   :scale: 50%

   Directed graph with cost and reverse cost columns

1) Path from :math:`6` to :math:`10`
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q70
   :end-before: -- q71

2) Path from :math:`6` to :math:`7`
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q71
   :end-before: -- q72

3) Path from :math:`12` to :math:`10`
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q72
   :end-before: -- q73

4) Path from :math:`12` to :math:`7`
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q73
   :end-before: -- q74

5) Using `One to Many`_ to get the solution of examples 1 and 2
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Paths :math:`\{6\}\rightarrow\{10, 7\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q74
   :end-before: -- q75

6) Using `Many to One`_ to get the solution of examples 2 and 4
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Paths :math:`\{6, 12\}\rightarrow\{7\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q75
   :end-before: -- q76

7) Using `Many to Many`_ to get the solution of examples 1 to 4
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Paths :math:`\{6, 12\}\rightarrow\{10, 7\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q76
   :end-before: -- q77

8) Using `Combinations`_ to get the solution of examples 1 to 3
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Paths :math:`\{6\}\rightarrow\{10, 7\}\cup\{12\}\rightarrow\{10\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q77
   :end-before: -- q80

For **undirected** graphs with ``cost`` and ``reverse_cost`` columns
...............................................................................

.. figure:: /images/Fig6-undirected.png
   :scale: 50%

   Undirected graph with cost and reverse cost columns

9) Path from :math:`6` to :math:`10`
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q90
   :end-before: -- q91

10) Path from :math:`6` to :math:`7`
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q91
   :end-before: -- q92

11) Path from :math:`12` to :math:`10`
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q92
   :end-before: -- q93

12) Path from :math:`12` to :math:`7`
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q93
   :end-before: -- q94

13) Using `One to Many`_ to get the solution of examples 9 and 10
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Paths :math:`\{6\}\rightarrow\{10, 7\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q94
   :end-before: -- q95

14) Using `Many to One`_ to get the solution of examples 10 and 12
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Paths :math:`\{6, 12\}\rightarrow\{7\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q95
   :end-before: -- q96

15) Using `Many to Many`_ to get the solution of examples 9 to 12
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Paths :math:`\{6, 12\}\rightarrow\{10, 7\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q96
   :end-before: -- q97

16) Using `Combinations`_ to get the solution of examples 9 to 11
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Paths :math:`\{6\}\rightarrow\{10, 7\}\cup\{12\}\rightarrow\{10\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q97
   :end-before: -- q10

For **directed** graphs only with ``cost`` column
...............................................................................

.. figure:: /images/Fig2-cost.png
   :scale: 50%

   Directed graph only with cost column

17) Path from :math:`6` to :math:`10`
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q11
   :end-before: -- q111

18) Path from :math:`6` to :math:`7`
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q111
   :end-before: -- q112

19) Path from :math:`12` to :math:`10`
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q112
   :end-before: -- q113

20) Path from :math:`12` to :math:`7`
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q113
   :end-before: -- q114

21) Using `One to Many`_ to get the solution of examples 17 and 18
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Paths :math:`\{6\}\rightarrow\{10, 7\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q114
   :end-before: -- q115

22) Using `Many to One`_ to get the solution of examples 18 and 20
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Paths :math:`\{6, 12\}\rightarrow\{7\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q115
   :end-before: -- q116

23) Using `Many to Many`_ to get the solution of examples 17 to 20
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Paths :math:`\{6, 12\}\rightarrow\{10, 7\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q116
   :end-before: -- q117

24) Using `Combinations`_ to get the solution of examples 17 to 19
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Paths :math:`\{6\}\rightarrow\{10, 7\}\cup\{12\}\rightarrow\{10\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q117
   :end-before: -- q12

For **undirected** graphs only with ``cost`` column
...............................................................................

.. figure:: /images/Fig4-costUndirected.png
   :scale: 50%

   Undirected graph only with cost column

25) Path from :math:`6` to :math:`10`
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q13
   :end-before: -- q131

26) Path from :math:`6` to :math:`7`
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q131
   :end-before: -- q132

27) Path from :math:`12` to :math:`10`
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q132
   :end-before: -- q133

28) Path from :math:`12` to :math:`7`
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q133
   :end-before: -- q134

29) Using `One to Many`_ to get the solution of examples 25 and 26
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Paths :math:`\{6\}\rightarrow\{10, 7\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q134
   :end-before: -- q135

30) Using `Many to One`_ to get the solution of examples 26 and 28
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Paths :math:`\{6, 12\}\rightarrow\{7\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q135
   :end-before: -- q136

31) Using `Many to Many`_ to get the solution of examples 25 to 28
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Paths :math:`\{6, 12\}\rightarrow\{10, 7\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q136
   :end-before: -- q137

32) Using `Combinations`_ to get the solution of examples 25 to 27
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Paths :math:`\{6\}\rightarrow\{10, 7\}\cup\{12\}\rightarrow\{10\}`

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q137
   :end-before: -- q14


Equvalences between signatures
...............................................................................

The following examples find the path for :math:`\{6\}\rightarrow\{10\}`

33) Using `One to One`_
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q15
   :end-before: -- q151

34) Using `One to Many`_
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q151
   :end-before: -- q152

35) Using `Many to One`_
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q152
   :end-before: -- q153

36) Using `Many to Many`_
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: doc-pgr_dijkstra.queries
   :start-after: -- q153
   :end-before: -- q154

37) Using `Combinations`_
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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

