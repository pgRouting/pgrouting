/* --data-start */

DROP TABLE IF EXISTS roadworks CASCADE;
CREATE table roadworks (
    id BIGINT not null primary key,
    source BIGINT,
    target BIGINT,
    road_work FLOAT,
    reverse_road_work FLOAT

);


INSERT INTO roadworks(
  id, source, target, road_work, reverse_road_work) VALUES
  (1,  1,  2,  0,  0),
  (2,  2,  3,  -1,  1),
  (3,  3,  4,  -1,  0),
  (4,  2,  5,  0,  0),
  (5,  3,  6,  1,  -1),
  (6,  7,  8,  1,  1),
  (7,  8,  5,  0,  0),
  (8,  5,  6,  1,  1),
  (9,  6,  9,  1,  1),
  (10,  5,  10,  1,  1),
  (11,  6,  11,  1,  -1),
  (12,  10,  11,  0,  -1),
  (13,  11,  12,  1,  -1),
  (14,  10,  13,  1,  1),
  (15,  9,  12,  0,  0),
  (16,  4,  9,  0,  0),
  (17,  14,  15,  0,  0),
  (18,  16,  17,  0,  0);

/* --data-end */

/*  -- q1 */
SELECT * FROM pgr_binaryBreadthFirstSearch(
    'select id, source, target, cost, reverse_cost 
    from edge_table',
    2, 3
);
/* -- q3 */
SELECT * FROM pgr_binaryBreadthFirstSearch(
    'select id, source, target, cost 
    from edge_table',
    2, ARRAY[3,5],
    false
);
/* -- q4 */
SELECT * FROM pgr_binaryBreadthFirstSearch(
    'select id, source, target, cost, reverse_cost 
    from edge_table',
    ARRAY[2,11], 5
);
/* -- q5 */
SELECT * FROM pgr_binaryBreadthFirstSearch(
    'select id, source, target, cost, reverse_cost 
    from edge_table',
    ARRAY[2,11], ARRAY[3,5],
    false
);
/* -- q51 */
SELECT * FROM combinations_table;
/* -- q6 */
SELECT * FROM pgr_binaryBreadthFirstSearch(
    'select id, source, target, cost, reverse_cost 
    from edge_table',
    'select * from combinations_table',
    false
);
/* -- q7 */
SELECT * FROM pgr_binaryBreadthFirstSearch(
    'select id, source, target, road_work as cost, reverse_road_work as reverse_cost 
    from roadworks',
    2, 3
);
SELECT * FROM pgr_binaryBreadthFirstSearch(
    'select id, source, target, road_work as cost 
    from roadworks',
    2, ARRAY[3,5],
    false
);
SELECT * FROM pgr_binaryBreadthFirstSearch(
    'select id, source, target, road_work as cost, reverse_road_work as reverse_cost 
    from roadworks',
    ARRAY[2,11], 5
);
SELECT * FROM pgr_binaryBreadthFirstSearch(
    'select id, source, target, road_work as cost, reverse_road_work as reverse_cost 
    from roadworks',
    ARRAY[2,11], ARRAY[3,5],
    false
);
/* -- q8 */