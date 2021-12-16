SET extra_float_digits=-3;

CREATE TABLE restrictions2 (
    id SERIAL PRIMARY KEY,
    path BIGINT[],
    cost float
);

INSERT INTO restrictions2 (path, cost) VALUES
(ARRAY[4, 7], 100),
(ARRAY[5, 3], 4),
(ARRAY[16, 9], 100);

UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

SELECT * FROM pgr_trsp_withPoints(
  $$SELECT * FROM edge_table$$,
  $$SELECT * FROM restrictions2$$,
  $$(SELECT 1 AS pid, 1 AS edge_id, 0.5::float  AS fraction)
  UNION
  (SELECT 2, 6, 0.5)$$,
  -1, -2, true);
