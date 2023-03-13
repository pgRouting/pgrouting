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
  (`3.6 <https://docs.pgrouting.org/3.6/en/pgr_withPointsVia.html>`__)
  `3.5 <https://docs.pgrouting.org/3.5/en/pgr_withPointsVia.html>`__
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_withPointsVia.html>`__

``pgr_withPointsVia`` - Proposed
===============================================================================

``pgr_withPointsVia`` - Route that goes through a list of vertices and/or
points.

.. include:: proposed.rst
   :start-after: stable-begin-warning
   :end-before: stable-end-warning

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/table_of_contents.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.4.0

  * New **proposed** function ``pgr_withPointsVia`` (`One Via`_)

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

Signatures
-------------------------------------------------------------------------------

.. index::
    single: withPointsVia - Proposed on v3.4

One Via
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_withPointsVia(`Edges SQL`_, `Points SQL`_, **via vertices**, [**options**])
   | **options:** ``[directed, strict, U_turn_on_edge]``

   | RETURNS SET OF |via-result|
   | OR EMPTY SET

:Example: Find the route that visits the vertices :math:`\{ -6, 15, -1\}` in
          that order on a **directed** graph.

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

Use :doc:`pgr_findCloseEdges` in the `Points SQL`_
...............................................................................

Visit from vertex :math:`1` to the two locations on the graph of point `(2.9,
1.8)` in order of closeness to the graph.

.. literalinclude:: withPointsVia.queries
    :start-after: -- q7
    :end-before: -- q8

* Point :math:`-1` corresponds to the closest edge from point `(2.9,1.8)`.
* Point :math:`-2` corresponds to the next close edge from point `(2.9,1.8)`.
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
* :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

