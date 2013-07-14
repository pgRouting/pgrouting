..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_kdijkstra:

pgr_kDijkstra - Mutliple destination Shortest Path Dijkstra
===============================================================================

.. index::
    single: pgr_kDijkstraCost(text,integer,integer[],boolean,boolean)
    single: pgr_kDijkstraPath(text,integer,integer[],boolean,boolean)
    module: dijkstra

Name
-------------------------------------------------------------------------------

* ``pgr_kdijkstraCost`` - Returns the costs for K shortest paths using Dijkstra algorithm.
* ``pgr_kdijkstraPath`` - Returns the paths for K shortest paths using Dijkstra algorithm.


Synopsis
-------------------------------------------------------------------------------

This function allow you to have a single start node and multiple destination nodes and will compute the routes to all the destinations from the source node. Returns a set of :ref:`pgr_costResult <type_cost_result>` (seq, id1, id2, cost) rows, that summarize the total path cost. ``pgr_kdijkstraCost`` returns one record for each destination node and the cost is the total code of the route to that node. ``pgr_kdijkstraPath`` returns one record for every edge in that path from source to destination and the cost is to traverse that edge.

.. code-block:: sql

    pgr_costResult[] pgr_kdijkstraCost(text sql, integer source,
                     integer[] targets, boolean directed, boolean has_rcost);

    pgr_costResult[] pgr_kdijkstraPath(text sql, integer source,
                     integer[] targets, boolean directed, boolean has_rcost);


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
:targets: ``int4[]`` an array of ids of the end points
:directed: ``true`` if the graph is directed
:has_rcost: if ``true``, the ``reverse_cost`` column of the SQL generated set of rows will be used for the cost of the traversal of the edge in the opposite direction.


``pgr_kdijkstraCost`` returns set of :ref:`type_cost_result`:

:seq:   row sequence
:id1:   path vertex source id (this will always be source start point in the query).
:id2:   path vertex target id
:cost:  cost to traverse the path from ``id1`` to ``id2``. Cost will be -1.0 if there is no path to that target vertex id.


``pgr_kdijkstraPath`` returns set of :ref:`type_cost_result`:

:seq:   row sequence
:id1:   path vertex target id (identifies the target path).
:id2:   path edge id
:cost:  cost to traverse this edge or -1.0 if there is no path to this target


.. rubric:: History

* New in version 2.0.0


Examples
-------------------------------------------------------------------------------

* Returning a ``cost`` result

.. code-block:: sql

    SELECT seq, id1 AS source, id2 AS target, cost FROM pgr_kdijkstraCost(
        'SELECT id, source, target, cost FROM edge_table',
        10, array[4,12], false, false
    );

     seq | source | target | cost 
    -----+--------+--------+------
       0 |     10 |      4 |    4
       1 |     10 |     12 |    2


.. code-block:: sql

    SELECT seq, id1 AS path, id2 AS edge, cost FROM pgr_kdijkstraPath(
        'SELECT id, source, target, cost FROM edge_table',
        10, array[4,12], false, false
    );

     seq | path | edge | cost 
    -----+------+------+------
       0 |    4 |   12 |    1
       1 |    4 |   13 |    1
       2 |    4 |   15 |    1
       3 |    4 |   16 |    1
       4 |   12 |   12 |    1
       5 |   12 |   13 |    1
    (6 rows)


See Also
-------------------------------------------------------------------------------

* :ref:`type_cost_result`
* http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
