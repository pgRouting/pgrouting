..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_trsp - Turn Restriction Shortest Path (TRSP)
===============================================================================

.. index::
	single: trsp(text,integer,integer,boolean,boolean)
	single: trsp(text,integer,integer,boolean,boolean,text)
	single: trspViaVertices - Prototype
	single: trspViaEdges - Prototype

``pgr_trsp`` — Returns the shortest path with support for turn restrictions.

.. rubric:: Availability

* Version 2.1.0

  * New *Via* **prototypes**

    * pgr_trspViaVertices
    * pgr_trspViaEdges

* Version 2.0.0

  * **Official** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_trsp.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_trsp.html>`__

* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_trsp.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_trsp.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_trsp.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/trsp/doc/pgr_trsp.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/trsp/doc/pgr_trsp.html>`__
  `2.1 <https://docs.pgrouting.org/2.1/en/src/trsp/doc/index.html>`__
  `2.0 <https://docs.pgrouting.org/2.0/en/src/trsp/doc/index.html>`__


Description
-------------------------------------------------------------------------------

The turn restricted shorthest path (TRSP) is a shortest path algorithm that can optionally take into account complicated turn restrictions like those found in real world navigable road networks. Performamnce wise it is nearly as fast as the A* search but has many additional features like it works with edges rather than the nodes of the network. Returns a set of (seq, id1, id2, cost) or (seq, id1, id2, id3, cost) rows, that make up a path.

.. code-block:: sql

  pgr_trsp(sql text, source integer, target integer,
                    directed boolean, has_rcost boolean [,restrict_sql text]);
  RETURNS SETOF (seq, id1, id2, cost)


.. code-block:: sql

  pgr_trsp(sql text, source_edge integer, source_pos float8,
	                target_edge integer, target_pos float8,
                    directed boolean, has_rcost boolean [,restrict_sql text]);
  RETURNS SETOF (seq, id1, id2, cost)

.. code-block:: sql

  pgr_trspViaVertices(sql text, vids integer[],
                    directed boolean, has_rcost boolean
                    [, turn_restrict_sql text]);
  RETURNS SETOF (seq, id1, id2, id3, cost)

.. code-block:: sql

  pgr_trspViaEdges(sql text, eids integer[], pcts float8[],
                    directed boolean, has_rcost boolean
                    [, turn_restrict_sql text]);
  RETURNS SETOF (seq, id1, id2, id3, cost)

**The main characteristics are:**

The Turn Restricted Shortest Path algorithm (TRSP) is similar to the shooting star in that you can specify turn restrictions.

The TRSP setup is mostly the same as :doc:`Dijkstra shortest path <pgr_dijkstra>` with the addition of an optional turn restriction table. This provides an easy way of adding turn restrictions to a road network by placing them in a separate table.


:sql: a SQL query, which should return a set of rows with the following columns:

	.. code-block:: sql

		SELECT id, source, target, cost, [,reverse_cost] FROM edge_table


	:id: ``int4`` identifier of the edge
	:source: ``int4`` identifier of the source vertex
	:target: ``int4`` identifier of the target vertex
	:cost: ``float8`` value, of the edge traversal cost. A negative cost will prevent the edge from being inserted in the graph.
	:reverse_cost: (optional) the cost for the reverse traversal of the edge. This is only used when the ``directed`` and ``has_rcost`` parameters are ``true`` (see the above remark about negative costs).

:source: ``int4`` **NODE id** of the start point
:target: ``int4`` **NODE id** of the end point
:directed: ``true`` if the graph is directed
:has_rcost: if ``true``, the ``reverse_cost`` column of the SQL generated set of rows will be used for the cost of the traversal of the edge in the opposite direction.

:restrict_sql: (optional) a SQL query, which should return a set of rows with the following columns:

	.. code-block:: sql

		SELECT to_cost, target_id, via_path FROM restrictions

	:to_cost: ``float8`` turn restriction cost
	:target_id: ``int4`` target id
	:via_path: ``text`` comma separated list of edges in the reverse order of ``rule``

Another variant of TRSP allows to specify **EDGE id** of source and target together with a fraction to interpolate the position:

:source_edge: ``int4`` **EDGE id** of the start edge
:source_pos: ``float8`` fraction of 1 defines the position on the start edge
:target_edge: ``int4`` **EDGE id** of the end edge
:target_pos: ``float8`` fraction of 1 defines the position on the end edge

