..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: withPoints Family ; pgr_withPointsCost
   single: With Points Category ; pgr_withPointsCost
   single: Cost Category ; pgr_withPointsCost
   single: withPointsCost

|

``pgr_withPointsCost``
===============================================================================

``pgr_withPointsCost`` - Calculates the shortest path and returns only the
aggregate cost of the shortest path found, for the combination of points
given.

.. rubric:: Availability

.. rubric:: Version 4.0.0

* Function promoted to official.
* **Driving side** parameter is unnamed and compulsory.

  * Valid values depend on kind of graph

* Output columns standardized to |matrix-result|
* Breaking change, signatures no longer available:

  * pgr_withpointscost(text,text,anyarray,anyarray,boolean,character)
  * pgr_withpointscost(text,text,anyarray,bigint,boolean,character)
  * pgr_withpointscost(text,text,bigint,anyarray,boolean,character)
  * pgr_withpointscost(text,text,bigint,bigint,boolean,character)
  * pgr_withpointscost(text,text,text,boolean,character)


.. rubric:: Version 3.2.0

* New proposed signature:

  * pgr_withPointsCost(Combinations)

.. rubric:: Version 2.2.0

* New proposed function.

Description
-------------------------------------------------------------------------------

Modify the graph to include points defined by points_sql.
Using Dijkstra algorithm, return only the aggregate cost of the shortest path
found.

The main characteristics are:

- Process is done only on edges with positive costs.
- It does not return a path.
- Returns the sum of the costs of the shortest path for pair combination of
  vertices in the modified graph.

  - The returned values are in the form of a set of |matrix-result|.

- Vertices of the graph are:

  - **positive** when it belongs to the edges_sql
  - **negative** when it belongs to the points_sql

- Values are returned when there is a path.

  - When the starting vertex and ending vertex are the same, there is no path.

    - The `agg_cost` in the non included values `(v, v)` is `0`

  - When the starting vertex and ending vertex are the different and there is no
    path:

    - The `agg_cost` in the non included values `(u, v)` is :math:`\infty`

  - If the values returned are stored in a table, the unique index would be the
    pair: `(start_vid, end_vid)`.

  - For **undirected** graphs, the results are **symmetric**.

    - The `agg_cost` of `(u, v)` is the same as for `(v, u)`.

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

   | pgr_withPointsCost(`Edges SQL`_, `Points SQL`_, **start vid**, **end vid**, **driving side** [**options**])
   | pgr_withPointsCost(`Edges SQL`_, `Points SQL`_, **start vid**, **end vids**, **driving side** [**options**])
   | pgr_withPointsCost(`Edges SQL`_, `Points SQL`_, **start vids**, **end vid**, **driving side** [**options**])
   | pgr_withPointsCost(`Edges SQL`_, `Points SQL`_, **start vids**, **end vids**, **driving side** [**options**])
   | pgr_withPointsCost(`Edges SQL`_, `Points SQL`_, `Combinations SQL`_, **driving side** [**options**])
   | **options:** ``[directed]``

   | Returns set of |matrix-result|
   | OR EMPTY SET

.. note:: There is no **details** flag, unlike the other members of the
   withPoints family of functions.

.. index::
    single: withPointsCost ; One to One

One to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_withPointsCost(`Edges SQL`_, `Points SQL`_, **start vid**, **end vid**, **driving side** [**options**])
   | **options:** ``[directed]``

   | Returns set of |matrix-result|
   | OR EMPTY SET

.. rubric:: From point :math:`1` to vertex :math:`10` with right driving side in directed graph.

.. literalinclude:: withPointsCost.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: withPointsCost ; One to Many

One to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_withPointsCost(`Edges SQL`_, `Points SQL`_, **start vid**, **end vids**, **driving side** [**options**])
   | **options:** ``[directed]``

   | Returns set of |matrix-result|
   | OR EMPTY SET

.. rubric:: From point :math:`1` to point :math:`3` and vertex :math:`7` on an
   undirected graph

.. literalinclude:: withPointsCost.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: withPointsCost ; Many to One

Many to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_withPointsCost(`Edges SQL`_, `Points SQL`_, **start vids**, **end vid**, **driving side** [**options**])
   | **options:** ``[directed]``

   | Returns set of |matrix-result|
   | OR EMPTY SET

:Example: From point :math:`1` and vertex :math:`6` to point :math:`3` with right driving side in directed graph.

.. literalinclude:: withPointsCost.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: withPointsCost ; Many to Many

Many to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_withPointsCost(`Edges SQL`_, `Points SQL`_, **start vids**, **end vids**, **driving side** [**options**])
   | **options:** ``[directed]``

   | Returns set of |matrix-result|
   | OR EMPTY SET

:Example: From point :math:`1` and vertex :math:`6` to point :math:`3` and
          vertex :math:`1` with left side driving.

.. literalinclude:: withPointsCost.queries
   :start-after: -- q4
   :end-before: -- q5

.. index::
    single: withPointsCost ; Combinations

Combinations
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_withPointsCost(`Edges SQL`_, `Points SQL`_, `Combinations SQL`_, **driving side** [**options**])
   | **options:** ``[directed]``

   | Returns set of |matrix-result|
   | OR EMPTY SET

:Example: Two combinations

From point :math:`1` to vertex :math:`10`, and from vertex :math:`6` to point
:math:`3` with right side driving.

.. literalinclude:: withPointsCost.queries
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

Result columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_cost_start
    :end-before: return_cost_withPoints_end

Additional Examples
-------------------------------------------------------------------------------

.. contents::
   :local:

Use :doc:`pgr_findCloseEdges` in the `Points SQL`_.
...............................................................................

Find the cost of the routes from vertex :math:`1` to the two closest locations
on the graph of point `(2.9, 1.8)`.

.. literalinclude:: withPointsCost.queries
    :start-after: -- q9
    :end-before: -- q10

* Point :math:`-1` corresponds to the closest edge from point `(2.9, 1.8)`.
* Point :math:`-2` corresponds to the next close edge from point `(2.9, 1.8)`.
* Being close to the graph does not mean have a shorter route.

Right side driving topology
...............................................................................

Traveling from point :math:`1` and vertex :math:`5` to points :math:`\{2, 3,
6\}` and vertices :math:`\{10, 11\}`

.. literalinclude:: withPointsCost.queries
   :start-after: -- q6
   :end-before: -- q7

Left side driving topology
...............................................................................

Traveling from point :math:`1` and vertex :math:`5` to points :math:`\{2, 3,
6\}` and vertices :math:`\{10, 11\}`

.. literalinclude:: withPointsCost.queries
   :start-after: -- q7
   :end-before: -- q8

Does not matter driving side driving topology
...............................................................................

Traveling from point :math:`1` and vertex :math:`5` to points :math:`\{2, 3,
6\}` and vertices :math:`\{10, 11\}`

.. literalinclude:: withPointsCost.queries
   :start-after: -- q8
   :end-before: -- q9


See Also
-------------------------------------------------------------------------------

* :doc:`withPoints-family`
* :doc:`cost-category`
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
