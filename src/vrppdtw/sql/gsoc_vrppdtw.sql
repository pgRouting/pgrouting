CREATE OR REPLACE FUNCTION pgr_gsoc_vrppdtw(
    sql text,
    vehicle_num INTEGER,
    capacity INTEGER,
    OUT seq INTEGER,
    OUT rid INTEGER,
    OUT nid INTEGER,
    OUT cost INTEGER
)
RETURNS SETOF record AS
$BODY$
DECLARE
has_reverse BOOLEAN;
customers_sql TEXT;
BEGIN
    RETURN query
         SELECT a.seq, vehicle_id::INTEGER, stop_id::INTEGER AS id2, departure_time::INTEGER
        FROM _pgr_pickDeliver($1, $2, $3, 1, 30) AS a;
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;
