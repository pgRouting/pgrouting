-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/
/* -- q1 */
SELECT * FROM pgr_johnson(
  'SELECT source, target, cost FROM edges
  WHERE id < 5'
) ORDER BY start_vid, end_vid;
/* -- q2 */
