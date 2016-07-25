.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_astar-V2.0:

pgr_astar - Deprecated Signature
===============================================================================

.. warning:: This function signature is deprecated!!!

    * That means it has been replaced by new signature(s)
    * This signature is no longer supported, and may be removed from future versions.
    * All code that use this function signature should be converted to use its replacement :ref:`pgr_astar`.

Name
-------------------------------------------------------------------------------

``pgr_astar`` — Returns the shortest path using A* algorithm.



Synopsis
-------------------------------------------------------------------------------

The A* (pronounced "A Star") algorithm is based on Dijkstra's algorithm with a heuristic that allow it to solve most shortest path problems by evaluation only a sub-set of the overall graph. Returns a set of :ref:`pgr_costResult <type_cost_result>` (seq, id1, id2, cost) rows, that make up a path.

.. code-block:: sql

	pgr_costResult[] pgr_astar(sql text, source integer, target integer,
                               directed boolean, has_rcost boolean);


Description
-------------------------------------------------------------------------------

:sql: a SQL query, which should return a set of rows with the following columns:

	.. code-block:: sql

		SELECT id, source, target, cost, x1, y1, x2, y2 [,reverse_cost] FROM edge_table


	:id: ``int4`` identifier of the edge
	:source: ``int4`` identifier of the source vertex
	:target: ``int4`` identifier of the target vertex
	:cost: ``float8`` value, of the edge traversal cost. A negative cost will prevent the edge from being inserted in the graph.
	:x1: ``x`` coordinate of the start point of the edge
	:y1: ``y`` coordinate of the start point of the edge
	:x2: ``x`` coordinate of the end point of the edge
	:y2: ``y`` coordinate of the end point of the edge
	:reverse_cost: (optional) the cost for the reverse traversal of the edge. This is only used when the ``directed`` and ``has_rcost`` parameters are ``true`` (see the above remark about negative costs).

:source: ``int4`` id of the start point
:target: ``int4`` id of the end point
:directed: ``true`` if the graph is directed
:has_rcost: if ``true``, the ``reverse_cost`` column of the SQL generated set of rows will be used for the cost of the traversal of the edge in the opposite direction.

Returns set of :ref:`type_cost_result`:

:seq:   row sequence
:id1:   node ID
:id2:   edge ID (``-1`` for the last row)
:cost:  cost to traverse from ``id1`` using ``id2``


.. rubric:: History

* Renamed in version 2.0.0


Examples
-------------------------------------------------------------------------------

* Without ``reverse_cost``

.. literalinclude:: doc-astar-v2.queries
   :start-after: -- q1
   :end-before: -- q2

* With ``reverse_cost``

.. literalinclude:: doc-astar-v2.queries
   :start-after: -- q2
   :end-before: -- q3


The queries use the :ref:`sampledata` network.


See Also
-------------------------------------------------------------------------------

* :ref:`pgr_astar`
* :ref:`type_cost_result`
* http://en.wikipedia.org/wiki/A*_search_algorithm