Returns set of:

:seq:   row sequence
:id1:   node ID
:id2:   edge ID (``-1`` for the last row)
:cost:  cost to traverse from ``id1`` using ``id2``

Support for Vias
-------------------------------------------------------------------------------

.. warning:: The Support for Vias functions are prototypes. Not all corner cases are being considered.


We also have support for vias where you can say generate a from A to B to C, etc. We support both methods above only you pass an array of vertices or and array of edges and percentage position along the edge in two arrays.

:sql: a SQL query, which should return a set of rows with the following columns:

	.. code-block:: sql

		SELECT id, source, target, cost, [,reverse_cost] FROM edge_table


	:id: ``int4`` identifier of the edge
	:source: ``int4`` identifier of the source vertex
	:target: ``int4`` identifier of the target vertex
	:cost: ``float8`` value, of the edge traversal cost. A negative cost will prevent the edge from being inserted in the graph.
	:reverse_cost: (optional) the cost for the reverse traversal of the edge. This is only used when the ``directed`` and ``has_rcost`` parameters are ``true`` (see the above remark about negative costs).

:vids: ``int4[]`` An ordered array of **NODE id** the path will go through from start to end.
:directed: ``true`` if the graph is directed
:has_rcost: if ``true``, the ``reverse_cost`` column of the SQL generated set of rows will be used for the cost of the traversal of the edge in the opposite direction.

:restrict_sql: (optional) a SQL query, which should return a set of rows with the following columns:

	.. code-block:: sql

		SELECT to_cost, target_id, via_path FROM restrictions

	:to_cost: ``float8`` turn restriction cost
	:target_id: ``int4`` target id
	:via_path: ``text`` commar separated list of edges in the reverse order of ``rule``

Another variant of TRSP allows to specify **EDGE id** together with a fraction to interpolate the position:

:eids: ``int4`` An ordered array of **EDGE id** that the path has to traverse
:pcts: ``float8`` An array of fractional positions along the respective edges in ``eids``, where 0.0 is the start of the edge and 1.0 is the end of the eadge.

Returns set of:

:seq:   row sequence
:id1:   route ID
:id2:   node ID
:id3:   edge ID (``-1`` for the last row)
:cost:  cost to traverse from ``id2`` using ``id3``

Additional Examples
-------------------------------------------------------------------------------

:Example: Without turn restrictions

.. literalinclude:: doc-trsp.queries
   :start-after: --q1
   :end-before: --q2

:Example: With turn restrictions

Then a query with turn restrictions is created as:

.. literalinclude:: doc-trsp.queries
   :start-after: --q3
   :end-before: --q4

An example query using vertex ids and via points:

.. literalinclude:: doc-trsp.queries
   :start-after: --q4
   :end-before: --q5

An example query using edge ids and vias:

.. literalinclude:: doc-trsp.queries
   :start-after: --q5
   :end-before: --q6

The queries use the :doc:`sampledata` network.

Known Issues
-------------------------------------------------------------------------------
Introduction
.........................................................................
pgr_trsp code has issues that are not being fixed yet, but as time passes and new functionality is added to pgRouting with wrappers to **hide** the issues, not to fix them.

For clarity on the queries:

* _pgr_trsp (internal_function) is the original code
* pgr_trsp (lower case) represents the wrapper calling the original code
* pgr_TRSP (upper case) represents the wrapper calling the replacement function, depending on the function, it can be:

  * pgr_dijkstra
  * pgr_dijkstraVia
  * pgr_withPoints
  * _pgr_withPointsVia (internal function)

The restrictions
.........................................................................

The restriction used in the examples does not have to do anything with the graph:

* No vertex has id: 25, 32 or 33
* No edge has id: 25, 32 or 33

A restriction is assigned as:

.. literalinclude:: trsp_notes.queries
   :start-after: --place1
   :end-before: --place2

The back end code has that same restriction as follows

.. literalinclude:: trsp_notes.queries
   :start-after: --place2
   :end-before: --place3

therefore the shortest path expected are as if there was no restriction involved

The "Vertices" signature version
.........................................................................

.. code-block:: sql

  pgr_trsp(sql text, source integer, target integer,
           directed boolean, has_rcost boolean [,restrict_sql text]);

