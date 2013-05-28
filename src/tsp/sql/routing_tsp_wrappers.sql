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

-----------------------------------------------------
-- Returns TSP solution as a set of vertex ids
--
-- Last changes: 14.02.2008
-----------------------------------------------------
CREATE OR REPLACE FUNCTION pgr_tspIds(geom_table varchar, 
       ids varchar, source integer) 
       RETURNS SETOF integer AS
$$
DECLARE 
        r record;
        path_result record;
        v_id integer;
	prev integer;

BEGIN
	prev := -1;
	FOR path_result IN EXECUTE 'SELECT vertex_id FROM tsp(''select distinct source::integer as source_id, ST_X(ST_StartPoint(the_geom)), ST_Y(ST_StartPoint(the_geom)) from ' ||
		pgr_quote_ident(geom_table) || ' where source in (' || 
                ids || ')  UNION select distinct target as source_id, ST_X(ST_EndPoint(the_geom)), ST_Y(ST_EndPoint(the_geom)) from '|| pgr_quote_ident(geom_table) ||' where target in ('||ids||')'', '''|| ids  ||''', '|| source  ||')' LOOP

                v_id = path_result.vertex_id;
        RETURN NEXT v_id;
	END LOOP;

        RETURN;
END;
$$
LANGUAGE 'plpgsql' VOLATILE STRICT; 


