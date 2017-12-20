SELECT plan(1);

SELECT bag_has(
  $$SELECT * FROM pgr_lineGraphFull('select id, source, target, cost, reverse_cost from edge_table where id =1')$$,
  $$SELECT * FROM pgr_lineGraphFull(
    'select id, source, target, cost from edge_table where id = 1
     UNION ALL
     select -1*id, target AS source, source AS target, reverse_cost AS cost from edge_table where id = 1')$$
);
