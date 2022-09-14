/* -- q1 */
SELECT * FROM pgr_cuthillMckeeOrdering(
  'SELECT id, source, target, cost, reverse_cost FROM edges'
  );
/* -- q2 */



-- added to pgtap test


CREATE TABLE expected_result (
  seq BIGINT,
  node BIGINT);

INSERT INTO expected_result (seq, node) VALUES
(1, 13),
(2, 14),
(3, 2),
(4, 4),
(5, 9),
(6, 1),
(7, 8),
(8, 3),
(9, 12),
(10, 7),
(11, 5),
(12, 17),
(13, 11),
(14, 6),
(15, 16),
(16, 10),
(17, 15);