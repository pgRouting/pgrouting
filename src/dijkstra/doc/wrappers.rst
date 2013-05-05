.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This work is licensed under a Creative Commons Attribution-Share Alike 3.0 
    License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_dijkstra_wrappers:

PGR_Dijkstra (Wrappers) — Shortest Path Dijkstra
===============================================================================

.. index:: 
	single: PGR_Dijkstra(varchar,integer,integer)
	single: PGR_Dijkstra(varchar,integer,integer,double precision)
	single: PGR_Dijkstra(varchar,integer,integer,double precision,boolean,boolean)
	single: PGR_Dijkstra(varchar,integer,integer,boolean,boolean)
	module: dijkstra

Name
-------------------------------------------------------------------------------

``PGR_Dijkstra`` — Returns the shortest path using Dijkstra algorithm.


Synopsis
-------------------------------------------------------------------------------

Returns a set of :ref:`geoms <type_geoms>` (id, gid, the_geom) rows, that make up a path.

.. code-block:: sql

	geoms[] PGR_Dijkstra(varchar geom_column, integer source, integer target);

	geoms[] PGR_Dijkstra(varchar geom_column, integer source, integer target, double precision delta);

	geoms[] PGR_Dijkstra(varchar geom_column, integer source, integer target, double precision delta, boolean directed, boolean reverse_cost);

	geoms[] PGR_Dijkstra(varchar geom_column, integer source, integer target, boolean directed, boolean reverse_cost);


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

* :ref:`pgr_dijkstra_core`
* :ref:`type_geoms`

