.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _custom_query:

Custom Query
===============================================================================

.. Routing Algorithms:

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




.. warning:: The functions for V3.0 of pgr_dijkstra, pgr_ksp and pgr_drivingDistance use the following columns
              - **id**: ``ANY-INTEGER`` identifier of the edge.
              - **source**: ``ANY-INTEGER`` identifier of the source vertex of the edge.
              - **target**: ``ANY-INTEGER`` identifier of the target vertex of the edge.
              - **cost**: ``ANY-NUMERICAL`` value of the edge traversal cost. A negative cost will prevent the edge (``source``, ``target``) from being inserted in the graph.
              - **reverse_cost**: ``ANY-NUMERICAL`` (optional) the value for the reverse traversal of the edge. A negative cost will prevent the edge (``target``, ``source``) from being inserted in the graph.

             Where:

              - **ANY-INTEGER**: smallint, int, bigint
              - **ANY-NUMERICAL**: smallint, int, bigint, real, float


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

