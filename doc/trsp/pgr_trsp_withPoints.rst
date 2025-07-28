..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: Turn Restriction Shortest Path Family ; pgr_trsp_withPoints
   single: Shortest Path Category ; pgr_trsp_withPoints
   single: Restrictions Category ; pgr_trsp_withPoints
   single: With Points Category ; pgr_trsp_withPoints
   single: trsp_withPoints

|

``pgr_trsp_withPoints``
===============================================================================

``pgr_trsp_withPoints`` Routing Vertex/Point with restrictions.

.. rubric:: Availability

.. rubric:: Version 4.0.0

* Function promoted to official.
* **Driving side** parameter is positional unnamed and compulsory.

  * Valid values depend on kind of graph

* Breaking change, signatures no longer available:

  * pgr_trsp_withpoints(text,text,text,anyarray,anyarray,boolean,character,boolean)
  * pgr_trsp_withpoints(text,text,text,anyarray,bigint,boolean,character,boolean)
  * pgr_trsp_withpoints(text,text,text,bigint,anyarray,boolean,character,boolean)
  * pgr_trsp_withpoints(text,text,text,bigint,bigint,boolean,character,boolean)
  * pgr_trsp_withpoints(text,text,text,text,boolean,character,boolean)

.. rubric:: Version 3.4.0

* New proposed function.

Description
-------------------------------------------------------------------------------

Modify the graph to include points defined by `Points SQL`_.
Consider the invalid paths on `Restrictions SQL`_.
Using Dijkstra algorithm, find the shortest path.

The main characteristics are:

- Process is done only on edges with positive costs.
- Vertices of the graph are:

  - **positive** when it belongs to the `Edges SQL`_
  - **negative** when it belongs to the `Points SQL`_

- Values are returned when there is a path.

  - When the starting vertex and ending vertex are the same, there is no path.

    - The `agg_cost` in the non included values `(v, v)` is `0`

  - When the starting vertex and ending vertex are the different and there is no
    path:

    - The `agg_cost` in the non included values `(u, v)` is :math:`\infty`

- For optimization purposes, any duplicated value in the input arrays of **start vids** or
  **end vids** or are ignored.

- The returned values are ordered:

  - `start_vid` ascending
  - `end_vid` ascending

- Running time: :math:`O(|start\_vids|\times(V \log V + E))`

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_trsp_withPoints(`Edges SQL`_, `Restrictions SQL`_, `Points SQL`_, **start vid**, **end vid**, [**options**])
   | pgr_trsp_withPoints(`Edges SQL`_, `Restrictions SQL`_, `Points SQL`_, **start vid**, **end vids**, [**options**])
   | pgr_trsp_withPoints(`Edges SQL`_, `Restrictions SQL`_, `Points SQL`_, **start vids**, **end vid**, [**options**])
   | pgr_trsp_withPoints(`Edges SQL`_, `Restrictions SQL`_, `Points SQL`_, **start vids**, **end vids**, [**options**])
   | pgr_trsp_withPoints(`Edges SQL`_, `Restrictions SQL`_, `Combinations SQL`_, `Points SQL`_, [**options**])
   | **options:** ``[directed, driving_side, details]``

   | Returns set of |short-generic-result|
   | OR EMPTY SET

.. index::
    single: trsp_withPoints ; One to One

One to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_trsp_withPoints(`Edges SQL`_, `Restrictions SQL`_, `Points SQL`_, **start vid**, **end vid**, [**options**])
   | **options:** ``[directed, driving_side, details]``

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: From point :math:`1` to vertex :math:`10` with right driving side in directed graph.
          (with details)

.. literalinclude:: trsp_withPoints.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: trsp_withPoints ; One to Many

One to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_trsp_withPoints(`Edges SQL`_, `Restrictions SQL`_, `Points SQL`_, **start vid**, **end vids**, **driving side** [**options**])
   | **options:** ``[directed, details]``

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: From point :math:`1` to point :math:`3` and vertex :math:`7` on an
          undirected graph

.. literalinclude:: trsp_withPoints.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: trsp_withPoints ; Many to One

Many to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_trsp_withPoints(`Edges SQL`_, `Restrictions SQL`_, `Points SQL`_, **start vids**, **end vid**, [**options**])
   | **options:** ``[directed, driving_side, details]``

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: From point :math:`1` and vertex :math:`6` to point :math:`3` with right driving side in directed graph.
   (without details)

.. literalinclude:: trsp_withPoints.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: trsp_withPoints ; Many to Many

Many to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_trsp_withPoints(`Edges SQL`_, `Restrictions SQL`_, `Points SQL`_, **start vids**, **end vids**, [**options**])
   | **options:** ``[directed, driving_side, details]``

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: From point :math:`1` and vertex :math:`6` to point :math:`3` and
          vertex :math:`1` with left side driving.

.. literalinclude:: trsp_withPoints.queries
   :start-after: -- q4
   :end-before: -- q5

.. index::
    single: trsp_withPoints ; Combinations

Combinations
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_trsp_withPoints(`Edges SQL`_, `Restrictions SQL`_, `Combinations SQL`_, `Points SQL`_, [**options**])
   | **options:** ``[directed, driving_side, details]``

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: Two combinations

From point :math:`1` to vertex :math:`10`, and from vertex :math:`6` to point
:math:`3` with right side driving.
(with details)

.. literalinclude:: trsp_withPoints.queries
   :start-after: -- q5
   :end-before: -- q6

Parameters
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: restriction_parameters_start
    :end-before: restriction_parameters_end

Optional parameters
...............................................................................

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

With points optional parameters
...............................................................................

.. include:: withPoints-category.rst
   :start-after: withPoints_optionals_start
   :end-before: withPoints_optionals_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Restrictions SQL
...............................................................................

.. include:: pgRouting-concepts.rst
   :start-after: restrictions_columns_start
   :end-before: restrictions_columns_end

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

Result columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_path_complete_start
    :end-before: return_path_complete_end

Additional Examples
-------------------------------------------------------------------------------

.. contents::
   :local:

Use :doc:`pgr_findCloseEdges` in the `Points SQL`_.
...............................................................................

Find the routes from vertex :math:`1` to the two closest locations on the graph
of point `(2.9, 1.8)`.

.. literalinclude:: trsp_withPoints.queries
    :start-after: -- q9
    :end-before: -- q10

* Point :math:`-1` corresponds to the closest edge from point `(2.9, 1.8)`.
* Point :math:`-2` corresponds to the next close edge from point `(2.9, 1.8)`.

Usage variations
...............................................................................

All the examples are about traveling from point :math:`1` and vertex :math:`5`
to points :math:`\{2, 3, 6\}` and vertices :math:`\{10, 11\}`
with restrictions

.. literalinclude:: trsp_withPoints.queries
   :start-after: -- q6
   :end-before: -- q7

Passes in front or visits with right side driving.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

For point :math:`6` and vertex :math:`11`.

.. literalinclude:: trsp_withPoints.queries
   :start-after: -- q7
   :end-before: -- q8

Passes in front or visits with left side driving.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

For point :math:`6` and vertex :math:`11`.

.. literalinclude:: trsp_withPoints.queries
   :start-after: -- q8
   :end-before: -- q9


See Also
-------------------------------------------------------------------------------

* :doc:`TRSP-family`
* :doc:`withPoints-category`
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
