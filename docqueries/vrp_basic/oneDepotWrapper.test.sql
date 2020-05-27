SET extra_float_digits=-3;

SELECT * FROM _pgr_vrpOneDepot(
    'SELECT * FROM solomon_100_RC_101',
    'SELECT * FROM vrp_vehicles',
    'SELECT src_id AS start_vid, dest_id AS end_vid, traveltime AS agg_cost FROM vrp_distance',
    1);
