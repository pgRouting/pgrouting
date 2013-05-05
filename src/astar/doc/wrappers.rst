.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This work is licensed under a Creative Commons Attribution-Share Alike 3.0 
    License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_astar_wrappers:

PGR_Astar (Wrappers) — Shortest Path A*
===============================================================================

.. index:: 
	single: PGR_Astar(varchar,integer,integer)
	single: PGR_Astar(varchar,integer,integer,double precision)
	single: PGR_Astar(varchar,integer,integer,double precision,boolean,boolean)
	single: PGR_Astar(varchar,integer,integer,double precision,varchar)
	single: PGR_Astar(varchar,integer,integer,double precision,varchar,boolean,boolean)
	single: PGR_Astar(varchar,integer,integer,boolean,boolean)
	module: astar

Name
-------------------------------------------------------------------------------

``PGR_Astar`` — Returns the shortest path using A* algorithm.


Synopsis
-------------------------------------------------------------------------------

Returns a set of :ref:`geoms <type_geoms>` (id, gid, the_geom) rows, that make up a path.

.. code-block:: sql

	geoms[] PGR_Astar(varchar table_name, integer source, integer target);

	geoms[] PGR_Astar(varchar table_name, integer source, integer target, double precision delta);

	geoms[] PGR_Astar(varchar table_name, integer source, integer target, double precision delta, boolean directed, boolean reverse_cost);

	geoms[] PGR_Astar(varchar table_name, integer source, integer target, double precision delta, varchar cost_column);

	geoms[] PGR_Astar(varchar table_name, integer source, integer target, double precision delta, varchar cost_column, boolean directed, boolean reverse_cost);

	geoms[] PGR_Astar(varchar table_name, integer source, integer target, boolean directed, boolean reverse_cost);


Description
-------------------------------------------------------------------------------

.. note:: 
	Not completed.


Examples
-------------------------------------------------------------------------------

.. note:: 
	Not completed.


See Also
-------------------------------------------------------------------------------

* :ref:`pgr_astar_core`
* :ref:`type_geoms`

