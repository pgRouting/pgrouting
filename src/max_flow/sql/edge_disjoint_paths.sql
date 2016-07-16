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
CREATE OR REPLACE FUNCTION pgr_edgedisjointpaths(
    IN edges_sql TEXT,
    IN source_vertex bigint,
    IN sink_vertex bigint,
    IN directed BOOLEAN DEFAULT TRUE,
    OUT number_of_paths bigint
    )
  AS
  $BODY$
  BEGIN
	number_of_paths := COALESCE((SELECT sum(flow)
	FROM pgr_maxflowpushrelabel('
	    SELECT id, source, target, CASE WHEN ' || CASE WHEN directed THEN 'going > 0' ELSE '(going > 0 OR coming > 0)' END || ' THEN 1 ELSE 0 END as capacity' || '
	                             , CASE WHEN ' || CASE WHEN directed THEN 'coming > 0' ELSE '(going > 0 OR coming > 0)' END  || ' THEN 1 ELSE 0 END as reverse_capacity
	    FROM (
	        SELECT * FROM _pgr_executehelper(_pgr_get_statement(''' || $1 || '''))
	        AS (id bigint, source bigint, target bigint, going double precision, coming double precision)
	    ) AS f', source_vertex, sink_vertex)
	WHERE source = source_vertex), 0) as paths_number;
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE;

CREATE OR REPLACE FUNCTION pgr_edgedisjointpaths(
    IN edges_sql TEXT,
    IN source_vertex ANYARRAY,
    IN sink_vertex bigint,
    IN directed BOOLEAN DEFAULT TRUE,
    OUT number_of_paths bigint
    )
  AS
  $BODY$
  BEGIN
	number_of_paths := COALESCE((SELECT sum(flow)
	FROM pgr_maxflowpushrelabel('
	    SELECT id, source, target, CASE WHEN ' || CASE WHEN directed THEN 'going > 0' ELSE '(going > 0 OR coming > 0)' END || ' THEN 1 ELSE 0 END as capacity' || '
	                             , CASE WHEN ' || CASE WHEN directed THEN 'coming > 0' ELSE '(going > 0 OR coming > 0)' END  || ' THEN 1 ELSE 0 END as reverse_capacity
	    FROM (
	        SELECT * FROM _pgr_executehelper(_pgr_get_statement(''' || $1 || '''))
	        AS (id bigint, source bigint, target bigint, going double precision, coming double precision)
	    ) AS f', source_vertex, sink_vertex)
	WHERE source = ANY(source_vertex)), 0) as paths_number;
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE;

CREATE OR REPLACE FUNCTION pgr_edgedisjointpaths(
    IN edges_sql TEXT,
    IN source_vertex bigint,
    IN sink_vertex ANYARRAY,
    IN directed BOOLEAN DEFAULT TRUE,
    OUT number_of_paths bigint
    )
  AS
  $BODY$
  BEGIN
	number_of_paths := COALESCE((SELECT sum(flow)
	FROM pgr_maxflowpushrelabel('
	    SELECT id, source, target, CASE WHEN ' || CASE WHEN directed THEN 'going > 0' ELSE '(going > 0 OR coming > 0)' END || ' THEN 1 ELSE 0 END as capacity' || '
	                             , CASE WHEN ' || CASE WHEN directed THEN 'coming > 0' ELSE '(going > 0 OR coming > 0)' END  || ' THEN 1 ELSE 0 END as reverse_capacity
	    FROM (
	        SELECT * FROM _pgr_executehelper(_pgr_get_statement(''' || $1 || '''))
	        AS (id bigint, source bigint, target bigint, going double precision, coming double precision)
	    ) AS f', source_vertex, sink_vertex)
	WHERE source = source_vertex), 0) as paths_number;
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE;

CREATE OR REPLACE FUNCTION pgr_edgedisjointpaths(
    IN edges_sql TEXT,
    IN source_vertex ANYARRAY,
    IN sink_vertex ANYARRAY,
    IN directed BOOLEAN DEFAULT TRUE,
    OUT number_of_paths bigint
    )
  AS
  $BODY$
  BEGIN
	number_of_paths := COALESCE((SELECT sum(flow)
	FROM pgr_maxflowpushrelabel('
	    SELECT id, source, target, CASE WHEN ' || CASE WHEN directed THEN 'going > 0' ELSE '(going > 0 OR coming > 0)' END || ' THEN 1 ELSE 0 END as capacity' || '
	                             , CASE WHEN ' || CASE WHEN directed THEN 'coming > 0' ELSE '(going > 0 OR coming > 0)' END  || ' THEN 1 ELSE 0 END as reverse_capacity
	    FROM (
	        SELECT * FROM _pgr_executehelper(_pgr_get_statement(''' || $1 || '''))
	        AS (id bigint, source bigint, target bigint, going double precision, coming double precision)
	    ) AS f', source_vertex, sink_vertex)
	WHERE source = ANY(source_vertex)), 0) as paths_number;
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE;