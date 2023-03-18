..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


``pgr_trspVia_withPoints`` - Proposed
===============================================================================

``pgr_trspVia_withPoints`` - Route that goes through a list of vertices and/or
points with restrictions.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/table_of_contents.html

   Boost Graph Inside

.. include:: proposed.rst
   :start-after: stable-begin-warning
   :end-before: stable-end-warning

.. rubric:: Availability

* Version 3.4.0

  * New proposed function:

    ``pgr_trspVia_withPoints`` (`One Via`_)

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

Signatures
-------------------------------------------------------------------------------

.. index::
    single: trspVia_withPoints - Proposed on v3.4

One Via
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_trspVia_withPoints(`Edges SQL`_, `Restrictions SQL`_, `Points SQL`_, **via vertices**, [**options**])
   | **options:** ``[directed, strict, U_turn_on_edge]``

   | RETURNS SET OF |via-result|
   | OR EMPTY SET

:Example: Find the route that visits the vertices :math:`\{-6, 15, -5\}` in that
          order on an directed graph.

.. literalinclude:: trspVia_withPoints.queries
    :start-after: -- q0
    :end-before: -- q1

Parameters
-------------------------------------------------------------------------------

.. include:: via-category.rst
    :start-after: via_withPoints_parameters_start
    :end-before: via_withPoints_parameters_end

Optional parameters
...............................................................................

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

Via optional parameters
...............................................................................

.. include:: via-category.rst
    :start-after: via_optionals_start
    :end-before: via_optionals_end

With points optional parameters
...............................................................................

.. include:: pgr_trsp_withPoints.rst
    :start-after: withPoints_parameters_start
    :end-before: withPoints_parameters_end

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

Result Columns
-------------------------------------------------------------------------------

.. include:: via-category.rst
    :start-after: result_via_start
    :end-before: result_via_end

.. include:: via-category.rst
    :start-after: result_via_withPoints_start
    :end-before: result_via_withPoints_end

Additional Examples
-------------------------------------------------------------------------------

.. contents::
   :local:

Use ``pgr_findCloseEdges`` for points on the fly
...............................................................................

Using :doc:`pgr_findCloseEdges`:

Visit from vertex :math:`1` to the two locations on the graph of point `(2.9,
1.8)` in order of closeness to the graph.

.. literalinclude:: trspVia_withPoints.queries
    :start-after: -- q14
    :end-before: -- q15

* Point :math:`-1` corresponds to the closest edge from point `(2.9,1.8)`.
* Point :math:`-2` corresponds to the next close edge from point `(2.9,1.8)`.
* Point :math:`-2` is visited on the route to from vertex :math:`1` to Point
  :math:`-1` (See row where :math:`seq = 4`).

Usage variations
...............................................................................

All this examples are about the route that visits the vertices :math:`\{-6, 7,
-4, 8, -2\}` in that order on a directed graph.

.. literalinclude:: trspVia_withPoints.queries
    :start-after: -- q1
    :end-before: -- q2

Aggregate cost of the third path.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: trspVia_withPoints.queries
    :start-after: -- q2
    :end-before: -- q3

Route's aggregate cost of the route at the end of the third path.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: trspVia_withPoints.queries
    :start-after: -- q3
    :end-before: -- q4

Nodes visited in the route.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: trspVia_withPoints.queries
    :start-after: -- q4
    :end-before: -- q5

The aggregate costs of the route when the visited vertices are reached.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: trspVia_withPoints.queries
    :start-after: -- q5
    :end-before: -- q6

Status of "passes in front" or "visits" of the nodes and points.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: trspVia_withPoints.queries
    :start-after: -- q6
    :end-before: -- q7

Simulation of how algorithm works.
...............................................................................

The algorithm performs a :doc:`pgr_withPointsVia`

.. literalinclude:: trspVia_withPoints.queries
    :start-after: -- q7
    :end-before: -- q8

Detects which of the paths pass through a restriction in this case is for the
``path_id = 1`` from ``-6`` to ``15`` because the path :math:`9 \rightarrow 16`
is restricted.

Executes the :ref:`TRSP-family:TRSP algorithm` for the conflicting paths.

.. literalinclude:: trspVia_withPoints.queries
    :start-after: -- q8
    :end-before: -- q9

From the :doc:`pgr_withPointsVia` result it removes the conflicting paths and
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

The first step, doing a :doc:`pgr_withPointsVia` does consider not making a U
turn on the same edge. But the path :math:`9 \rightarrow 16` (Rows 4 and 5) is
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
From the :doc:`pgr_withPointsVia` result it removes the conflicting paths and
builds the solution with the results of the :doc:`pgr_trsp` algorithm.
In this case a U turn is been done using the same edge.

.. literalinclude:: trspVia_withPoints.queries
    :start-after: -- q13
    :end-before: -- q14

See Also
-------------------------------------------------------------------------------

* :doc:`TRSP-family`
* :doc:`via-category`
* :doc:`withPoints-category`
* :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

