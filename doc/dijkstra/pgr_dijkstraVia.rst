..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: Dijkstra Family ; pgr_dijkstraVia
   single: Via Category ; pgr_dijkstraVia
   single: dijkstraVia

|

``pgr_dijkstraVia``
===============================================================================

``pgr_dijkstraVia`` — Route that goes through a list of vertices.

.. include:: proposed.rst
   :start-after: warning-begin
   :end-before: end-warning

.. rubric:: Availability

.. rubric:: Version 4.0.0

* Function promoted to official.

.. rubric:: Version 2.2.0

* New proposed function.

Description
-------------------------------------------------------------------------------

Given a list of vertices and a graph, this function is equivalent to finding the
shortest path between :math:`vertex_i` and :math:`vertex_{i+1}` for all :math:`i
< size\_of(via\;vertices)`.

:Route: is a sequence of paths.
:Path: is a section of the route.

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. index::
    single: dijkstraVia ; One Via

One Via
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_dijkstraVia(`Edges SQL`_, **via vertices**, [**options**])
   | **options:** ``[directed, strict, U_turn_on_edge]``

   | Returns set of |via-result|
   | OR EMPTY SET

:Example: Find the route that visits the vertices :math:`\{5, 1, 8\}` in that
          order on an directed graph.

.. literalinclude:: dijkstraVia.queries
    :start-after: -- q01
    :end-before: -- q1

Parameters
-------------------------------------------------------------------------------

.. via_parameters_start

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
   * - **via vertices**
     - ``ARRAY`` [ **ANY-INTEGER** ]
     -
     - Array of ordered vertices identifiers that are going to be visited.

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT

.. via_parameters_end

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

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Result columns
-------------------------------------------------------------------------------

.. include:: via-category.rst
    :start-after: result_via_start
    :end-before: result_via_end

Additional Examples
-------------------------------------------------------------------------------

.. contents::
   :local:

All this examples are about the route that visits the vertices :math:`\{5, 7, 1,
8, 15\}` in that order on a **directed** graph.

The main query
...............................................................................

.. literalinclude:: dijkstraVia.queries
    :start-after: -- q1
    :end-before: -- q2

Aggregate cost of the third path.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: dijkstraVia.queries
    :start-after: -- q2
    :end-before: -- q3

Route's aggregate cost of the route at the end of the third path.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: dijkstraVia.queries
    :start-after: -- q3
    :end-before: -- q4

Nodes visited in the route.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: dijkstraVia.queries
    :start-after: -- q4
    :end-before: -- q5

The aggregate costs of the route when the visited vertices are reached.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: dijkstraVia.queries
    :start-after: -- q5
    :end-before: -- q6

Status of "passes in front" or "visits" of the nodes.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: dijkstraVia.queries
    :start-after: -- q6
    :end-before: -- q7

See Also
-------------------------------------------------------------------------------

* :doc:`via-category`.
* :doc:`dijkstra-family`.
* :doc:`sampledata`
* `Boost: Dijkstra shortest paths
  <https://www.boost.org/libs/graph/doc/dijkstra_shortest_paths.html>`__
* https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

