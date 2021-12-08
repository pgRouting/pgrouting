

    ------------------------------------------------------------------------------------------------------
    ------------------------------------------------------------------------------------------------------
    --              PGR_pgr_trsp
    ------------------------------------------------------------------------------------------------------
    ------------------------------------------------------------------------------------------------------

/* --q1 */
SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost FROM edge_table$$,
  $$SELECT * FROM new_restrictions$$,
  7, 12, false
);
SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost FROM edge_table$$,
  $$SELECT * FROM new_restrictions$$,
  7, ARRAY[12,3], false
);
SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost FROM edge_table$$,
  $$SELECT * FROM new_restrictions$$,
  ARRAY[12,3], 7, false
);
SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost FROM edge_table$$,
  $$SELECT * FROM new_restrictions$$,
  ARRAY[12,3]::BIGINT[], ARRAY[7,1]::BIGINT[], false
);
SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost FROM edge_table$$,
  $$SELECT * FROM new_restrictions$$,
  $$SELECT * FROM combinations_table$$
);
/* On a directed graph by default */
SELECT * FROM pgr_trsp(
  'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table',
  $$SELECT * FROM new_restrictions$$,
  7, 12
);
SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost FROM edge_table$$,
  $$SELECT 1 AS id, 100::float AS cost, 25::INTEGER AS target_id, ARRAY[33, 32, 25] AS path$$,
  7, 12, false
);
SELECT * FROM pgr_trsp(
  'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table',
  $$SELECT 1 AS id, 100::float AS cost, 25::INTEGER AS target_id, ARRAY[33, 32, 25] AS path$$,
  7, 12
);
SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost FROM edge_table$$,
  $$SELECT * FROM new_restrictions WHERE id > 7$$,
  7, 12, false
);
SELECT * FROM pgr_trsp(
  'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table',
  $$SELECT * FROM new_restrictions WHERE id > 7$$,
  7, 12
);

/* --q2 */

/* --q3 */
SELECT * FROM pgr_trsp(
  $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table$$,
  $$SELECT 1 AS id, 100::float AS cost, 25::INTEGER AS target_id, ARRAY[33, 32, 25] AS path$$,
  2, 7, false
);
SELECT * FROM pgr_trsp(
  $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table$$,
  $$SELECT * FROM new_restrictions$$,
  7, 11, false
);
SELECT * FROM pgr_trsp(
  $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table$$,
  $$SELECT * FROM new_restrictions WHERE id > 7$$,
  7, 11, false
);


/* --q4 */
SELECT * FROM pgr_trspViaVertices(
  'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table',
  ARRAY[2,7,11]::INTEGER[],
  false,  false,
  'SELECT to_cost, target_id::int4, from_edge ||
  coalesce('',''||via_path,'''') AS via_path FROM restrictions');


/* --q5 */
SELECT * FROM pgr_trspViaEdges(
  'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost,
  reverse_cost FROM edge_table',
  ARRAY[2,7,11]::INTEGER[],
  ARRAY[0.5, 0.5, 0.5]::FLOAT[],
  true,
  true,
  'SELECT to_cost, target_id::int4, FROM_edge ||
  coalesce('',''||via_path,'''') AS via_path FROM restrictions');

/* --q6 */
