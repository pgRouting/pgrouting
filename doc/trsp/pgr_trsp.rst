..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _trsp:

pgr_trsp - Turn Restriction Shortest Path (TRSP)
===============================================================================

.. index::
	single: trsp(text,integer,integer,boolean,boolean)
	single: trsp(text,integer,integer,boolean,boolean,text)
	single: trspViaVertices(text,integer,double precision,integer,double precision,boolean,boolean)
	single: trspViaEdges(text,integer,double precision,integer,double precision,boolean,boolean,text)

Name
-------------------------------------------------------------------------------

``pgr_trsp`` — Returns the shortest path with support for turn restrictions.


Synopsis
-------------------------------------------------------------------------------

The turn restricted shorthest path (TRSP) is a shortest path algorithm that can optionally take into account complicated turn restrictions like those found in real world navigable road networks. Performamnce wise it is nearly as fast as the A* search but has many additional features like it works with edges rather than the nodes of the network. Returns a set of :ref:`pgr_costResult <type_cost_result>` (seq, id1, id2, cost) rows, that make up a path.

.. code-block:: sql

	pgr_costResult[] pgr_trsp(sql text, source integer, target integer,
                    directed boolean, has_rcost boolean [,restrict_sql text]);


.. code-block:: sql

	pgr_costResult[] pgr_trsp(sql text, source_edge integer, source_pos float8,
	                target_edge integer, target_pos float8,
                    directed boolean, has_rcost boolean [,restrict_sql text]);

.. code-block:: sql

    pgr_costResult3[] pgr_trspViaVertices(sql text, vids integer[],
                    directed boolean, has_rcost boolean
                    [, turn_restrict_sql text]);

.. code-block:: sql

     pgr_costResult3[] pgr_trspViaEdges(sql text, eids integer[], pcts float8[],
                    directed boolean, has_rcost boolean
                    [, turn_restrict_sql text]);

Description
-------------------------------------------------------------------------------

The Turn Restricted Shortest Path algorithm (TRSP) is similar to the shooting star in that you can specify turn restrictions.

The TRSP setup is mostly the same as :ref:`Dijkstra shortest path <pgr_dijkstra>` with the addition of an optional turn restriction table. This provides an easy way of adding turn restrictions to a road network by placing them in a separate table.


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

Returns set of :ref:`type_cost_result`:

:seq:   row sequence
:id1:   node ID
:id2:   edge ID (``-1`` for the last row)
:cost:  cost to traverse from ``id1`` using ``id2``

.. rubric:: History

* New in version 2.0.0

Support for Vias
--------------------------------------------------------------------

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

Returns set of :ref:`type_cost_result`:

:seq:   row sequence
:id1:   route ID
:id2:   node ID
:id3:   edge ID (``-1`` for the last row)
:cost:  cost to traverse from ``id2`` using ``id3``


.. rubric:: History

* Via Support prototypes new in version 2.1.0

Examples
-------------------------------------------------------------------------------

**Without turn restrictions**

.. literalinclude:: doc-trsp.queries
   :start-after: --q1
   :end-before: --q2


**With turn restrictions**

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


See Also
-------------------------------------------------------------------------------

* :ref:`type_cost_result`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
