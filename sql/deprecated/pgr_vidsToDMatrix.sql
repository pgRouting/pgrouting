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

CREATE OR REPLACE FUNCTION pgr_vidsToDMatrix(sql TEXT, vids  INTEGER[], dir BOOLEAN, has_rcost BOOLEAN, want_symmetric BOOLEAN)
RETURNS float8[] AS
$BODY$
DECLARE
directed BOOLEAN;
has_reverse BOOLEAN;
edges_sql TEXT;
dmatrix_row float8[];
dmatrix float8[];
cell RECORD;
unique_vids INTEGER[];
total BIGINT;
from_v BIGINT;
to_v BIGINT;
BEGIN
    RAISE NOTICE 'Deprecated function pgr_vidsToDMatrix';
    has_reverse =_pgr_parameter_check('dijkstra', sql, false);
    edges_sql = sql;
    IF (has_reverse != has_rcost) THEN
        IF (has_reverse) THEN
            sql = 'SELECT id, source, target, cost FROM (' || sql || ') a';
        ELSE
            raise EXCEPTION 'has_rcost set to true but reverse_cost not found';
        END IF;
    END IF;

    unique_vids :=  ARRAY(SELECT DISTINCT UNNEST(vids) ORDER BY 1);

    IF want_symmetric THEN
        directed = false;
    ELSE
        directed = dir;
    END IF;

    total := array_length(unique_vids, 1);

    -- initializing dmatrix
    FOR i in 1 .. total LOOP
        dmatrix_row := dmatrix_row || '+Infinity'::float8;
    END LOOP;
    FOR i in 1 .. total LOOP
    dmatrix := dmatrix || ARRAY[dmatrix_row];
    dmatrix[i][i] = 0;
    END LOOP;

    CREATE TEMP TABLE __x___y____temp AS
        WITH result AS
            (SELECT unnest(unique_vids) AS vid)
        SELECT row_number() OVER() AS idx, vid FROM result;

    FOR cell IN SELECT * FROM pgr_dijkstraCostMatrix(sql, unique_vids, directed) LOOP
        SELECT idx INTO from_v FROM __x___y____temp WHERE vid =  cell.start_vid;
        SELECT idx INTO to_v FROM __x___y____temp WHERE vid =  cell.end_vid;

        dmatrix[from_v][to_v] = cell.agg_cost;
        dmatrix[to_v][from_v] = cell.agg_cost;
    END LOOP;

    DROP TABLE IF EXISTS __x___y____temp;
    RETURN dmatrix;

    EXCEPTION WHEN others THEN
       DROP TABLE IF EXISTS __x___y____temp;
       raise exception '% %', SQLERRM, SQLSTATE;
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100;

