..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_withPoints.html>`__
  (`3.6 <https://docs.pgrouting.org/3.6/en/pgr_withPoints.html>`__)
  `3.5 <https://docs.pgrouting.org/3.5/en/pgr_withPoints.html>`__
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_withPoints.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_withPoints.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_withPoints.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_withPoints.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_withPoints.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_withPoints.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_withPoints.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_withPoints.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/withPoints/doc/pgr_withPoints.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/withPoints/doc/pgr_withPoints.html>`__

``pgr_withPoints`` - Proposed
===============================================================================

``pgr_withPoints`` - Returns the shortest path in a graph with additional
temporary vertices.

.. include:: proposed.rst
   :start-after: begin-warning
   :end-before: end-warning

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/table_of_contents.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.2.0

  * New **proposed** function:

    * pgr_withPoints(Combinations)

* Version 2.2.0

  * New **proposed** function

Description
-------------------------------------------------------------------------------

Modify the graph to include points defined by points_sql.
Using Dijkstra algorithm, find the shortest path(s)

**The main characteristics are:**

- Process is done only on edges with positive costs.
- Vertices of the graph are:

  - **positive** when it belongs to the edges_sql
  - **negative** when it belongs to the points_sql

- Values are returned when there is a path.

  - When the starting vertex and ending vertex are the same, there is no path.
    - The agg_cost the non included values (v, v) is 0

  - When the starting vertex and ending vertex are the different and there is no
    path:
    - The agg_cost the non included values (u, v) is ∞

- For optimization purposes, any duplicated value in the start_vids or end_vids
  are ignored.
- The returned values are ordered:
  - start_vid ascending
  - end_vid ascending

* Running time: :math:`O(|start\_vids|\times(V \log V + E))`

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_withPoints(`Edges SQL`_, `Points SQL`_, **start vid**, **end vid**, [**options**])
   | pgr_withPoints(`Edges SQL`_, `Points SQL`_, **start vid**, **end vids**, [**options**])
   | pgr_withPoints(`Edges SQL`_, `Points SQL`_, **start vids**, **end vid**, [**options**])
   | pgr_withPoints(`Edges SQL`_, `Points SQL`_, **start vids**, **end vids**, [**options**])
   | pgr_withPoints(`Edges SQL`_, `Points SQL`_, `Combinations SQL`_, [**options**])
   | **options:** ``[directed, driving_side, details])``

   | RETURNS SET OF |old-pid-result|
   | OR EMTPY SET

.. index::
    single: withPoints(One to One) - Proposed on v2.2

One to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_withPoints(`Edges SQL`_, `Points SQL`_, **start vid**, **end vid**, [**options**])
   | **options:** [directed, driving_side, details])

   | RETURNS SET OF |result-1-1|
   | OR EMTPY SET

:Example: From point :math:`1` to vertex :math:`10` with details

.. literalinclude:: doc-pgr_withPoints.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: withPoints(One to Many) - Proposed on v2.2

One to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_withPoints(`Edges SQL`_, `Points SQL`_, **start vid**, **end vids**, [**options**])
   | **options:** [directed, driving_side, details])

   | RETURNS SET OF |pid-1-m|
   | OR EMTPY SET

:Example: From point :math:`1` to point :math:`3` and vertex :math:`7` on an
          undirected graph

.. literalinclude:: doc-pgr_withPoints.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: withPoints(Many to One) - Proposed on v2.2

Many to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_withPoints(`Edges SQL`_, `Points SQL`_, **start vids**, **end vid**, [**options**])
   | **options:** [directed, driving_side, details])

   | RETURNS SET OF |pid-m-1|
   | OR EMTPY SET

:Example: From point :math:`1` and vertex :math:`6` to point :math:`3`

.. literalinclude:: doc-pgr_withPoints.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: withPoints(Many to Many) - Proposed on v2.2

Many to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_withPoints(`Edges SQL`_, `Points SQL`_, **start vids**, **end vids**, [**options**])
   | **options:** [directed, driving_side, details])

   | RETURNS SET OF |pid-m-m|
   | OR EMTPY SET

:Example: From point :math:`1` and vertex :math:`6`  to point :math:`3` and
          vertex :math:`1`

.. literalinclude:: doc-pgr_withPoints.queries
   :start-after: -- q4
   :end-before: -- q5

.. index::
    single: withPoints(Combinations) - Proposed on v3.2

Combinations
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_withPoints(`Edges SQL`_, `Points SQL`_, `Combinations SQL`_, [**options**])
   | **options:** [directed, driving_side, details])

   | RETURNS SET OF |pid-m-m|
   | OR EMTPY SET

:Example: Two combinations

From point :math:`1` to vertex :math:`10`, and from vertex :math:`6` to point
:math:`3` with **right** side driving.

.. literalinclude:: doc-pgr_withPoints.queries
   :start-after: -- q5
   :end-before: -- q6

Parameters
-------------------------------------------------------------------------------

.. include:: withPoints-category.rst
    :start-after: withPoints_parameters_start
    :end-before: withPoints_parameters_end

Optional parameters
...............................................................................

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

With points optional parameters
...............................................................................

.. include:: withPoints-family.rst
    :start-after: withPoints_optionals_start
    :end-before: withPoints_optionals_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Points SQL
...............................................................................

.. include:: withPoints-category.rst
    :start-after: points_sql_start
    :end-before: points_sql_end

Combinations SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_combinations_sql_start
    :end-before: basic_combinations_sql_end

Result Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_withpoint_path_short_start
    :end-before: return_withpoint_path_short_end

Additional Examples
-------------------------------------------------------------------------------

.. contents::
   :local:

Use :doc:`pgr_findCloseEdges` in the `Points SQL`_.
...............................................................................

Find the routes from vertex :math:`1` to the two closest locations on the graph
of point `(2.9, 1.8)`.

.. literalinclude:: doc-pgr_withPoints.queries
    :start-after: -- q9
    :end-before: -- q10

* Point :math:`-1` corresponds to the closest edge from point `(2.9,1.8)`.
* Point :math:`-2` corresponds to the next close edge from point `(2.9,1.8)`.

Usage variations
...............................................................................

All the examples are about traveling from point :math:`1` and vertex :math:`5`
to points :math:`\{2, 3, 6\}` and vertices :math:`\{10, 11\}`

.. literalinclude:: doc-pgr_withPoints.queries
   :start-after: -- q6
   :end-before: -- q7

Passes in front or visits with right side driving.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

For point :math:`6` and vertex :math:`11`.


.. literalinclude:: doc-pgr_withPoints.queries
   :start-after: -- q7
   :end-before: -- q8

Passes in front or visits with left side driving.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

For point :math:`6` and vertex :math:`11`.

.. literalinclude:: doc-pgr_withPoints.queries
   :start-after: -- q8
   :end-before: -- q9


See Also
-------------------------------------------------------------------------------

* :doc:`withPoints-family`
* :doc:`withPoints-category`
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
