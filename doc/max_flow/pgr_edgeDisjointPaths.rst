..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_edgeDisjointPaths.html>`__
  (`3.6 <https://docs.pgrouting.org/3.6/en/pgr_edgeDisjointPaths.html>`__)
  `3.5 <https://docs.pgrouting.org/3.5/en/pgr_edgeDisjointPaths.html>`__
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_edgeDisjointPaths.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_edgeDisjointPaths.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_edgeDisjointPaths.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_edgeDisjointPaths.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_edgeDisjointPaths.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_edgeDisjointPaths.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_edgeDisjointPaths.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_edgeDisjointPaths.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/max_flow/doc/pgr_edgeDisjointPaths.html>`__

``pgr_edgeDisjointPaths``
===============================================================================

``pgr_edgeDisjointPaths`` — Calculates edge disjoint paths between two groups of
vertices.


.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/boykov_kolmogorov_max_flow.html

   Boost Graph Inside

.. Rubric:: Availability

* Version 3.2.0

  * New **proposed** function:

    * pgr_edgeDisjointPaths(Combinations)

* Version 3.0.0

  * **Official** function

* Version 2.5.0

  * **Proposed** function

* Version 2.3.0

  * New **Experimental** function


Description
-------------------------------------------------------------------------------

Calculates the edge disjoint paths between two groups of vertices.
Utilizes underlying maximum flow algorithms to calculate the paths.

The main characterics are:
  - Calculates the edge disjoint paths between any two groups of vertices.
  - Returns EMPTY SET when source and destination are the same, or cannot be reached.
  - The graph can be directed or undirected.
  - Uses :doc:`pgr_boykovKolmogorov` to calculate the paths.

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_edgeDisjointPaths(`Edges SQL`_, **start vid**, **end vid**, [``directed``])
   | pgr_edgeDisjointPaths(`Edges SQL`_, **start vid**, **end vids**, [``directed``])
   | pgr_edgeDisjointPaths(`Edges SQL`_, **start vids**, **end vid**, [``directed``])
   | pgr_edgeDisjointPaths(`Edges SQL`_, **start vids**, **end vids**, [``directed``])
   | pgr_edgeDisjointPaths(`Edges SQL`_, `Combinations SQL`_, [``directed``])

   | RETURNS SET OF |result-disjoint|
   | OR EMPTY SET

.. index::
    single: edgeDisjointPaths(One to One)

One to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_edgeDisjointPaths(`Edges SQL`_, **start vid**, **end vid**, [``directed``])

   | RETURNS SET OF |result-disjoint-1-1|
   | OR EMPTY SET

:Example: From vertex :math:`11` to vertex :math:`12`

.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: edgeDisjointPaths(One to Many)

One to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_edgeDisjointPaths(`Edges SQL`_, **start vid**, **end vids**, [``directed``])

   | RETURNS SET OF |result-disjoint-1-m|
   | OR EMPTY SET

:Example: From vertex :math:`11` to vertices :math:`\{5, 10, 12\}`


.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: edgeDisjointPaths(Many to One)

Many to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_edgeDisjointPaths(`Edges SQL`_, **start vids**, **end vid**, [``directed``])

   | RETURNS SET OF |result-disjoint-m-1|
   | OR EMPTY SET

:Example: From vertices :math:`\{11, 3, 17\}` to vertex :math:`12`

.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: edgeDisjointPaths(Many to Many)

Many to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_edgeDisjointPaths(`Edges SQL`_, **start vids**, **end vids**, [``directed``])

   | RETURNS SET OF |result-disjoint-m-m|
   | OR EMPTY SET

:Example: From vertices :math:`\{11, 3, 17\}` to vertices :math:`\{5, 10, 12\}`

.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q4
   :end-before: -- q5

.. index::
    single: edgeDisjointPaths(Combinations) -- Proposed on v3.2

Combinations
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_edgeDisjointPaths(`Edges SQL`_, `Combinations SQL`_, [``directed``])

   | RETURNS SET OF |result-disjoint-m-m|
   | OR EMPTY SET

:Example: Using a combinations table, equivalent to calculating result from
          vertices :math:`\{5, 6\}` to vertices :math:`\{10, 15, 14\}` on an
          undirected graph.

The combinations table:

.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q5
   :end-before: -- q51

The query:

.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q51
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

.. include::  pgRouting-concepts.rst
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

:Example: Manually assigned vertex combinations on an undirected graph.

.. literalinclude:: doc-pgr_edgeDisjointPaths.queries
   :start-after: -- q6
   :end-before: -- q7

See Also
-------------------------------------------------------------------------------

* :doc:`flow-family`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