Different ways to represent 'no path found`
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

* Sometimes represents with EMPTY SET a no path found
* Sometimes represents with Error a no path found

.. rubric:: Returning EMPTY SET to represent no path found

.. literalinclude:: trsp_notes.queries
   :start-after: --place3
   :end-before: --place4

pgr_trsp calls :doc:`pgr_dijkstra` when there are no restrictions which returns
`EMPTY SET` when a path is not found

.. literalinclude:: trsp_notes.queries
   :start-after: --place4
   :end-before: --place5

.. rubric:: Throwing EXCEPTION to represent no path found

.. literalinclude:: trsp_notes.queries
   :start-after: --place5
   :end-before: --place6

pgr_trsp use the original code when there are restrictions, even if they have nothing to do with the graph,
which will throw an EXCEPTION to represent no path found.

Routing from/to same location
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

When routing from location :math:`1` to the same location :math:`1`, no path is needed to
reach the destination, its already there. Therefore is expected to return an
`EMPTY SET` or an `EXCEPTION` depending on the parameters

* Sometimes represents with EMPTY SET no path found (expected)
* Sometimes represents with EXCEPTION no path found (expected)
* Sometimes finds a path (not expected)

.. rubric:: Returning expected EMPTY SET to represent no path found

.. literalinclude:: trsp_notes.queries
   :start-after: --place6
   :end-before: --place7

``pgr_trsp`` calls :doc:`pgr_dijkstra` when there are no restrictions which returns
the expected to return `EMPTY SET` to represent no path found.

.. rubric:: Returning expected EXCEPTION to represent no path found

.. literalinclude:: trsp_notes.queries
   :start-after: --place7
   :end-before: --place8

In this case ``pgr_trsp`` calls the original code when there are restrictions, even if they have nothing to do with the graph,
in this case that code throws the expected EXCEPTION

.. rubric:: Returning unexpected path

.. literalinclude:: trsp_notes.queries
   :start-after: --place8
   :end-before: --place9

In this case ``pgr_trsp`` calls the original code when there are restrictions, even if they have nothing to do with the graph,
in this case that code finds an unexpected path.


User contradictions
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

``pgr_trsp`` unlike other pgRouting functions does not autodectect the existence of
``reverse_cost`` column. Therefor it has ``has_rcost`` parameter to check the existence
of ``reverse_cost`` column. Contradictions happen:

- When the reverse_cost is missing, and the flag `has_rcost` is set to true
- When the reverse_cost exists, and the flag `has_rcost` is set to false

.. rubric:: When the reverse_cost is missing, and the flag `has_rcost` is set to true.

.. literalinclude:: trsp_notes.queries
   :start-after: --place9
   :end-before: --place10

An EXCEPTION is thrown.

.. rubric:: When the reverse_cost exists, and the flag `has_rcost` is set to false

.. literalinclude:: trsp_notes.queries
   :start-after: --place10
   :end-before: --place11

The ``reverse_cost`` column will be effectively removed and will cost execution time

The "Edges" signature version
.........................................................................

.. code-block:: sql

  pgr_trsp(sql text, source_edge integer, source_pos float8,
           target_edge integer, target_pos float8,
           directed boolean, has_rcost boolean [,restrict_sql text]);

Different ways to represent 'no path found`
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

* Sometimes represents with EMPTY SET a no path found
* Sometimes represents with EXCEPTION a no path found

.. rubric:: Returning EMPTY SET to represent no path found

.. literalinclude:: trsp_notes.queries
   :start-after: --place11
   :end-before: --place12

pgr_trsp calls :doc:`pgr_withPoints` when there are no restrictions which returns
`EMPTY SET` when a path is not found

.. rubric:: Throwing EXCEPTION to represent no path found

.. literalinclude:: trsp_notes.queries
   :start-after: --place12
   :end-before: --place13

pgr_trsp use the original code when there are restrictions, even if they have nothing to do with the graph,
which will throw an EXCEPTION to represent no path found.

Paths with equal number of vertices and edges
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

A path is made of `N` vertices and `N - 1` edges.

* Sometimes returns `N` vertices and `N - 1` edges.
* Sometimes returns `N - 1` vertices and `N - 1` edges.

.. rubric:: Returning `N` vertices and `N - 1` edges.

