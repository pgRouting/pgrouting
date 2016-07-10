/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2016 Andrea Nardelli
mail: nrd.nardelli@gmail.com

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

--INTERNAL FUNCTIONS
CREATE OR REPLACE FUNCTION _pgr_executehelper(
    IN edges_sql TEXT
    )
  RETURNS SETOF RECORD AS
  $BODY$
  DECLARE
	r RECORD;
  BEGIN
	FOR r in EXECUTE $1 LOOP
		RETURN NEXT r;
	END LOOP;
	RETURN;
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE;

CREATE OR REPLACE FUNCTION pgr_edgedisjointpaths(
    IN edges_sql TEXT,
    IN source_vertex bigint,
    IN sink_vertex bigint,
    IN directed BOOLEAN DEFAULT TRUE,
    OUT paths_number bigint
    )
  AS
  $BODY$
  BEGIN
	paths_number := COALESCE((SELECT sum(flow)
	FROM pgr_maxflowpushrelabel('
	    SELECT id, source, target, 1 as capacity, ' || CASE WHEN directed THEN '0' ELSE '1' END || ' AS reverse_capacity
	    FROM (
	        SELECT * FROM _pgr_executehelper(_pgr_get_statement(''' || $1 || '''))
	        AS (id bigint, source bigint, target bigint)
	    ) AS f', source_vertex, sink_vertex)
	WHERE source = source_vertex GROUP BY source_vertex), 0) as paths_number;
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE;

