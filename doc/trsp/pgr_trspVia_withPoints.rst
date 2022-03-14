..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_trspVia_withPoints.html>`__
  (`3.4 <https://docs.pgrouting.org/3.4/en/pgr_trspVia_withPoints.html>`__)

``pgr_trspVia_withPoints`` - Proposed
===============================================================================

``pgr_trspVia_withPoints`` Via vertices/points routing with restrictions.

.. include:: proposed.rst
   :start-after: stable-begin-warning
   :end-before: stable-end-warning

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/table_of_contents.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.4.0

  * New **proposed** function ``pgr_trspVia_withPoints`` (`One Via`_)

|

Description
-------------------------------------------------------------------------------

Given a graph, a set of restriction on the graph edges, a set of points on the
graphs edges and a list of vertices, this function is equivalent to finding the
shortest path between :math:`vertex_i` and :math:`vertex_{i+1}` (where
:math:`vertex` can be a vertex or a point on the graph) for all :math:`i <
size\_of(via\;vertices)` trying not to use restricted paths.

:Route: is a sequence of paths
:Path: is a section of the route.

The general algorithm is as follows:

* Build the Graph with the new points.

  * The points identifiers will be converted to negative values.
  * The vertices identifiers will remain positive.

* Execute a :doc:`pgr_withPointsVia`.
* For the set of paths of the solution that pass through a restriction then

  * Execute the **TRSP** algorithm with restrictions for the path.
  * **NOTE** when this is done, ``U_turn_on_edge`` flag is ignored.

.. Note:: Do not use negative values on identifiers of the inner queries.

|

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. index::
    single: trspVia - Proposed on v3.4

.. parsed-literal::

    pgr_trspVia_withPoints(`Edges SQL`_, `Restrictions SQL`_, `Points SQL`_, **via vertices**
               [, directed] [, strict] [, U_turn_on_edge]) -- Proposed on v3.4
    RETURNS SET OF (seq, path_pid, path_seq, start_vid, end_vid,
                    node, edge, cost, agg_cost, route_agg_cost)
    OR EMPTY SET

|

One Via
...............................................................................

.. parsed-literal::

    pgr_trspVia_withPoints(`Edges SQL`_, `Restrictions SQL`_, `Points SQL`_, **via vertices**
               [, directed] [, strict] [, U_turn_on_edge]) -- Proposed on v3.4
    RETURNS SET OF (seq, path_pid, path_seq, start_vid, end_vid,
                    node, edge, cost, agg_cost, route_agg_cost)
    OR EMPTY SET

:Example: Find the route that visits the vertices :math:`\{ -6, 4, -5\}` in that
          order on an **directed** graph.

.. literalinclude:: trspVia_withPoints.queries
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
   * - `Restrictions SQL`_
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

.. include:: pgr_trsp_withPoints.rst
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

Restrictions SQL
...............................................................................

.. include:: TRSP-family.rst
   :start-after: restrictions_columns_start
   :end-before: restrictions_columns_end

|

Points SQL
...............................................................................

.. include:: withPoints-category.rst
    :start-after: points_sql_start
    :end-before: points_sql_end

|

Return Columns
-------------------------------------------------------------------------------

.. include:: via-category.rst
    :start-after: result columns start
    :end-before: result columns end

|

Additional Examples
-------------------------------------------------------------------------------

:Example 1: Find the route that visits the vertices :math:`\{1, 5, 7, 10, 4\}`
            in that order

.. literalinclude:: trspVia_withPoints.queries
    :start-after: -- q1
    :end-before: -- q2

:Example 2: What's the aggregate cost of the third path?

.. literalinclude:: trspVia_withPoints.queries
    :start-after: -- q2
    :end-before: -- q3

:Example 3: What's the route's aggregate cost of the route at the end of the
            third path?

.. literalinclude:: trspVia_withPoints.queries
    :start-after: -- q3
    :end-before: -- q4

:Example 4: How are the nodes visited in the route?

.. literalinclude:: trspVia_withPoints.queries
    :start-after: -- q4
    :end-before: -- q5

:Example 5: What are the aggregate costs of the route when the visited vertices
            are reached?

.. literalinclude:: trspVia_withPoints.queries
    :start-after: -- q5
    :end-before: -- q6

:Example 6: Show the route's seq and aggregate cost and a status of "passes in
            front" or "visits"

.. literalinclude:: trspVia_withPoints.queries
    :start-after: -- q6
    :end-before: -- q7

:Example 7: Simulation of how algorithm works

The algorithm performs a :doc:`pgr_withPointsVia`

.. literalinclude:: trspVia_withPoints.queries
    :start-after: -- q7
    :end-before: -- q8

Detects which of the paths pass through a restriction in this case is for the
``path_id = 1`` from ``-6`` to ``4`` because the path :math:`9 \rightarrow 16`
is restricted.

Executes the :ref:`TRSP-family:TRSP algorithm` for the conflicting paths.

.. literalinclude:: trspVia_withPoints.queries
    :start-after: -- q8
    :end-before: -- q9

From the :doc:`pgr_dijkstraVia` result it removes the conflicting paths and
builds the solution with the results of the :doc:`pgr_trsp` algorithm:

.. literalinclude:: trspVia_withPoints.queries
    :start-after: -- q9
    :end-before: -- q10

Getting the same result as ``pgr_trspVia_withPoints``:

.. literalinclude:: trspVia_withPoints.queries
    :start-after: -- q10
    :end-before: -- q11

:Example 8: Sometimes ``U_turn_on_edge`` flag is ignored when is set to
            ``false``.

The first step, doing a :doc:`pgr_dijkstraVia` does consider not making a U turn
on the same edge. But the path :math:`9 \rightarrow 16` (Rows 4 and 5) is
restricted and the result is using it.

.. literalinclude:: trspVia_withPoints.queries
    :start-after: -- q11
    :end-before: -- q12

When executing the :doc:`pgr_trsp_withPoints` algorithm for the conflicting
path, there is no ``U_turn_on_edge`` flag.

.. literalinclude:: trspVia_withPoints.queries
    :start-after: -- q12
    :end-before: -- q13

Therefore the result ignores the ``U_turn_on_edge`` flag when set to ``false``.

.. literalinclude:: trspVia_withPoints.queries
    :start-after: -- q13
    :end-before: -- q14

See Also
-------------------------------------------------------------------------------

* :doc:`TRSP-family`
* :doc:`via-category`
* :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