.. literalinclude:: trsp_notes.queries
   :start-after: --place13
   :end-before: --place14

pgr_trsp calls :doc:`pgr_withPoints` when there are no restrictions which returns
the correct number of rows that will include all the vertices. The last row will have a ``-1``
on the edge column to indicate the edge number is invalidu for that row.


.. rubric:: Returning `N - 1` vertices and `N - 1` edges.

.. literalinclude:: trsp_notes.queries
   :start-after: --place14
   :end-before: --place15

pgr_trsp use the original code when there are restrictions, even if they have nothing to do with the graph,
and will not return the last vertex of the path.

Routing from/to same location
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

When routing from the same edge and position to the same edge and position, no path is needed to
reach the destination, its already there. Therefore is expected to return an
`EMPTY SET` or an `EXCEPTION` depending on the parameters, non of which is happening.


.. rubric:: A path with 2 vertices and edge cost 0

.. literalinclude:: trsp_notes.queries
   :start-after: --place15
   :end-before: --place16

pgr_trsp calls :doc:`pgr_withPoints` setting the first :math:`(edge, position)` with a differenct point id
from the second :math:`(edge, position)` making them different points. But the cost using the edge, is :math:`0`.


.. rubric:: A path with 1 vertices and edge cost 0

.. literalinclude:: trsp_notes.queries
   :start-after: --place16
   :end-before: --place17

pgr_trsp use the original code when there are restrictions, even if they have nothing to do with the graph,
and will not have the row for the vertex :math:`-2`.


User contradictions
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

``pgr_trsp`` unlike other pgRouting functions does not autodectect the existence of
``reverse_cost`` column. Therefor it has ``has_rcost`` parameter to check the existence
of ``reverse_cost`` column. Contradictions happen:

- When the reverse_cost is missing, and the flag `has_rcost` is set to true
- When the reverse_cost exists, and the flag `has_rcost` is set to false

.. rubric:: When the reverse_cost is missing, and the flag `has_rcost` is set to true.

.. literalinclude:: trsp_notes.queries
   :start-after: --place17
   :end-before: --place18

An EXCEPTION is thrown.

.. rubric:: When the reverse_cost exists, and the flag `has_rcost` is set to false

.. literalinclude:: trsp_notes.queries
   :start-after: --place18
   :end-before: --place19

The ``reverse_cost`` column will be effectively removed and will cost execution time

Using a points of interest table
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Given a set of points of interest:

.. literalinclude:: trsp_notes.queries
   :start-after: --place19
   :end-before: --place20

.. rubric:: Using pgr_trsp

.. literalinclude:: trsp_notes.queries
   :start-after: --place20
   :end-before: --place21

On *pgr_trsp*, to be able to use the table information:

* Each parameter has to be extracted explicitly from the table
* Regardles of the point pid original value

  * will always be -1 for the first point
  * will always be -2 for the second point

    * the row reaching point -2 will not be shown

.. rubric:: Using :doc:`pgr_withPoints`

.. literalinclude:: trsp_notes.queries
   :start-after: --place21
   :end-before: --place22

Suggestion: use :doc:`pgr_withPoints` when there are no turn restrictions:

* Results are more complete
* Column names are meaningful

Routing from a vertex to a point
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Solving a shortest path from vertex :math:`6` to pid 1 using a points of interest table

.. rubric:: Using pgr_trsp

.. literalinclude:: trsp_notes.queries
   :start-after: --place22
   :end-before: --place23

* Vertex 6 is on edge 8 at 1 fraction


.. literalinclude:: trsp_notes.queries
   :start-after: --place23
   :end-before: --place24

* Vertex 6 is also edge 11 at 0 fraction

.. rubric:: Using :doc:`pgr_withPoints`

.. literalinclude:: trsp_notes.queries
   :start-after: --place24
   :end-before: --place25

Suggestion: use :doc:`pgr_withPoints` when there are no turn restrictions:

* No need to choose where the vertex is located.
* Results are more complete
* Column names are meaningful


prototypes
.........................................................................

``pgr_trspViaVertices`` and ``pgr_trspViaEdges`` were added to pgRouting as prototypes

These functions use the pgr_trsp functions inheriting all the problems mentioned above.
When there are no restrictions and have a routing "via" problem with vertices:

* :doc:`pgr_dijkstraVia`



See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
