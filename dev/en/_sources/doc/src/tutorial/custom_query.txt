.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _custom_query:

Dictionary of columns & Custom Query
===============================================================================

:path: a sequence of vertices/edges from A to B.
:route: a sequence of paths. 

Description of the edges_sql query
-------------------------------------------------------------------------------

:edges_sql: an SQL query, which should return a set of rows with the following columns:

        :id: ``ANY-INTEGER`` identifier of the edge.
        :source: ``ANY-INTEGER`` identifier of the first end point vertex of the edge.
        :target: ``ANY-INTEGER`` identifier of the second end pont vertex of the edge.
        :cost: ``ANY-NUMERICAL`` weight of the edge `(source, target)`, if negative: edge `(source, target)` does not exist, therefore it's not part of the graph.
        :reverse_cost: ``ANY-NUMERICAL`` (optional) weight of the edge `(target, source)`, if negative: edge `(target, source)` does not exist, therefore it's not part of the graph.

Where:

:ANY-INTEGER: smallint, int, bigint
:ANY-NUMERICAL: smallint, int, bigint, real, float


Description of the parameters of the signatures
-------------------------------------------------------------------------------

:edges_sql: ``TEXT`` SQL query as decribed above.
:start_vid: ``BIGINT`` identifier of the starting vertex of the path.
:start_vids: ``array[ANY-INTEGER]`` array of identifiers of starting vertices.
:end_vid: ``BIGINT`` identifier of the ending vertex of the path.
:end_vids: ``array[ANY-INTEGER]`` array of identifiers of ending vertices.
:directed: ``boolean`` (optional). When ``false`` the graph is considered as Undirected. Default is ``true`` which considers the graph as Directed.


Description of the return values
-------------------------------------------------------------------------------

Returns set of ``(seq [, start_vid] [, end_vid] , node, edge, cost, agg_cost)``

:seq: ``INTEGER``  is a sequential value starting from **1**.
:route_seq: ``INTEGER``  relative position in the route. Has value **1** for the begining of a route.
:route_id: ``INTEGER`` id of the route.
:path_seq: ``INTEGER``  relative position in the path. Has value **1** for the begining of a path.
:path_id: ``INTEGER`` id of the path.
:start_vid: ``BIGINT`` id of the starting vertex. Used when multiple starting vetrices are in the query.
:end_vid: ``BIGINT`` id of the ending vertex. Used when multiple ending vertices are in the query.
:node: ``BIGINT`` id of the node in the path from start_vid to end_v.
:edge: ``BIGINT`` id of the edge used to go from ``node`` to the next node in the path sequence. ``-1`` for the last node of the path.
:cost: ``FLOAT`` cost to traverse from ``node`` using ``edge`` to the next node in the path sequence.
:agg_cost:  ``FLOAT`` total cost from ``start_vid`` to ``node``.



Descriptions for version 2.0 signatures
---------------------------------------

In general, the routing algorithms need an SQL query that contain one or more of the following required columns with the preferred type:

:id:	 int4
:source: int4
:target: int4
:cost:	float8
:reverse_cost: float8
:x:     float8
:y:     float8
:x1:	float8
:y1:	float8 
:x2:	float8 
:y2:	float8



.. code-block:: sql

	SELECT source, target, cost FROM edge_table;
	SELECT id, source, target, cost FROM edge_table;
	SELECT id, source, target, cost, x1, y1, x2, y2 ,reverse_cost FROM edge_table

When the edge table has a different name to represent the required columns:

.. code-block:: sql

        SELECT src as source,  target, cost FROM othertable;
        SELECT gid as id, src as source, target, cost FROM othertable;
        SELECT gid as id, src as source, target, cost, fromX as x1, fromY as y1, toX as x2, toY as y2 ,Rcost as reverse_cost 
	       FROM othertable;


.. Topology functions

The topology functions use the same names for ``id``, ``source`` and ``target`` columns of the edge table, The fowllowing parameters have as default value:

:id:	 int4 Default ``id``
:source: int4 Default ``source``
:target: int4 Default ``target``
:the_geom: text Default ``the_geom``
:oneway: text Default ``oneway``
:rows_where: text Default ``true`` to indicate all rows (this is not a column)

The following parameters do not have a default value and when used they have to be inserted in strict order:

:edge_table: text
:tolerance: float8
:s_in_rules: text[]
:s_out_rules: text[]
:t_in_rules: text[]
:t_out_rules: text[]

When the columns required have the default names this can be used (pgr_func is to represent a topology function)

.. code-block:: sql

        pgr_func('edge_table')        -- when tolerance is not requiered
	pgr_func('edge_table',0.001)  -- when tolerance is requiered
        -- s_in_rule, s_out_rule, st_in_rules, t_out_rules are requiered
	SELECT pgr_analyzeOneway('edge_table', ARRAY['', 'B', 'TF'], ARRAY['', 'B', 'FT'], 
					       ARRAY['', 'B', 'FT'], ARRAY['', 'B', 'TF']) 

When the columns required do not have the default names its strongly recomended to use the *named notation*.

.. code-block:: sql

        pgr_func('othertable', id:='gid',source:='src',the_geom:='mygeom')     
	pgr_func('othertable',0.001,the_geom:='mygeom',id:='gid',source:='src') 
	SELECT pgr_analyzeOneway('othertable', ARRAY['', 'B', 'TF'], ARRAY['', 'B', 'FT'], 
					       ARRAY['', 'B', 'FT'], ARRAY['', 'B', 'TF']
                                 source:='src',oneway:='dir') 

