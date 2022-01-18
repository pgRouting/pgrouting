/* -- q1 */
SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
  $$SELECT id, path, cost FROM new_restrictions$$,
  2, 3
);
/* -- q2 */
SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
  $$SELECT id, path, cost FROM new_restrictions$$,
  2, 3,
  false
);
/* -- q3 */
SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost FROM edge_table$$,
  $$SELECT id, path, cost FROM new_restrictions$$,
  2, ARRAY[3,5],
  false
);
/* -- q4 */
SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
  $$SELECT id, path, cost FROM new_restrictions$$,
  ARRAY[2,11], 5
);
/* -- q5 */
SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
  $$SELECT id, path, cost FROM new_restrictions$$,
  ARRAY[2,11], ARRAY[3,5],
  false
);
/* -- q6 */
SELECT * FROM pgr_dijkstra(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
  $$SELECT * FROM (VALUES (2, 3), (2, 5), (11, 3), (11, 5)) AS combinations (source, target)$$
);
/* -- q7 */

/* --q1 */
    SELECT * FROM pgr_trsp(
        'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table',
        7, 12, false, false
    );

/* --q2 */

/* --q3 */
    SELECT * FROM pgr_trsp(
        'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table',
        2, 7, false, false,
        'SELECT to_cost, target_id::int4,
        from_edge || coalesce('','' || via_path, '''') AS via_path
        FROM restrictions'
    );
    SELECT * FROM pgr_trsp(
        'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table',
        7, 11, false, false,
        'SELECT to_cost, target_id::int4,
        from_edge || coalesce('','' || via_path, '''') AS via_path
        FROM restrictions'
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
