
SELECT * FROM pgr_vrpOneDepot(
    'SELECT * FROM vrp_orders',
    'SELECT * FROM vrp_vehicles',
    'SELECT * FROM vrp_distance',
    1);
