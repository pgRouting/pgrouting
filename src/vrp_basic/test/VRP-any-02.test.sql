select * from pgr_vrpOneDepot(
  'select * from vrp100_orders order by id'::text,
  'select vehicle_id::integer, 200 as capacity from generate_series(1,50) as vehicle_id',
  'select a.id as src_id, b.id as dest_id, sqrt((a.x-b.x)*(a.x-b.x)) as cost, sqrt((a.x-b.x)*(a.x-b.x)) as distance, sqrt((a.x-b.x)*(a.x-b.x)) as traveltime from vrp100_orders a, vrp100_orders b where a.id != b.id order by a.id, b.id', 0) order by vid, opos;
