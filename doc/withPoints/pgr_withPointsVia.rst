..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: withPoints Family ; pgr_withPointsVia
   single: With Points Category ; pgr_withPointsVia
   single: Via Category ; pgr_withPointsVia
   single: withPointsVia

|

``pgr_withPointsVia``
===============================================================================

``pgr_withPointsVia`` - Route that goes through a list of vertices and/or
points.

.. rubric:: Availability

.. rubric:: Version 4.0.0

* Function promoted to official.
* **Driving side** parameter is positional unnamed and compulsory.

  * Valid values depend on kind of graph

.. Breaking change

* Breaking change, signatures no longer available:

  * pgr_withpointsvia(text,text,anyarray,boolean,boolean,boolean,character,boolean)

.. rubric:: Version 3.4.0

* New proposed function.

Description
-------------------------------------------------------------------------------

Given a graph, a set of points on the graphs edges and a list of vertices, this
function is equivalent to finding the shortest path between :math:`vertex_i` and
:math:`vertex_{i+1}` (where :math:`vertex` can be a vertex or a point on the
graph) for all :math:`i < size\_of(via\;vertices)`.

:Route: is a sequence of paths.
:Path: is a section of the route.

The general algorithm is as follows:

* Build the Graph with the new points.

  * The points identifiers will be converted to negative values.
  * The vertices identifiers will remain positive.

* Execute a :doc:`pgr_dijkstraVia`.

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. index::
    single: withPointsVia ; One Via

One Via
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_withPointsVia(`Edges SQL`_, `Points SQL`_, **via vertices**, **driving side** [**options**])
   | **options:** ``[directed, strict, U_turn_on_edge, details]``

   | Returns set of |via-result|
   | OR EMPTY SET

:Example: Find the route that visits the vertices :math:`\{ -6, 15, -1\}` in
          that order with right driving side in directed graph..

.. literalinclude:: withPointsVia.queries
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

Points SQL
...............................................................................

.. include:: withPoints-category.rst
    :start-after: points_sql_start
    :end-before: points_sql_end

Result columns
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

Use :doc:`pgr_findCloseEdges` in the `Points SQL`_
...............................................................................

Visit from vertex :math:`1` to the two locations on the graph of point `(2.9,
1.8)` in order of closeness to the graph.

.. literalinclude:: withPointsVia.queries
    :start-after: -- q7
    :end-before: -- q8

* Point :math:`-1` corresponds to the closest edge from point `(2.9, 1.8)`.
* Point :math:`-2` corresponds to the next close edge from point `(2.9, 1.8)`.
* Point :math:`-2` is visited on the route to from vertex :math:`1` to Point
  :math:`-1` (See row where :math:`seq = 4`).

Usage variations
...............................................................................

All this examples are about the route that visits the vertices :math:`\{-1, 7,
-3, 16, 15\}` in that order on a **directed** graph.

.. literalinclude:: withPointsVia.queries
    :start-after: -- q1
    :end-before: -- q2

Aggregate cost of the third path.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: withPointsVia.queries
    :start-after: -- q2
    :end-before: -- q3

Route's aggregate cost of the route at the end of the third path.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: withPointsVia.queries
    :start-after: -- q3
    :end-before: -- q4

Nodes visited in the route.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: withPointsVia.queries
    :start-after: -- q4
    :end-before: -- q5

The aggregate costs of the route when the visited vertices are reached.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: withPointsVia.queries
    :start-after: -- q5
    :end-before: -- q6

Status of "passes in front" or "visits" of the nodes and points.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: withPointsVia.queries
    :start-after: -- q6
    :end-before: -- q7

See Also
-------------------------------------------------------------------------------

* :doc:`withPoints-family`
* :doc:`via-category`
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

