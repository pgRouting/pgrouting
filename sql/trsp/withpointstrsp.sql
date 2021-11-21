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

/*
    Wrapper change on v2.6
    - without restrictions
        - call pgr_dijkstra when both end points have a fraction IN (0,1)
        - call pgr_withPoints when at least one fraction NOT IN (0,1)

    - with restrictions
      - calls original trsp code
*/

/* TODO

* This one should be
* pgr_withPointsTRSP

* The restrictions sql should be the second parameter
*

*/
--v3.0
CREATE FUNCTION pgr_withPointsTRSP(
    TEXT,    -- sql (required)
    INTEGER, -- source_eid (required)
    FLOAT,   -- source_pos (required)
    INTEGER, -- target_eid (required)
    FLOAT,   -- target_pos (required)
    BOOLEAN, -- directed (required)
    BOOLEAN, -- has_reverse_cost (required)

    turn_restrict_sql text DEFAULT null,

    OUT seq INTEGER,
    OUT id1 INTEGER,
    OUT id2 INTEGER,
    OUT cost FLOAT
)
RETURNS SETOF record AS
$BODY$
DECLARE
    sql TEXT                 := $1;
    source_eid INTEGER       := $2;
    source_pos FLOAT         := $3;
    target_eid INTEGER       := $4;
    target_pos FLOAT         := $5;
    directed BOOLEAN         := $6;
    has_reverse_cost BOOLEAN := $7;

has_reverse BOOLEAN;
new_sql TEXT;
trsp_sql TEXT;
source_sql TEXT;
target_sql TEXT;
union_sql TEXT;
union_sql1 TEXT;
union_sql2 TEXT;
final_sql TEXT;

BEGIN
    IF $2 IS NULL OR $3 IS NULL OR $4 IS NULL OR $5 IS NULL OR $6 IS NULL THEN
        RETURN;
    END IF;
    /*
    has_reverse =_pgr_parameter_check('dijkstra', sql, false);

    new_sql := sql;
    IF (has_reverse != has_reverse_cost) THEN  -- user contradiction
        IF (has_reverse) THEN
            -- it has reverse_cost but user don't want it.
            -- to be on the safe side because it reads the data wrong, sending only postitive values
            new_sql :=
            'WITH old_sql AS (' || sql || ')' ||
            '   SELECT id, source, target, cost FROM old_sql';
        ELSE -- it does not have reverse_cost but user wants it
            RAISE EXCEPTION 'Error, reverse_cost is used, but query did''t return ''reverse_cost'' column'
            USING ERRCODE := 'XX000';
        END IF;
    END IF;
    */

    IF (turn_restrict_sql IS NULL OR length(turn_restrict_sql) = 0) THEN
        -- no restrictions then its a withPoints or dijkstra
        IF source_pos = 0 THEN
            source_sql = '(SELECT source FROM (' || sql || ') b WHERE id = ' ||  source_eid || ')';
        ELSE IF source_pos = 1 THEN
            source_sql = '(SELECT target FROM (' || sql || ') b WHERE id = ' || source_eid || ')';
          ELSE
              source_sql = '-1';
              union_sql1 =  '(SELECT 1 as pid, ' || source_eid || ' as edge_id, ' || source_pos || '::float8 as fraction)';
          END IF;
        END IF;

        -- raise notice 'source_sql %', source_sql;
        -- raise notice 'union_sql1 %', union_sql1;


        IF target_pos = 0 THEN
            target_sql = '(SELECT source FROM (' || sql || ') c WHERE id = ' ||  target_eid || ')';
        ELSE IF target_pos = 1 THEN
            target_sql = '(SELECT target FROM (' || sql || ') c WHERE id = ' ||  target_eid || ')';
        ELSE
            target_sql = '-2';
            union_sql2 =  ' (SELECT 2 as pid, ' || target_eid || ' as edge_id, ' || target_pos || '::float8 as fraction)';
        END IF;
        END IF;

        -- raise notice 'target_sql %', target_sql;
        -- raise notice 'union_sql2 %', union_sql2;

        IF union_sql1 IS NOT NULL AND union_sql2 IS NOT NULL THEN
            union_sql = union_sql1 || ' UNION ' || union_sql2;
        ELSE IF union_sql1 IS NOT NULL AND union_sql2 IS NULL THEN
            union_sql = union_sql1;
        ELSE IF union_sql1 IS NULL AND union_sql2 IS NOT NULL THEN
            union_sql = union_sql2;
        END IF;
        END IF;
        END IF;

        IF union_sql IS NULL THEN
            -- no points then its a dijkstra
            RAISE WARNING 'executing pgr_dijkstra';
            final_sql = 'WITH final_sql AS (
                 SELECT  a.seq-1 AS seq, node::INTEGER AS id1, edge::INTEGER AS id2, cost FROM pgr_dijkstra($$' || $1 || '$$
                ,' || source_sql || '
                ,' || target_sql || '
                , directed := ' || directed || '
            ) a )
            SELECT seq, id1, id2, cost  FROM final_sql ORDER BY seq';
        ELSE
            -- points then its a withPoints
            RAISE WARNING 'executing pgr_withpoints';
            final_sql = 'WITH final_sql AS (
                SELECT  a.seq-1 AS seq, node::INTEGER AS id1, edge::INTEGER AS id2, cost FROM pgr_withpoints($$' || $1 || '$$
                , $$' || union_sql || '$$
                ,' || source_sql || '
                ,' || target_sql || '
                , directed := ' || directed || '
            ) a )
            SELECT seq, CASE WHEN seq = 0 AND ' || source_pos || '=0 THEN id1
                             WHEN seq = 0 AND ' || source_pos || '!=0 THEN -1
                             WHEN id2 = -1 AND ' || target_pos || '=0 THEN id1
                             WHEN id2 = -1 AND ' || target_pos || '!=0 THEN id1
                             ELSE id1 END AS id1, id2, cost  FROM final_sql ORDER BY seq';
        END IF;


        -- raise notice 'final_sql %', final_sql;
        RETURN QUERY EXECUTE final_sql;
        RETURN;

    END IF;

    -- with restrictions calls the original code
    RETURN query SELECT * FROM _pgr_withpointstrsp($1, source_eid, source_pos, target_eid, target_pos, directed, has_reverse_cost, turn_restrict_sql);
    RETURN;

END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;

-- COMMENTS

COMMENT ON FUNCTION pgr_withPointsTRSP(TEXT, INTEGER, FLOAT, INTEGER, FLOAT, BOOLEAN, BOOLEAN, TEXT)
IS 'pgr_withPointsTRSP
- Parameters
    - edges SQL with columns: id, source, target, cost [,reverse_cost]
    - source edge identifier
    - fraction position on source edge
    - target edge identifier
    - fraction position on target edge
    - directed
    - has reverse cost
- Optional parameters
    - turn_restrict_sql := NULL
- Documentation:
    - ${PROJECT_DOC_LINK}/pgr_trsp.html
';
