..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: Turn Restriction Shortest Path Family ; pgr_trspVia
   single: Via Category ; pgr_trspVia
   single: Restrictions Category ; pgr_trspVia
   single: trspVia

|

``pgr_trspVia``
===============================================================================

``pgr_trspVia`` Route that goes through a list of vertices with restrictions.

.. rubric:: Availability

.. rubric:: Version 4.0.0

* Function promoted to official.
* Breaking change, signatures no longer available:

  * pgr_trspviavertices(text,anyarray,boolean,boolean,text)

.. rubric:: Version 3.4.0

* New proposed function.
* Deprecated signatures

  * pgr_trspviavertices(text,anyarray,boolean,boolean,text)

Description
-------------------------------------------------------------------------------

Given a list of vertices and a graph, this function is equivalent to finding the
shortest path between :math:`vertex_i` and :math:`vertex_{i+1}` for all :math:`i
< size\_of(via\;vertices)` trying not to use restricted paths.

The paths represents the sections of the route.

The general algorithm is as follows:

* Execute a :doc:`pgr_dijkstraVia`.
* For the set of sub paths of the solution that pass through a restriction then

  * Execute the **TRSP** algorithm with restrictions for the paths.
  * **NOTE** when this is done, ``U_turn_on_edge`` flag is ignored.

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. index::
    single: trspVia ; One Via

One Via
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_trspVia(`Edges SQL`_, `Restrictions SQL`_, **via vertices**, [**options**])
   | **options:** ``[directed, strict, U_turn_on_edge]``

   | Returns set of |via-result|
   | OR EMPTY SET

:Example: Find the route that visits the vertices :math:`\{5, 1, 8\}` in that
          order on an directed graph.

.. literalinclude:: trspVia.queries
    :start-after: -- q0
    :end-before: -- q1

Parameters
-------------------------------------------------------------------------------

.. include:: TRSP-family.rst
    :start-after: via_parameters_start
    :end-before: via_parameters_end

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

Restrictions SQL
...............................................................................

.. include:: pgRouting-concepts.rst
   :start-after: restrictions_columns_start
   :end-before: restrictions_columns_end

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
8, 15\}` in that order on a directed graph.

The main query
...............................................................................

.. literalinclude:: trspVia.queries
    :start-after: -- q1
    :end-before: -- q2

Aggregate cost of the third path.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: trspVia.queries
    :start-after: -- q2
    :end-before: -- q3

Route's aggregate cost of the route at the end of the third path.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: trspVia.queries
    :start-after: -- q3
    :end-before: -- q4

Nodes visited in the route.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: trspVia.queries
    :start-after: -- q4
    :end-before: -- q5

The aggregate costs of the route when the visited vertices are reached.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: trspVia.queries
    :start-after: -- q5
    :end-before: -- q6

Status of "passes in front" or "visits" of the nodes.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: trspVia.queries
    :start-after: -- q6
    :end-before: -- q7

Simulation of how algorithm works.
...............................................................................

The algorithm performs a :doc:`pgr_dijkstraVia`

.. literalinclude:: trspVia.queries
    :start-after: -- q7
    :end-before: -- q8

Detects which of the sub paths pass through a restriction in this case is for
the ``path_id = 5`` from ``6`` to ``3`` because the path :math:`15 \rightarrow 1`
is restricted.

Executes the :doc:`pgr_trsp` algorithm for the conflicting paths.

.. literalinclude:: trspVia.queries
    :start-after: -- q8
    :end-before: -- q9

From the :doc:`pgr_dijkstraVia` result it removes the conflicting paths and
builds the solution with the results of the :doc:`pgr_trsp` algorithm:

.. literalinclude:: trspVia.queries
    :start-after: -- q9
    :end-before: -- q10

Getting the same result as ``pgr_trspVia``:

.. literalinclude:: trspVia.queries
    :start-after: -- q10
    :end-before: -- q11

:Example 8: Sometimes ``U_turn_on_edge`` flag is ignored when is set to
            ``false``.

The first step, doing a :doc:`pgr_dijkstraVia` does consider not making a U turn
on the same edge. But the path :math:`16 \rightarrow 13` (Rows 4 and 5) is
restricted and the result is using it.

.. literalinclude:: trspVia.queries
    :start-after: -- q11
    :end-before: -- q12

When executing the :doc:`pgr_trsp` algorithm for the conflicting path, there is
no ``U_turn_on_edge`` flag.

.. literalinclude:: trspVia.queries
    :start-after: -- q12
    :end-before: -- q13

Therefore the result ignores the ``U_turn_on_edge`` flag when set to ``false``.

.. literalinclude:: trspVia.queries
    :start-after: -- q13
    :end-before: -- q14

See Also
-------------------------------------------------------------------------------

* :doc:`via-category`
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

