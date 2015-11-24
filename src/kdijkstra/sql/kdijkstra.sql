-----------------------------------------------------------------------
-- Core function for one_to_many_dijkstra_shortest_path computation
-----------------------------------------------------------------------
--
--


CREATE OR REPLACE FUNCTION pgr_kdijkstracost(
    sql text,
    source_vid integer,
    target_vid integer array,
    directed boolean,
    has_reverse_cost boolean)
RETURNS SETOF pgr_costresult AS
$BODY$
DECLARE
has_rcost boolean;
edges_sql TEXT;
BEGIN
    RAISE NOTICE 'Deprecated function: Use pgr_dijkstraCost instead';
    has_rcost =_pgr_parameter_check('dijkstra', sql, false);
    edges_sql = sql;
    if (has_reverse_cost != has_rcost) then
        if (has_rcost) then -- has reverse_cost but the user doesnt want to use it, lets remove it
            edges_sql = 'SELECT id, source, target, cost FROM (' || edges_sql || ') a';
        else raise EXCEPTION 'has_reverse_cost set to true but reverse_cost not found';
        end if;
    end if;

    return query SELECT ((row_number() over()) -1)::integer  AS seq, a.start_vid::integer as id1, a.end_vid::integer as id2,  agg_cost as cost
    FROM pgr_dijkstraCost(edges_sql, source_vid, target_vid, directed) a;
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;


-- V2 signature
CREATE OR REPLACE FUNCTION pgr_kdijkstrapath(
    sql text,
    source_vid integer,
    target_vid integer array,
    directed boolean,
    has_reverse_cost boolean)
RETURNS SETOF pgr_costresult3 AS
$BODY$
DECLARE
has_rcost boolean;
edges_sql TEXT;
BEGIN
    RAISE NOTICE 'Deprecated function: Use pgr_dijkstra instead';
    has_rcost =_pgr_parameter_check('dijkstra', sql, false);
    edges_sql = sql;
    if (has_reverse_cost != has_rcost) then
        if (has_rcost) then -- has reverse_cost but the user doesnt want to use it, lets remove it
            edges_sql = 'SELECT id, source, target, cost FROM (' || edges_sql || ') a';
        else raise EXCEPTION 'has_reverse_cost set to true but reverse_cost not found';
        end if;
    end if;

    return query SELECT seq-1::integer as seq, end_vid::integer as id1, node::integer as id2, edge::integer as id3, cost
    FROM pgr_dijkstra(edges_sql, source_vid, target_vid, directed);
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
