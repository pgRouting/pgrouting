..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_withPointsCost.html>`__
  (`3.3 <https://docs.pgrouting.org/3.3/en/pgr_withPointsCost.html>`__)
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_withPointsCost.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_withPointsCost.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_withPointsCost.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_withPointsCost.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_withPointsCost.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_withPointsCost.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/withPoints/doc/pgr_withPointsCost.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/withPoints/doc/pgr_withPointsCost.html>`__

``pgr_withPointsCost`` - Proposed
===============================================================================

``pgr_withPointsCost`` - Calculates the shortest path and returns only the
aggregate cost of the shortest path(s) found, for the combination of points
given.

.. include:: proposed.rst
   :start-after: begin-warning
   :end-before: end-warning

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/table_of_contents.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.2.0

  * New **proposed** function:

    * pgr_withPointsCost(Combinations)

* Version 2.2.0

  * New **proposed** function


Description
-------------------------------------------------------------------------------

Modify the graph to include points defined by points_sql.
Using Dijkstra algorithm, return only the aggregate cost of the shortest path(s) found.

The main characteristics are:
  - It does not return a path.
  - Returns the sum of the costs of the shortest path for pair combination of vertices in the modified graph.
  - Vertices of the graph are:

    - **positive** when it belongs to the edges_sql
    - **negative** when it belongs to the points_sql

  - Process is done only on edges with positive costs.
  - Values are returned when there is a path.

    - The returned values are in the form of a set of `(start_vid, end_vid, agg_cost)`.

    - When the starting vertex and ending vertex are the same, there is no path.

      - The `agg_cost` in the non included values `(v, v)` is `0`

    - When the starting vertex and ending vertex are the different and there is no path.

      - The `agg_cost` in the non included values `(u, v)` is :math:`\infty`

  - If the values returned are stored in a table, the unique index would be the pair:
    `(start_vid, end_vid)`.

  - For **undirected** graphs, the results are **symmetric**.

    - The  `agg_cost` of `(u, v)` is the same as for `(v, u)`.

  - For optimization purposes, any duplicated value in the `start_vids` or `end_vids` is ignored.

  - The returned values are ordered:

    - `start_vid` ascending
    - `end_vid` ascending

  - Running time: :math:`O(| start\_vids | * (V \log V + E))`

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. parsed-literal::

    pgr_withPointsCost(`Edges SQL`_, **start vid**, **end vid**  [, directed] [, driving_side])
    pgr_withPointsCost(`Edges SQL`_, **start vid**, **end vids** [, directed] [, driving_side])
    pgr_withPointsCost(`Edges SQL`_, **start vids**, **end vid**  [, directed] [, driving_side])
    pgr_withPointsCost(`Edges SQL`_, **start vids**, **end vids** [, directed] [, driving_side])
    pgr_withPointsCost(`Edges SQL`_, `Combinations SQL`_ [, directed] [, driving_side])
    RETURNS SET OF (start_vid, end_vid, agg_cost)

.. note:: There is no **details** flag, unlike the other members of the
   withPoints family of functions.

.. index::
    single: withPointsCost(One To One) - Proposed on v2.2

One to One
...............................................................................

.. parsed-literal::

    pgr_withPointsCost(`Edges SQL`_, **start vid**, **end vid**  [, directed] [, driving_side])
    RETURNS SET OF (start_vid, end_vid, agg_cost)

:Example: From point :math:`1` to vertex :math:`3` with defaults

.. literalinclude:: doc-pgr_withPointsCost.queries
   :start-after: --e2
   :end-before: --e3

.. index::
    single: withPointsCost(One To Many) - Proposed on v2.2

One to Many
...............................................................................

.. parsed-literal::

    pgr_withPointsCost(`Edges SQL`_, **start vid**, **end vids** [, directed] [, driving_side])
    RETURNS SET OF (start_vid, end_vid, agg_cost)

:Example: From point :math:`1` to point :math:`3` and vertex :math:`5` on an
          undirected graph

.. literalinclude:: doc-pgr_withPointsCost.queries
   :start-after: --e3
   :end-before: --e4

.. index::
    single: withPointsCost(Many To One) - Proposed on v2.2

Many to One
...............................................................................

.. parsed-literal::

    pgr_withPointsCost(`Edges SQL`_, **start vids**, **end vid**  [, directed] [, driving_side])
    RETURNS SET OF (start_vid, end_vid, agg_cost)

:Example: From point :math:`1` and vertex :math:`2` to point :math:`3`

.. literalinclude:: doc-pgr_withPointsCost.queries
   :start-after: --e4
   :end-before: --e5

.. index::
    single: withPointsCost(Many To Many) - Proposed on v2.2

Many to Many
...............................................................................

.. parsed-literal::

    pgr_withPointsCost(`Edges SQL`_, **start vids**, **end vids** [, directed] [, driving_side])
    RETURNS SET OF (start_vid, end_vid, agg_cost)

:Example: From point :math:`1` and vertex :math:`2`  to point :math:`3` and
          vertex :math:`7`

.. literalinclude:: doc-pgr_withPointsCost.queries
   :start-after: --e5
   :end-before: --e6

.. index::
    single: withPointsCost(Combinations) -- Proposed on v3.2

Combinations
...............................................................................

.. parsed-literal::

    pgr_withPointsCost(`Edges SQL`_, `Combinations SQL`_ [, directed] [, driving_side])
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)

:Example: Two combinations

From point :math:`1` to vertex :math:`3`, and from vertex :math:`2` to point
:math:`3` with **right** side driving.

.. literalinclude:: doc-pgr_withPointsCost.queries
   :start-after: --e6
   :end-before: --e7

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

.. list-table::
   :width: 81
   :widths: 14 7 7 60
   :header-rows: 1

   * - Parameter
     - Type
     - Default
     - Description
   * - ``driving_side``
     - ``CHAR``
     - ``b``
     - Value in [``r``, ``l``, ``b``] indicating if the driving side is:

       - ``r`` for right driving side.
       - ``l`` for left driving side.
       - ``b`` for both.

Inner queries
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

Return Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_cost_withPoints_start
    :end-before: return_cost_withPoints_end

Additional Examples
-------------------------------------------------------------------------------

:Example: **Right** side driving topology

Traveling from point 1 and vertex 1 to points :math:`\{2, 3, 6\}` and vertices
:math:`\{3, 6\}`

.. literalinclude:: doc-pgr_withPointsCost.queries
   :start-after: --q1
   :end-before: --q2

:Example: **Left** side driving topology

Traveling from point 1 and vertex 1 to points :math:`\{2, 3, 6\}` and vertices
:math:`\{3, 6\}`

.. literalinclude:: doc-pgr_withPointsCost.queries
   :start-after: --q2
   :end-before: --q3

:Example: Does not matter driving side driving topology

Traveling from point 1 and vertex 1 to points :math:`\{2, 3, 6\}` and vertices
:math:`\{3, 6\}`

.. literalinclude:: doc-pgr_withPointsCost.queries
   :start-after: --q3
   :end-before: --q4

The queries use the :doc:`sampledata` network.

See Also
-------------------------------------------------------------------------------

* :doc:`withPoints-family`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

