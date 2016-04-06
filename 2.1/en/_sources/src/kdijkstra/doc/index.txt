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

Name
-------------------------------------------------------------------------------

* ``pgr_kdijkstraCost`` - Returns the costs for K shortest paths using Dijkstra algorithm.
* ``pgr_kdijkstraPath`` - Returns the paths for K shortest paths using Dijkstra algorithm.


Synopsis
-------------------------------------------------------------------------------

These functions allow you to have a single start node and multiple destination nodes and will compute the routes to all the destinations from the source node. Returns a set of :ref:`pgr_costResult <type_cost_result>` or :ref:`pgr_costResult3 <type_cost_result3>`. ``pgr_kdijkstraCost`` returns one record for each destination node and the cost is the total code of the route to that node. ``pgr_kdijkstraPath`` returns one record for every edge in that path from source to destination and the cost is to traverse that edge.

.. code-block:: sql

    pgr_costResult[] pgr_kdijkstraCost(text sql, integer source,
                     integer[] targets, boolean directed, boolean has_rcost);

    pgr_costResult3[] pgr_kdijkstraPath(text sql, integer source,
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


``pgr_kdijkstraPath`` returns set of :ref:`type_cost_result3`:

:seq:   row sequence
:id1:   path target id (identifies the target path).
:id2:   path edge source node id
:id3:   path edge id (``-1`` for the last row)
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

    SELECT seq, id1 AS path, id2 AS node, id3 AS edge, cost
      FROM pgr_kdijkstraPath(
          'SELECT id, source, target, cost FROM edge_table',
          10, array[4,12], false, false
    );

     seq | path | node | edge | cost
    -----+------+------+------+------
       0 |    4 |   10 |   12 |    1
       1 |    4 |   11 |   13 |    1
       2 |    4 |   12 |   15 |    1
       3 |    4 |    9 |   16 |    1
       4 |    4 |    4 |   -1 |    0
       5 |   12 |   10 |   12 |    1
       6 |   12 |   11 |   13 |    1
       7 |   12 |   12 |   -1 |    0
    (8 rows)

* Returning a ``path`` result

.. code-block:: sql

    SELECT id1 as path, st_astext(st_linemerge(st_union(b.the_geom))) as the_geom
      FROM pgr_kdijkstraPath(
                      'SELECT id, source, target, cost FROM edge_table',
                      10, array[4,12], false, false
                ) a,
                edge_table b
    WHERE a.id3=b.id
    GROUP by id1
    ORDER by id1;

    path |            the_geom
    ------+---------------------------------
        4 | LINESTRING(2 3,3 3,4 3,4 2,4 1)
       12 | LINESTRING(2 3,3 3,4 3)
    (2 rows)


There is no assurance that the result above will be ordered in the direction
of flow of the route, ie: it might be reversed. You will need to check if
``st_startPoint()`` of the route is the same as the start node location and
if it is not then call ``st_reverse()`` to reverse the direction of the route.
This behavior is a function of PostGIS functions ``st_linemerge()`` and 
``st_union()`` and not pgRouting.


See Also
-------------------------------------------------------------------------------

* :ref:`type_cost_result`
* http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
