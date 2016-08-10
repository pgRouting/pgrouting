.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _bd_dijkstra:

pgr_bdDijkstra - Bi-directional Dijkstra Shortest Path
===============================================================================

.. index:: 
	single: bdDijkstra(Complete Signature)


Name
-------------------------------------------------------------------------------

``pgr_bdDijkstra`` - Returns the shortest path using Bidirectional Dijkstra algorithm.


Synopsis
-------------------------------------------------------------------------------

This is a bi-directional Dijkstra search algorithm. It searches from the source toward the distination and at the same time from the destination to the source and terminates whe these to searches meet in the middle. Returns a set of :ref:`pgr_costResult <type_cost_result>` (seq, id1, id2, cost) rows, that make up a path.

.. code-block:: sql

  pgr_costResult[] pgr_bdDijkstra(sql text, source integer, target integer,
                                  directed boolean, has_rcost boolean);


Description
-------------------------------------------------------------------------------

:sql: a SQL query, which should return a set of rows with the following columns:

  .. code-block:: sql

    SELECT id, source, target, cost [,reverse_cost] FROM edge_table


  :id: ``int4`` identifier of the edge
  :source: ``int4`` identifier of the source vertex
  :target: ``int4`` identifier of the target vertex
  :cost: ``float8`` value, of the edge traversal cost. A negative cost will prevent the edge from being inserted in the graph.
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

* New in version 2.0.0


Examples
-------------------------------------------------------------------------------

* Without ``reverse_cost``

.. literalinclude:: doc-bdDijkstra.queries
   :start-after: --q1
   :end-before: --q2

* With ``reverse_cost``

.. literalinclude:: doc-bdDijkstra.queries
   :start-after: --q2
   :end-before: --q3


The queries use the :ref:`sampledata` network.


See Also
-------------------------------------------------------------------------------

* :ref:`type_cost_result`
* :ref:`bd_astar`
* http://en.wikipedia.org/wiki/Bidirectional_search
* http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm

