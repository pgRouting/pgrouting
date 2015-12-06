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
-----------------------------------------------------------------------
-- Core function for time_dependent_shortest_path computation
-- See README for description
-----------------------------------------------------------------------
--TODO - Do we need to add another sql text for the query on time-dependent-weights table?
--     - For now just checking with static data, so the query is similar to shortest_paths.

CREATE OR REPLACE FUNCTION _pgr_trsp(
    sql text, 
    source_vid integer, 
    target_vid integer, 
    directed boolean, 
    has_reverse_cost boolean, 
    turn_restrict_sql text DEFAULT null)
RETURNS SETOF pgr_costResult
AS '$libdir/${PGROUTING_LIBRARY_NAME}', 'turn_restrict_shortest_path_vertex'
LANGUAGE 'c' IMMUTABLE;

CREATE OR REPLACE FUNCTION pgr_trsp(
    sql text, 
    source_eid integer, 
    source_pos float8,
    target_eid integer,
    target_pos float8,
    directed boolean, 
    has_reverse_cost boolean, 
    turn_restrict_sql text DEFAULT null)
RETURNS SETOF pgr_costResult
AS '$libdir/${PGROUTING_LIBRARY_NAME}', 'turn_restrict_shortest_path_edge'
LANGUAGE 'c' IMMUTABLE;




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
        ELSE -- it does not have reverse_cost but user want's it
            RAISE EXCEPTION 'Error, reverse_cost is used, but query did''t return ''reverse_cost'' column'
            USING ERRCODE := 'XX000';
        END IF;
    END IF;

    IF (restrictions_sql IS NULL OR length(restrictions_sql) = 0) THEN
        -- no restrictions then its a dijkstra
        RETURN query SELECT seq-1 AS seq, node::integer AS id1, edge::integer AS id2, cost
        FROM pgr_dijkstra(new_sql, start_vid, end_vid, directed);
        RETURN;
    END IF;

    -- make the call without contradiction from part of the user
    RETURN query SELECT * FROM _pgr_trsp(new_sql, start_vid, end_vid, directed, has_rcost, restrictions_sql);
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
        ELSE -- it does not have reverse_cost but user want's it
            RAISE EXCEPTION 'Error, reverse_cost is used, but query did''t return ''reverse_cost'' column'
            USING ERRCODE := 'XX000';
        END IF;
    END IF;

    IF (restrictions_sql IS NULL OR length(restrictions_sql) = 0) THEN
        RETURN query SELECT (row_number() over())::INTEGER, path_id:: INTEGER, node::INTEGER,
            (CASE WHEN edge = -2 THEN -1 ELSE edge END)::INTEGER, cost
            FROM pgr_dijkstraViaVertex(new_sql, via_vids, directed, strict:=true) WHERE edge != -1;
        RETURN;
    END IF;
   

    -- make the call without contradiction from part of the user
    RETURN query SELECT * FROM _pgr_trspViaVertices(new_sql, via_vids, directed, has_rcost, restrictions_sql);
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;
