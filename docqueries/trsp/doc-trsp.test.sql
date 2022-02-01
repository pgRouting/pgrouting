/* -- q2 */
SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
  $$SELECT path, cost FROM restrictions$$,
  2, 3,
  false
);
/* -- q3 */
SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost FROM edge_table$$,
  $$SELECT * FROM restrictions$$,
  2, ARRAY[3,7],
  false
);
/* -- q4 */
SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
  $$SELECT path, cost FROM restrictions$$,
  ARRAY[2,7], 10
);
/* -- q5 */
SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
  $$SELECT path, cost FROM restrictions$$,
  ARRAY[2,7], ARRAY[3,10],
  false
);
/* -- q6 */
SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
  $$SELECT path, cost FROM restrictions$$,
  $$SELECT * FROM (VALUES (2, 3), (2, 7), (2, 10), (7, 10)) AS combinations (source, target)$$
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
        FROM old_restrictions'
    );
    SELECT * FROM pgr_trsp(
        'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table',
        7, 11, false, false,
        'SELECT to_cost, target_id::int4,
        from_edge || coalesce('','' || via_path, '''') AS via_path
        FROM old_restrictions'
    );


/* --q4 */
    SELECT * FROM pgr_trspViaVertices(
        'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table',
        ARRAY[2,7,11]::INTEGER[],
        false,  false,
        'SELECT to_cost, target_id::int4, from_edge ||
        coalesce('',''||via_path,'''') AS via_path FROM old_restrictions');


/* --q5 */
    SELECT * FROM pgr_trspViaEdges(
        'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost,
        reverse_cost FROM edge_table',
        ARRAY[2,7,11]::INTEGER[],
        ARRAY[0.5, 0.5, 0.5]::FLOAT[],
        true,
        true,
        'SELECT to_cost, target_id::int4, FROM_edge ||
        coalesce('',''||via_path,'''') AS via_path FROM old_restrictions');

/* --q6 */
