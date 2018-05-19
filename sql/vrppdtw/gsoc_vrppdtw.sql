CREATE OR REPLACE FUNCTION pgr_gsoc_vrppdtw(
    sql text,
    vehicle_num INTEGER,
    capacity INTEGER,

    OUT seq INTEGER,
    OUT id1 INTEGER,
    OUT id2 INTEGER,
    OUT cost FLOAT
)
RETURNS SETOF record AS
$BODY$
DECLARE
has_reverse BOOLEAN;
customers_sql TEXT;
BEGIN
    RETURN query
    SELECT a.seq, vehicle_id::INTEGER AS id1, stop_id::INTEGER AS id2, departure_time AS cost
    FROM _pgr_gsoc_vrppdtw($1, $2, $3, 1, 30) AS a WHERE vehicle_id NOT IN (-2);
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;
