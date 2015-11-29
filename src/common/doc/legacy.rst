/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

********************************************************************PGR-GNU*/
..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _common_legacy:

Legacy Functions
===============================================================================

pgRouting 2.0 release has total restructured the function naming and obsoleted many of the functions that were available in the 1.x releases. While we realize that this may inconvenience our existing users, we felt this was needed for the long term viability of the project to be more response to our users and to be able to add new functionality and test existing functionality.

We have made a minimal effort to save most of these function and distribute with the release in a file ``pgrouting_legacy.sql`` that is not part of the pgrouting extension and is not supported. If you can use these functions that is great. We have not tested any of these functions so if you find issues and want to post a pull request or a patch to help other users that is fine, but it is likely this file will be removed in a future release and we strongly recommend that you convert your existing code to use the new documented and supported functions.

The follow is a list of TYPEs, CASTs and FUNCTION included in the ``pgrouting_legacy.sql`` file. The list is provide as a convenience but these functions are deprecated, not supported, and probably will need some changes to get them to work.


TYPEs & CASTs
********************************************************************************

.. code-block:: sql

	TYPE vertex_result AS ( x float8, y float8 ):
	CAST (pgr_pathResult AS path_result) WITHOUT FUNCTION AS IMPLICIT;
	CAST (pgr_geoms AS geoms) WITHOUT FUNCTION AS IMPLICIT;
	CAST (pgr_linkPoint AS link_point) WITHOUT FUNCTION AS IMPLICIT;


FUNCTIONs
********************************************************************************

.. code-block:: sql

	FUNCTION text(boolean)
	FUNCTION add_vertices_geometry(geom_table varchar)
	FUNCTION update_cost_from_distance(geom_table varchar)
	FUNCTION insert_vertex(vertices_table varchar, geom_id anyelement)
	FUNCTION pgr_shootingStar(sql text, source_id integer, target_id integer,
                              directed boolean, has_reverse_cost boolean)
	FUNCTION shootingstar_sp( varchar,int4, int4, float8, varchar, boolean, boolean)
	FUNCTION astar_sp_delta( varchar,int4, int4, float8)
	FUNCTION astar_sp_delta_directed( varchar,int4, int4, float8, boolean, boolean)
	FUNCTION astar_sp_delta_cc( varchar,int4, int4, float8, varchar)
	FUNCTION astar_sp_delta_cc_directed( varchar,int4, int4, float8, varchar, boolean, boolean)
	FUNCTION astar_sp_bbox( varchar,int4, int4, float8, float8, float8, float8)
	FUNCTION astar_sp_bbox_directed( varchar,int4, int4, float8, float8, float8,
                                     float8, boolean, boolean)
	FUNCTION astar_sp( geom_table varchar, source int4, target int4)
	FUNCTION astar_sp_directed( geom_table varchar, source int4, target int4,
                                dir boolean, rc boolean)
	FUNCTION dijkstra_sp( geom_table varchar, source int4, target int4)
	FUNCTION dijkstra_sp_directed( geom_table varchar, source int4, target int4,
                                   dir boolean, rc boolean)
	FUNCTION dijkstra_sp_delta( varchar,int4, int4, float8)
	FUNCTION dijkstra_sp_delta_directed( varchar,int4, int4, float8, boolean, boolean)
	FUNCTION tsp_astar( geom_table varchar,ids varchar, source integer, delta double precision)
	FUNCTION tsp_astar_directed( geom_table varchar,ids varchar, source integer, delta float8, dir boolean, rc boolean)
	FUNCTION tsp_dijkstra( geom_table varchar,ids varchar, source integer)
	FUNCTION tsp_dijkstra_directed( geom_table varchar,ids varchar, source integer,
                                    delta float8, dir boolean, rc boolean)

