/*PGR-GNU*****************************************************************

Copyright (c) 2016 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2016 Celia Virginia Vergara Castillo
mail: vicky_vergara@hotmail.com

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
/*
    Old signature has:
    sql: id INTEGER, x FLOAT, y FLOAT
*/


-----------------------
-- pgr_tsp
-----------------------


CREATE OR  REPLACE FUNCTION pgr_tsp(
    sql text, 
    start_id INTEGER,
    end_id INTEGER default (-1),

    OUT seq INTEGER,
    OUT id1 INTEGER,
    OUT id2 INTEGER,
    OUT cost FLOAT
)
returns setof record as
$body$
DECLARE
table_sql TEXT;
rec RECORD;
debuglevel TEXT;
n BIGINT;

BEGIN
    RAISE NOTICE 'Deprecated Signature pgr_tsp(sql, integer, integer)';

    table_sql := 'CREATE TEMP TABLE ___tmp ON COMMIT DROP AS ' || sql ;
    EXECUTE table_sql;


    BEGIN
        EXECUTE 'SELECT id, x, y FROM ___tmp' INTO rec;
        EXCEPTION
            WHEN OTHERS THEN
                RAISE EXCEPTION 'An expected column was not found in the query'
                USING ERRCODE = 'XX000',
                HINT = 'Please verify the column names: id, x, y';
    END;

    EXECUTE
    'SELECT
        pg_typeof(id)::text as id_type,
        pg_typeof(x)::text as x_type,
        pg_typeof(y)::text as y_type FROM ___tmp' INTO rec;


    IF NOT((rec.id_type in ('integer'::text))
        AND (rec.x_type = 'double precision'::text)
        AND (rec.y_type = 'double precision'::text)) THEN
            RAISE EXCEPTION '''id'' must be of type INTEGER, ''x'' ad ''y'' must be of type FLOAT'
            USING ERRCODE = 'XX000';
    END IF;

    EXECUTE 'SELECT count(*) AS n FROM (' || sql || ') AS __a__' INTO rec;
    n = rec.n;

    RETURN query
        SELECT (seq - 1)::INTEGER AS seq, node::INTEGER AS id1, node::INTEGER AS id2, cost
        FROM pgr_eucledianTSP(sql, start_id, end_id,

            tries_per_temperature :=  500 * n :: INTEGER,
            max_changes_per_temperature := 60 * n :: INTEGER,
            max_consecutive_non_changes := 500 * n :: INTEGER,

            randomize := false) WHERE seq <= n;
    DROP TABLE ___tmp;

END;
$body$
language plpgsql volatile STRICT cost 500 ROWS 50;


-- COMMENTS


COMMENT ON FUNCTION pgr_tsp(text, INTEGER, INTEGER)
IS 'DEPRECATED';
