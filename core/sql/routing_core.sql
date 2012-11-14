--
-- Copyright (c) 2005 Sylvain Pasche,
--               2006-2007 Anton A. Patrushev, Orkney, Inc.
--
-- This program is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation; either version 2 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with this program; if not, write to the Free Software
-- Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
--


CREATE TYPE path_result AS (vertex_id integer, edge_id integer, cost float8);
CREATE TYPE vertex_result AS (x float8, y float8);

-----------------------------------------------------------------------
-- Core function for shortest_path computation
-- See README for description
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION shortest_path(sql text, source_id integer, 
        target_id integer, directed boolean, has_reverse_cost boolean)
        RETURNS SETOF path_result
        AS '$libdir/librouting'
        LANGUAGE C IMMUTABLE STRICT;

-----------------------------------------------------------------------
-- Core function for shortest_path_astar computation
-- Simillar to shortest_path in usage but uses the A* algorithm
-- instead of Dijkstra's.
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION shortest_path_astar(sql text, source_id integer, 
        target_id integer,directed boolean, has_reverse_cost boolean)
         RETURNS SETOF path_result
         AS '$libdir/librouting'
         LANGUAGE C IMMUTABLE STRICT; 

-----------------------------------------------------------------------
-- Core function for shortest_path_astar computation
-- Simillar to shortest_path in usage but uses the Shooting* algorithm
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION shortest_path_shooting_star(sql text, source_id integer, 
        target_id integer,directed boolean, has_reverse_cost boolean)
         RETURNS SETOF path_result
         AS '$libdir/librouting'
         LANGUAGE C IMMUTABLE STRICT; 

-----------------------------------------------------------------------
-- This function should not be used directly. Use create_graph_tables instead
--
-- Insert a vertex into the vertices table if not already there, and
--  return the id of the newly inserted or already existing element
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION insert_vertex(vertices_table varchar, 
       geom_id anyelement) 
       RETURNS int AS
$$
DECLARE
        vertex_id int;
        myrec record;
BEGIN
        LOOP
          FOR myrec IN EXECUTE 'SELECT id FROM ' || 
                     quote_ident(vertices_table) || 
                     ' WHERE geom_id = ' || quote_literal(geom_id)  LOOP

                        IF myrec.id IS NOT NULL THEN
                                RETURN myrec.id;
                        END IF;
          END LOOP; 
          EXECUTE 'INSERT INTO ' || quote_ident(vertices_table) || 
                  ' (geom_id) VALUES (' || quote_literal(geom_id) || ')';
        END LOOP;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 
