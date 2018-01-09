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
-----------------------------------------------------------------------
-- Core function for time_dependent_shortest_path computation
-- See README for description
-----------------------------------------------------------------------
--TODO - Do we need to add another sql text for the query on time-dependent-weights table?
--     - For now just checking with static data, so the query is similar to shortest_paths.
*/


CREATE OR REPLACE FUNCTION _pgr_trsp(
    sql text,
    source_eid integer,
    source_pos float8,
    target_eid integer,
    target_pos float8,
    directed boolean,
    has_reverse_cost boolean,
    turn_restrict_sql text DEFAULT null)
RETURNS SETOF pgr_costResult
AS '${MODULE_PATHNAME}', 'turn_restrict_shortest_path_edge'
LANGUAGE 'c' IMMUTABLE;


CREATE OR REPLACE FUNCTION _pgr_array_reverse(anyarray) RETURNS anyarray AS $$
SELECT ARRAY(
    SELECT $1[i]
    FROM generate_subscripts($1,1) AS s(i)
    ORDER BY i DESC
);
$$ LANGUAGE 'sql' STRICT IMMUTABLE;


/*  pgr_trsp    VERTEX

 - if size of restrictions_sql  is Zero or no restrictions_sql are given
     then call to pgr_dijkstra is made

 - because it reads the data wrong, when there is a reverse_cost column:
   - put all data costs in one cost column and
   - a call is made to trsp without only the positive values
*/
CREATE OR REPLACE FUNCTION pgr_trsp(
    edges_sql TEXT,
    start_vid INTEGER,
    end_vid INTEGER,
    directed BOOLEAN,
    has_rcost BOOLEAN,
    restrictions_sql TEXT DEFAULT NULL)
RETURNS SETOF pgr_costResult AS
$BODY$
DECLARE
has_reverse BOOLEAN;
new_sql TEXT;
restrictions_query TEXT;
trsp_sql TEXT;
BEGIN
    has_reverse =_pgr_parameter_check('dijkstra', edges_sql, false);

    new_sql := edges_sql;
    IF (has_reverse != has_rcost) THEN  -- user contradiction
        IF (has_reverse) THEN  -- it has reverse_cost but user don't want it.
            -- to be on the safe side because it reads the data wrong, sending only postitive values
            new_sql :=
            'WITH old_sql AS (' || edges_sql || ')' ||
            '   SELECT id, source, target, cost FROM old_sql';
        ELSE -- it does not have reverse_cost but user wants it
            RAISE EXCEPTION 'Error, reverse_cost is used, but query did''t return ''reverse_cost'' column'
            USING ERRCODE := 'XX000';
        END IF;
    END IF;

    IF (restrictions_sql IS NULL OR length(restrictions_sql) = 0) THEN
        -- no restrictions then its a dijkstra
        RETURN query SELECT a.seq - 1 AS seq, node::INTEGER AS id1, edge::INTEGER AS id2, cost
        FROM pgr_dijkstra(new_sql, start_vid, end_vid, directed) a;
        RETURN;
    END IF;


    restrictions_query = $$
        WITH old_restrictions AS ( $$ ||
            $6 || $$
        )
        SELECT ROW_NUMBER() OVER() AS id,
            _pgr_array_reverse(array_prepend(target_id, string_to_array(via_path, ',')::INTEGER[])) AS path,
            to_cost AS cost
        FROM old_restrictions;
    $$;



    RETURN query
        SELECT (seq - 1)::INTEGER, a.node::INTEGER, a.edge::INTEGER, a.cost
        FROM _pgr_trsp(new_sql, restrictions_query, start_vid, end_vid, directed) AS a;
    IF NOT FOUND THEN
        RAISE EXCEPTION 'Error computing path: Path Not Found';
    END IF;

END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;


/* pgr_trspVia Vertices
 - if size of restrictions_sql  is Zero or no restrictions_sql are given
     then call to pgr_dijkstra is made

 - because it reads the data wrong, when there is a reverse_cost column:
   - put all data costs in one cost column and
   - a call is made to trspViaVertices without only the positive values
*/
CREATE OR REPLACE FUNCTION pgr_trspViaVertices(
    edges_sql TEXT,
    via_vids ANYARRAY,
    directed BOOLEAN,
    has_rcost BOOLEAN,
    restrictions_sql TEXT DEFAULT NULL)
RETURNS SETOF pgr_costResult3 AS
$BODY$
DECLARE
has_reverse BOOLEAN;
new_sql TEXT;
BEGIN

    has_reverse =_pgr_parameter_check('dijkstra', edges_sql, false);

    new_sql := edges_sql;
    IF (has_reverse != has_rcost) THEN  -- user contradiction
        IF (has_reverse) THEN  -- it has reverse_cost but user don't want it.
            new_sql :=
               'WITH old_sql AS (' || edges_sql || ')' ||
                '   SELECT id, source, target, cost FROM old_sql';
        ELSE -- it does not have reverse_cost but user wants it
            RAISE EXCEPTION 'Error, reverse_cost is used, but query did''t return ''reverse_cost'' column'
            USING ERRCODE := 'XX000';
        END IF;
    END IF;

    IF (restrictions_sql IS NULL OR length(restrictions_sql) = 0) THEN
        RETURN query SELECT (row_number() over())::INTEGER, path_id:: INTEGER, node::INTEGER,
            (CASE WHEN edge = -2 THEN -1 ELSE edge END)::INTEGER, cost
            FROM pgr_dijkstraVia(new_sql, via_vids, directed, strict:=true) WHERE edge != -1;
        RETURN;
    END IF;


    -- make the call without contradiction from part of the user
    RETURN query SELECT * FROM _pgr_trspViaVertices(new_sql, via_vids::INTEGER[], directed, has_rcost, restrictions_sql);
    IF NOT FOUND THEN
        RAISE EXCEPTION 'Error computing path: Path Not Found';
    END IF;
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;

/*
    Wrapper change on v2.6
    - without restrictions
        - call pgr_dijkstra when both end points have a fraction IN (0,1)
        - call pgr_withPoints when at least one fraction NOT IN (0,1)

    - with restrictions
      - calls original trsp code
*/
CREATE OR REPLACE FUNCTION pgr_trsp(
    sql text,
    source_eid integer,
    source_pos float8,
    target_eid integer,
    target_pos float8,
    directed boolean,
    has_reverse_cost boolean,
    turn_restrict_sql text DEFAULT null)
RETURNS SETOF pgr_costResult AS
$BODY$
DECLARE
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
            final_sql = 'WITH final_sql AS (
                 SELECT  a.seq-1 AS seq, node::INTEGER AS id1, edge::INTEGER AS id2, cost FROM pgr_dijkstra($$' || new_sql || '$$
                ,' || source_sql || '
                ,' || target_sql || '
                , directed := ' || directed || '
            ) a )
            SELECT seq, id1, id2, cost  FROM final_sql ORDER BY seq';
        ELSE
            -- points then its a withPoints
            final_sql = 'WITH final_sql AS (
                SELECT  a.seq-1 AS seq, node::INTEGER AS id1, edge::INTEGER AS id2, cost FROM pgr_withpoints($$' || new_sql || '$$
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
    RETURN query SELECT * FROM _pgr_trsp(new_sql, source_eid, source_pos, target_eid, target_pos, directed, has_reverse_cost, turn_restrict_sql);
    RETURN;

END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;
