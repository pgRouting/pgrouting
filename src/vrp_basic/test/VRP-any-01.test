select * from pgr_vrpOneDepot(
    'select * from vrp_orders order by id'::text,
    'select * from vrp_vehicles order by vehicle_id'::text,
    'select * from vrp_distance order by src_id, dest_id'::text,
    1 ) ORDER BY vid, opos;
