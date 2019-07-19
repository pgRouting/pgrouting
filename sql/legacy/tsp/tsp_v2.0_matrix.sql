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


----------------
----------------
-- tsp
----------------
----------------


-----------------------
-- _pgr_unnest_matrix
-----------------------


CREATE OR REPLACE FUNCTION _pgr_unnest_matrix
    (matrix float8[][], 

    OUT start_vid integer, 
    OUT end_vid integer, 
    out agg_cost float8)
RETURNS SETOF record AS

$body$
DECLARE

m float8[];

BEGIN
    start_vid = 1;
    foreach m slice 1 in  ARRAY matrix
    LOOP
        end_vid = 1;
        foreach agg_cost in  ARRAY m
        LOOP
            RETURN next;
            end_vid = end_vid + 1;
        END LOOP;
        start_vid = start_vid + 1;
    END LOOP;
END;
$body$
language plpgsql volatile STRICT cost 500 ROWS 50;


CREATE OR REPLACE FUNCTION pgr_tsp
    (matrix float8[][],

    startpt INTEGER,
    endpt INTEGER DEFAULT -1,

    OUT seq INTEGER,
    OUT id INTEGER)
RETURNS SETOF record AS
$body$
DECLARE
table_sql TEXT;
debuglevel TEXT;
BEGIN
    RAISE NOTICE 'Deprecated Signature pgr_tsp(float8[][], integer, integer)';

    CREATE TEMP TABLE ___tmp2 ON COMMIT DROP AS SELECT * FROM _pgr_unnest_matrix( matrix );


    startpt := startpt + 1;
    IF endpt = -1 THEN endpt := startpt;
    END IF;


    RETURN QUERY
    WITH
    result AS (
        SELECT * FROM pgr_TSP(
        $$SELECT * FROM ___tmp2 $$,
        startpt, endpt,

        tries_per_temperature :=  500 :: INTEGER,
        max_changes_per_temperature := 30 :: INTEGER,
        max_consecutive_non_changes := 500 :: INTEGER,

        randomize:=false)
    )
    SELECT (row_number() over(ORDER BY result.seq) - 1)::INTEGER AS seq, (result.node - 1)::INTEGER AS id

    FROM result WHERE NOT(result.node = startpt AND result.seq != 1);

    DROP TABLE ___tmp2;
END;
$body$
language plpgsql volatile STRICT cost 500 ROWS 50;


-- COMMENTS


COMMENT ON FUNCTION _pgr_unnest_matrix(FLOAT[][])
IS 'DEPRECATED';


COMMENT ON FUNCTION pgr_tsp(FLOAT[][], INTEGER, INTEGER)
IS 'pgr_tsp
- Parameters
    - Matrix
    - startpt
    - endpt
- Documentation:
    - ${PGROUTING_DOC_LINK}/pgr_tsp.html
';
