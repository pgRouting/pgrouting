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


-- BEGIN;

CREATE OR REPLACE FUNCTION text(boolean)
       RETURNS text AS
$$
SELECT CASE WHEN $1 THEN 'true' ELSE 'false' END
$$
LANGUAGE 'sql';

-----------------------------------------------------------------------
-- For each vertex in the vertices table, set a point geometry which is
--  the corresponding line start or line end point
--
-- Last changes: 14.02.2008
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION add_vertices_geometry(geom_table varchar) 
       RETURNS VOID AS
$$
DECLARE
	vertices_table varchar := quote_ident(geom_table) || '_vertices';
BEGIN
	
	BEGIN
		EXECUTE 'SELECT addGeometryColumn(''' || 
                        quote_ident(vertices_table)  || 
                        ''', ''the_geom'', -1, ''POINT'', 2)';
	EXCEPTION 
		WHEN DUPLICATE_COLUMN THEN
	END;

	EXECUTE 'UPDATE ' || quote_ident(vertices_table) || 
                ' SET the_geom = NULL';

	EXECUTE 'UPDATE ' || quote_ident(vertices_table) || 
                ' SET the_geom = ST_StartPoint(ST_GeometryN(m.the_geom, 1)) FROM ' ||
                 quote_ident(geom_table) || 
                ' m where geom_id = m.source';

	EXECUTE 'UPDATE ' || quote_ident(vertices_table) || 
                ' set the_geom = ST_EndPoint(ST_GeometryN(m.the_geom, 1)) FROM ' || 
                quote_ident(geom_table) || 
                ' m where geom_id = m.target_id AND ' || 
                quote_ident(vertices_table) || 
                '.the_geom IS NULL';

	RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 

-----------------------------------------------------------------------
-- Update the cost column from the edges table, from the length of
--  all lines which belong to an edge.
--
-- Last changes: 14.02.2008
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION update_cost_from_distance(geom_table varchar) 
       RETURNS VOID AS
$$
DECLARE 
BEGIN
	BEGIN
	  EXECUTE 'CREATE INDEX ' || quote_ident(geom_table) || 
                  '_edge_id_idx ON ' || quote_ident(geom_table) || 
                  ' (edge_id)';
	EXCEPTION 
		WHEN DUPLICATE_TABLE THEN
		RAISE NOTICE 'Not creating index, already there';
	END;

	EXECUTE 'UPDATE ' || quote_ident(geom_table) || 
              '_edges SET cost = (SELECT sum( ST_Length( g.the_geom ) ) FROM ' || 
              quote_ident(geom_table) || 
              ' g WHERE g.edge_id = id GROUP BY id)';

	RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 
