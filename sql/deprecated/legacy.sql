-------------------------------------------------------------------------------
-- Legacy FUNCTIONs that were meant to be used by
-- Temporarily while the rewrite of 2.0
-- These FUNCTIONs wer used on the plpgsql functions


-- FILE intended to be used on 3.0
-------------------------------------------------------------------------------

-- I do not know yet if they will create a conflict when putting the legacy




------------------------------------------------------------------------------

-- Deprecated on 2.2.0
CREATE OR REPLACE FUNCTION pgr_apspJohnson(edges_sql text)
RETURNS SETOF pgr_costResult AS
$BODY$
    SELECT (row_number() over () - 1)::integer, start_vid::integer, end_vid::integer, agg_cost
    FROM  pgr_johnson($1, TRUE);
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;




-- Deprecated on 2.2.0
CREATE OR REPLACE FUNCTION pgr_apspWarshall(edges_sql text, directed boolean, has_rcost boolean)
    RETURNS SETOF pgr_costResult AS
$BODY$
    SELECT (row_number() over () -1)::integer, start_vid::integer, end_vid::integer, agg_cost
    FROM  pgr_floydWarshall($1, $2);
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;





-- Deprecated on 2.3.0
CREATE OR REPLACE FUNCTION pgr_astar(edges_sql TEXT, source_id INTEGER, target_id INTEGER, directed BOOLEAN, has_rcost BOOLEAN)
RETURNS SETOF pgr_costresult AS
$BODY$
    SELECT seq - 1, node::INTEGER, edge::INTEGER, cost
    FROM pgr_astar($1, ARRAY[$2], ARRAY[$3], $4);
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;



-- Deprecated on 2.4.0
CREATE OR REPLACE FUNCTION pgr_bdDijkstra(edges_sql TEXT, start_vid INTEGER, end_vid INTEGER, directed BOOLEAN, has_rcost BOOLEAN)
RETURNS SETOF pgr_costresult AS
$BODY$
DECLARE
has_reverse BOOLEAN;
sql TEXT;
BEGIN
    SELECT seq - 1 AS seq, node::integer AS id1, edge::integer AS id2, cost
    FROM pgr_bdDijkstra($1, ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], $4);
  END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;




-- Deprecated on 2.5.0
CREATE OR REPLACE FUNCTION pgr_bdAstar(
    edges_sql TEXT,
    start_vid INTEGER,
    end_vid INTEGER,
    directed BOOLEAN,
    has_rcost BOOLEAN)
RETURNS SETOF pgr_costresult AS
$BODY$
    SELECT seq - 1 AS seq, node::integer AS id1, edge::integer AS id2, cost
    FROM _pgr_bdAstar($1, ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], $4);
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;

