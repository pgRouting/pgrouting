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
along with this program; IF not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

********************************************************************PGR-GNU*/

CREATE OR REPLACE FUNCTION pgr_kdijkstrapath(
    sql text,
    source_vid integer,
    target_vid integer array,
    directed boolean,
    has_reverse_cost boolean)
    RETURNS SETOF pgr_costResult3
    AS '$libdir/${PGROUTING_LIBRARY_NAME}', 'onetomany_dijkstra_path'
    LANGUAGE C STABLE STRICT;


CREATE OR REPLACE FUNCTION pgr_kdijkstracost(
    edges_sql text,
    start_vid INTEGER,
    end_vids INTEGER array,
    directed BOOLEAN,
    has_rcost BOOLEAN)
    RETURNS SETOF pgr_costResult AS
    $BODY$
    DECLARE
    has_reverse BOOLEAN;
    new_sql TEXT;
    BEGIN
        RAISE NOTICE 'Deprecated function';
        has_reverse =_pgr_parameter_check('dijkstra', edges_sql, false);
        new_sql = edges_sql;

        IF (has_reverse != has_rcost) THEN
            IF (has_reverse) THEN
                new_sql = 'SELECT id, source, target, cost FROM (' || edges_sql || ') a';
            else
                raise EXCEPTION 'has_rcost set to true but reverse_cost not found';
           END IF;
        END IF;

        return query SELECT ((row_number() over()) -1)::INTEGER, a.start_vid::INTEGER, a.end_vid::INTEGER, agg_cost
        FROM pgr_dijkstraCost(new_sql, start_vid, end_vids, directed) a;
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;

CREATE OR REPLACE FUNCTION pgr_vidsToDMatrix(sql text,
    vids integer[], dir bool, has_rcost bool, want_symmetric bool)
    RETURNS float8[]
    AS '$libdir/${PGROUTING_LIBRARY_NAME}', 'manytomany_dijkstra_dmatrix'
    LANGUAGE C STABLE STRICT;
