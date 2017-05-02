-------------------------------------------------------------------------------
-- Legacy FUNCTIONs that were meant to be used by
-- Temporarily while the rewrite of 2.0
-- These FUNCTIONs wer used on the plpgsql functions


-- FILE intended to be used on 3.0
-------------------------------------------------------------------------------

-- I do not know yet if they will create a conflict when putting the legacy

-- Deprecated signature on 2.1.0
CREATE OR REPLACE FUNCTION pgr_dijkstra(
    edges_sql TEXT,
    start_vid INTEGER,
    end_vid INTEGER,
    directed BOOLEAN,
    has_rcost BOOLEAN)
RETURNS SETOF pgr_costresult AS
$BODY$
    SELECT seq-1 , node::integer, edge::integer, cost
    FROM pgr_dijkstra($1, ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], directed);
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;




-- Deprecated signature on 2.1.0
CREATE OR REPLACE FUNCTION pgr_drivingDistance(edges_sql text, source INTEGER, distance FLOAT8, directed BOOLEAN, has_rcost BOOLEAN)
RETURNS SETOF pgr_costresult AS
$BODY$
    SELECT seq - 1, node::integer, edge::integer, agg_cost
    FROM pgr_drivingDistance($1, ARRAY[$2]::BIGINT[], $3, $4);
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;




-- Deprecated signature on 2.1.0
CREATE OR REPLACE FUNCTION pgr_ksp(edges_sql text, start_vid integer, end_vid integer, k integer, has_rcost boolean)
RETURNS SETOF pgr_costresult3 AS
$BODY$
    SELECT ((row_number() over()) -1)::integer,  (path_id - 1)::integer, node::integer, edge::integer, cost
    FROM _pgr_ksp($1::text, $2, $3, $4, TRUE, FALSE) WHERE path_id <= k;
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;


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


-- Deprecated on 2.2.0
CREATE OR REPLACE FUNCTION pgr_kdijkstraPath(
    sql text,
    source INTEGER,
    targets INTEGER ARRAY,
    directed BOOLEAN,
    has_rcost BOOLEAN)
RETURNS SETOF pgr_costResult3 AS
$BODY$
    SELECT (row_number() over () -1)::integer, end_vid::INTEGER, node::INTEGER, edge::INTEGER, cost
    FROM pgr_dijkstra($1, $2, $3, $4);
$BODY$
LANGUAGE sql VOLATILE
COST 100
ROWS 1000;


-- Deprecated on 2.2.0
CREATE OR REPLACE FUNCTION pgr_kdijkstracost(
    sql text,
    source INTEGER,
    targets INTEGER array,
    directed BOOLEAN,
    has_rcost BOOLEAN)
RETURNS SETOF pgr_costResult AS
$BODY$
    SELECT (row_number() over () -1)::integer, start_vid::integer, end_vid::INTEGER, agg_cost
    FROM pgr_dijkstraCost($1, $2, $3, $4);
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


