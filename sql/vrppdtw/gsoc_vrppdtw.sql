CREATE OR REPLACE FUNCTION pgr_gsoc_vrppdtw(
    text, -- SQL (required)
    INTEGER, -- vehicle_num (required)
    INTEGER, -- capacity (required)

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
LANGUAGE plpgsql VOLATILE STRICT
COST 100
ROWS 1000;

COMMENT ON FUNCTION pgr_gsoc_vrppdtw(TEXT, INTEGER, INTEGER)
IS 'pgr_vrpOneDepot
 - EXPERIMENTAL
 - Parameters
   - customer SQL with columns: id, x, y, demand, openTime, closeTime, serviceTime, pIndex, dIndex
   - number of vehicles
   - capacity of the vehicles';
