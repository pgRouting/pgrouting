-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/
--                pgr_nodeNetwork
--------------------------------------------------------------------------------

/* --q1 */
SELECT pgr_createTopology('edges', 0.001, 'geom', clean := TRUE);
/* --q1.1 */

/* --q2 */
SELECT pgr_analyzegraph('edges', 0.001, 'geom');
/* --q2.1 */

/* --q3 */
SELECT pgr_nodeNetwork('edges', 0.001, the_geom => 'geom');
/* --q3.1 */

/* --q4 */
SELECT old_id, sub_id FROM edges_noded ORDER BY old_id, sub_id;
/* --q4.1 */

/* --q5 */
SELECT pgr_createTopology('edges_noded', 0.001, 'geom');
/* --q5.1 */

/* --q6 */
SELECT pgr_analyzegraph('edges_noded', 0.001, 'geom');
/* --q6.1 */

/* --q7 */
alter table edges drop column if exists old_id;
alter table edges add column old_id integer;
insert into edges (old_id, cost, reverse_cost, geom)
   (with
       segmented as (select old_id,count(*) as i from edges_noded group by old_id)
   select  segments.old_id, cost, reverse_cost, segments.geom
       from edges as edges join edges_noded as segments on (edges.id = segments.old_id)
       where edges.id in (select old_id from segmented where i>1) );
/* --q7.1 */

/* --q8 */
SELECT pgr_createTopology('edges', 0.001, 'geom');
/* --q8.1 */

/* --q9 */
SELECT pgr_analyzegraph('edges', 0.001, 'geom', rows_where:='id not in (select old_id from edges where old_id is not null)');
/* --q9.1 */

/* --q10 */
SELECT pgr_analyzegraph('edges', 0.001, 'geom', rows_where:='old_id is null');
/* --q10.1 */

/* --q11 */
SELECT pgr_analyzegraph('edges', 0.001, 'geom');
/* --q11.1 */