-- included on 2.3.0
-- Deprecated on 2.5.0
CREATE OR REPLACE FUNCTION pgr_maxFlowPushRelabel(
    edges_sql TEXT,
    source_vertex BIGINT,
    sink_vertex BIGINT,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT * FROM pgr_PushRelabel($1, $2, $3);
  $BODY$
  LANGUAGE sql VOLATILE;




-- included on 2.3.0
-- Deprecated on 2.5.0
CREATE OR REPLACE FUNCTION pgr_maxFlowBoykovKolmogorov(
    edges_sql TEXT,
    source_vertex BIGINT,
    sink_vertex BIGINT,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT * FROM pgr_boykovKolmogorov($1, $2, $3);
  $BODY$
  LANGUAGE sql VOLATILE;



-- included on 2.3.0
-- Deprecated on 2.5.0
CREATE OR REPLACE FUNCTION pgr_maxFlowEdmondsKarp(
    edges_sql TEXT,
    source_vertex BIGINT,
    sink_vertex BIGINT,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT * FROM pgr_edmondsKarp($1, $2, $3);
  $BODY$
  LANGUAGE sql VOLATILE;



-- included on 2.3.0
-- Deprecated on 2.5.0
CREATE OR REPLACE FUNCTION pgr_maxFlowPushRelabel(
    edges_sql TEXT,
    source_vertex BIGINT,
    sink_vertices ANYARRAY,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT * FROM pgr_PushRelabel($1, $2, $3);
  $BODY$
  LANGUAGE SQL VOLATILE;



-- included on 2.3.0
-- Deprecated on 2.5.0
CREATE OR REPLACE FUNCTION pgr_maxFlowBoykovKolmogorov(
    edges_sql TEXT,
    source_vertex BIGINT,
    sink_vertices ANYARRAY,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT * FROM pgr_boykovKolmogorov($1, $2, $3);
  $BODY$
  LANGUAGE SQL VOLATILE;



-- included on 2.3.0
-- Deprecated on 2.5.0
CREATE OR REPLACE FUNCTION pgr_maxFlowEdmondsKarp(
    edges_sql TEXT,
    source_vertex BIGINT,
    sink_vertices ANYARRAY,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
       SELECT * FROM pgr_edmondsKarp($1, $2, $3);
  $BODY$
  LANGUAGE SQL VOLATILE;




-- included on 2.3.0
-- Deprecated on 2.5.0
CREATE OR REPLACE FUNCTION pgr_maxFlowPushRelabel(
    edges_sql TEXT,
    source_vertices ANYARRAY,
    sink_vertex BIGINT,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT * FROM pgr_PushRelabel($1, $2, $3);
  $BODY$
  LANGUAGE SQL VOLATILE;



-- included on 2.3.0
-- Deprecated on 2.5.0
CREATE OR REPLACE FUNCTION pgr_maxFlowBoykovKolmogorov(
    edges_sql TEXT,
    source_vertices ANYARRAY,
    sink_vertex BIGINT,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT * FROM pgr_boykovKolmogorov($1, $2, $3);
  $BODY$
  LANGUAGE SQL VOLATILE;



-- included on 2.3.0
-- Deprecated on 2.5.0
CREATE OR REPLACE FUNCTION pgr_maxFlowEdmondsKarp(
    edges_sql TEXT,
    source_vertices ANYARRAY,
    sink_vertex BIGINT,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT * FROM pgr_edmondsKarp($1, $2, $3);
  $BODY$
  LANGUAGE SQL VOLATILE;




-- included on 2.3.0
-- Deprecated on 2.5.0
CREATE OR REPLACE FUNCTION pgr_maxFlowPushRelabel(
    edges_sql TEXT,
    source_vertices ANYARRAY,
    sink_vertices ANYARRAY,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT * FROM pgr_PushRelabel($1, $2, $3);
  $BODY$
  LANGUAGE SQL VOLATILE;



-- included on 2.3.0
-- Deprecated on 2.5.0
CREATE OR REPLACE FUNCTION pgr_maxFlowBoykovKolmogorov(
    edges_sql TEXT,
    source_vertices ANYARRAY,
    sink_vertices ANYARRAY,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT * FROM pgr_boykovKolmogorov($1, $2, $3);
  $BODY$
  LANGUAGE SQL VOLATILE;



-- included on 2.3.0
-- Deprecated on 2.5.0
CREATE OR REPLACE FUNCTION pgr_maxFlowEdmondsKarp(
    edges_sql TEXT,
    source_vertices ANYARRAY,
    sink_vertices ANYARRAY,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT * FROM pgr_edmondsKarp($1, $2, $3);
  $BODY$
  LANGUAGE SQL VOLATILE;


-- included on 2.3.0
-- Deprecated on 2.5.0
CREATE OR REPLACE FUNCTION pgr_maximumcardinalitymatching(
    edges_sql TEXT,
    directed BOOLEAN DEFAULT TRUE,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT * FROM pgr_maxCardinalityMatch($1, $2);
  $BODY$
  LANGUAGE SQL VOLATILE;



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

