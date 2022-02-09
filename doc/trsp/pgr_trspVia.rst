..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_trspVia.html>`__
  (`3.4 <https://docs.pgrouting.org/3.4/en/pgr_trspVia.html>`__)

pgr_trspVia - Proposed
===============================================================================

``pgr_trspVia`` Via vertices routing with restrictions.

.. include:: proposed.rst
   :start-after: stable-begin-warning
   :end-before: stable-end-warning

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/table_of_contents.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.4.0

  * New **proposed** function ``pgr_trspVia`` (`One Via`_)


Description
-------------------------------------------------------------------------------

Given a list of vertices and a graph, this function is equivalent to finding the shortest path between :math:`vertex_i`
and :math:`vertex_{i+1}` for all :math:`i < size\_of(via\;vertices)` trying not to use restricted paths.

The paths represents the sections of the route.

The general algorithm is as follows:

* Execute a DijkstraVia.
* If a sub-path of solution passes thru a restriction then

  * Execute the TRSP algorithm with restrictions for the sub-path.
  * **NOTE** when this is done, ``U_turn_on_edge`` flag is ignored.


Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. index::
    single: trspVia - Proposed on v3.4

.. parsed-literal::

    pgr_trspVia(`Edges SQL`_, `Restrictions SQL`_, **via vertices** [, directed] [, strict] [, U_turn_on_edge])
    RETURNS SET OF (seq, path_pid, path_seq, start_vid, end_vid, node, edge, cost, agg_cost, route_agg_cost)
    OR EMPTY SET

One Via
...............................................................................

.. parsed-literal::

    pgr_trspVia(`Edges SQL`_, `Restrictions SQL`_, **via vertices** [, directed] [, strict] [, U_turn_on_edge])
    RETURNS SET OF (seq, path_pid, path_seq, start_vid, end_vid,
        node, edge, cost, agg_cost, route_agg_cost)
    OR EMPTY SET

:Example: Find the route that visits the vertices :math:`\{ 1, 7, 10\}` in that order on an **directed** graph.

.. literalinclude:: trspVia.queries
    :start-after: -- q0
    :end-before: -- q1

.. include:: pgRouting-concepts.rst
    :start-after: pgr_dijkstra_via_parameters_start
    :end-before: pgr_dijkstra_via_parameters_end

Inner query
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Restrictions SQL
...............................................................................

.. include:: TRSP-family.rst
   :start-after: restrictions_columns_start
   :end-before: restrictions_columns_end

Return Columns
-------------------------------------------------------------------------------

.. include:: pgr_dijkstraVia.rst
   :start-after: via result columns start
   :end-before: via result columns end

Additional Examples
-------------------------------------------------------------------------------

:Example 1: Find the route that visits the vertices :math:`\{1, 5, 7, 10, 4\}` in that order

.. literalinclude:: trspVia.queries
    :start-after: -- q1
    :end-before: -- q2

:Example 2: What's the aggregate cost of the third path?

.. literalinclude:: trspVia.queries
    :start-after: -- q2
    :end-before: -- q3

:Example 3: What's the route's aggregate cost of the route at the end of the third path?

.. literalinclude:: trspVia.queries
    :start-after: -- q3
    :end-before: -- q4

:Example 4: How are the nodes visited in the route?

.. literalinclude:: trspVia.queries
    :start-after: -- q4
    :end-before: -- q5

:Example 5: What are the aggregate costs of the route when the visited vertices are reached?

.. literalinclude:: trspVia.queries
    :start-after: -- q5
    :end-before: -- q6

:Example 6: Show the route's seq and aggregate cost and a status of "passes in front" or "visits" node :math:`9`

.. literalinclude:: trspVia.queries
    :start-after: -- q6
    :end-before: -- q7

:Example 7: When original sub-path passes thru a restriction, ``U_turn_on_edge`` flag is ignored.
:Example 8: Simulation of how algorithm works

.. literalinclude:: trspVia.queries
    :start-after: -- q7
    :end-before: -- q8

See Also
-------------------------------------------------------------------------------

* :doc:`pgr_dijkstraVia`
* :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

