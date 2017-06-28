
SELECT * FROM _pgr_vrpOneDepot(
    'SELECT * FROM vrp_orders',
    'SELECT * FROM vrp_vehicles',
    'SELECT src_id AS start_vid, dest_id AS end_vid, traveltime AS agg_cost FROM vrp_distance',
    1);
