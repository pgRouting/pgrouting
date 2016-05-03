



-- V2 signature
CREATE OR REPLACE FUNCTION pgr_astar(edges_sql TEXT, source_id INTEGER, target_id INTEGER, directed BOOLEAN, has_rcost BOOLEAN)
RETURNS SETOF pgr_costresult AS
$BODY$
DECLARE
has_reverse BOOLEAN;
sql TEXT;
BEGIN
    RAISE NOTICE 'Deprecated signature of function pgr_astar';
    has_reverse =_pgr_parameter_check('astar', edges_sql, false);
    sql = edges_sql;
    IF (has_reverse != has_rcost) THEN
        IF (has_reverse) THEN
            sql = 'SELECT id, source, target, cost, x1,y1, x2, y2 FROM (' || edges_sql || ') a';
        ELSE
            raise EXCEPTION 'has_rcost set to true but reverse_cost not found';
        END IF;
    END IF;

    RETURN query SELECT * 
    FROM _pgr_astar(sql, source_id, target_id, directed, has_rcost);
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;
