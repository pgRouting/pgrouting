..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_withPointsVia.html>`__
  (`3.4 <https://docs.pgrouting.org/3.4/en/pgr_withPointsVia.html>`__)

``pgr_withPointsVia`` - Proposed
===============================================================================

``pgr_withPointsVia`` Via points/vertices routing.

.. include:: proposed.rst
   :start-after: stable-begin-warning
   :end-before: stable-end-warning

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/table_of_contents.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.4.0

  * New **proposed** function ``pgr_withPointsVia`` (`One Via`_)


|

Description
-------------------------------------------------------------------------------

Given a list of vertices and a graph, this function is equivalent to finding the
shortest path between :math:`vertex_i` and :math:`vertex_{i+1}` for all :math:`i
< size\_of(via\;vertices)` when vertices can be points.

The paths represents the sections of the route.

The general algorithm is as follows:

* Add the additional points to the graph
* Execute a :doc:`pgr_dijkstraVia`.

|

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. index::
    single: withPointsVia - Proposed on v3.4

.. parsed-literal::

    pgr_withPointsVia(`Edges SQL`_, `Points SQL`_, **via vertices**
               [, directed] [, strict] [, U_turn_on_edge]) -- Proposed on v3.4
    RETURNS SET OF (seq, path_pid, path_seq, start_vid, end_vid,
                    node, edge, cost, agg_cost, route_agg_cost)
    OR EMPTY SET

|

One Via
...............................................................................

.. parsed-literal::

    pgr_withPointsVia(`Edges SQL`_, `Points SQL`_, **via vertices**
               [, directed] [, strict] [, U_turn_on_edge]) -- Proposed on v3.4
    RETURNS SET OF (seq, path_pid, path_seq, start_vid, end_vid,
                    node, edge, cost, agg_cost, route_agg_cost)
    OR EMPTY SET

:Example: Find the route that visits the vertices :math:`\{ -1, -3, 9\}` in that
          order on an **undirected** graph.

.. literalinclude:: withPointsVia.queries
    :start-after: -- q0
    :end-before: -- q1

|

Parameters
-------------------------------------------------------------------------------

.. list-table::
   :width: 81
   :widths: 14 20 7 40
   :header-rows: 1

   * - Parameter
     - Type
     - Default
     - Description
   * - `Edges SQL`_
     - ``TEXT``
     -
     - SQL query as described.
   * - `Points SQL`_
     - ``TEXT``
     -
     - SQL query as described.
   * - **via vertices**
     - ``ARRAY[`` **ANY-INTEGER** ``]``
     -
     - Array of ordered vertices identifiers that are going to be visited.

       * When positive it is considered a vertex identifier
       * When negative it is considered a point identifier
   * - ``directed``
     - ``BOOLEAN``
     - ``true``
     - - When ``true`` Graph is considered `Directed`
       - When ``false`` the graph is considered as Undirected.

|

Via optional parameters
...............................................................................

.. include:: via-category.rst
    :start-after: via_opt_parameters_start
    :end-before: via_opt_parameters_end

|

With points optional parameters
...............................................................................

.. include:: via-category.rst
    :start-after: withPoints_parameters_start
    :end-before: withPoints_parameters_end

|

Inner query
-------------------------------------------------------------------------------

|

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

|

Points SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: points_sql_start
    :end-before: points_sql_end

|

Return Columns
-------------------------------------------------------------------------------

.. include:: via-category.rst
    :start-after: result columns start
    :end-before: result columns end

.. Note:: When ``start_vid`` or ``end_vid`` is negative, then it is a point.

|

Additional Examples
-------------------------------------------------------------------------------

:Example 1: Find the route that visits the vertices :math:`\{-1, 5, -3, 9, 4\}`
            in that order on a **directed** graph

.. literalinclude:: withPointsVia.queries
    :start-after: -- q1
    :end-before: -- q2

:Example 2: What's the aggregate cost of the third path?

.. literalinclude:: withPointsVia.queries
    :start-after: -- q2
    :end-before: -- q3

:Example 3: What's the route's aggregate cost of the route at the end of the
            third path?

.. literalinclude:: withPointsVia.queries
    :start-after: -- q3
    :end-before: -- q4

:Example 4: How are the nodes visited in the route?

.. literalinclude:: withPointsVia.queries
    :start-after: -- q4
    :end-before: -- q5

:Example 5: What are the aggregate costs of the route when the visited vertices
            are reached?

.. literalinclude:: withPointsVia.queries
    :start-after: -- q5
    :end-before: -- q6

:Example 6: Show a status of "passes in front" or "visits" of the nodes and
            points.

.. literalinclude:: withPointsVia.queries
    :start-after: -- q6
    :end-before: -- q7

See Also
-------------------------------------------------------------------------------

* :doc:`via-category`
* :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

