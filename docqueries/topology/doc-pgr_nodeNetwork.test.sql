--                pgr_nodeNetwork
--------------------------------------------------------------------------------

/* --q1 */
SELECT pgr_createTopology('edge_table', 0.001, clean := TRUE);
/* --q1.1 */

/* --q2 */
SELECT pgr_analyzegraph('edge_table', 0.001);
/* --q2.1 */

/* --q3 */
SELECT pgr_nodeNetwork('edge_table', 0.001);
/* --q3.1 */

/* --q4 */
SELECT old_id, sub_id FROM edge_table_noded ORDER BY old_id, sub_id;
/* --q4.1 */

/* --q5 */
SELECT pgr_createTopology('edge_table_noded', 0.001);
/* --q5.1 */

/* --q6 */
SELECT pgr_analyzegraph('edge_table_noded', 0.001);
/* --q6.1 */

/* --q7 */
alter table edge_table drop column if exists old_id;
alter table edge_table add column old_id integer;
insert into edge_table (old_id, dir, cost, reverse_cost, the_geom)
   (with
       segmented as (select old_id,count(*) as i from edge_table_noded group by old_id)
   select  segments.old_id, dir, cost, reverse_cost, segments.the_geom
       from edge_table as edges join edge_table_noded as segments on (edges.id = segments.old_id)
       where edges.id in (select old_id from segmented where i>1) );
/* --q7.1 */

/* --q8 */
SELECT pgr_createTopology('edge_table', 0.001);
/* --q8.1 */

/* --q9 */
SELECT pgr_analyzegraph('edge_table', 0.001, rows_where:='id not in (select old_id from edge_table where old_id is not null)');
/* --q9.1 */

/* --q10 */
SELECT pgr_analyzegraph('edge_table', 0.001, rows_where:='old_id is null');
/* --q10.1 */

/* --q11 */
SELECT pgr_analyzegraph('edge_table', 0.001);
/* --q11.1 */
