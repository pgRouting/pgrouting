/*
Test for old code VS new code results
Data from sample data of the documentation
*/
SELECT plan(54);

PREPARE q1 AS
SELECT ST_area(pgr_alphaShape) FROM pgr_alphaShape((SELECT ST_Collect(the_geom) FROM edge_table));

PREPARE q2 AS
SELECT ST_area(pgr_alphaShape((SELECT ST_Collect(the_geom) FROM edge_table), 1.582));

SELECT set_eq('q1', $$SELECT 11.75$$, 'Shall have the expected area');
SELECT set_eq('q1', 'q2', '1.582 shall be the best spoon raidus');

SELECT alphaShape_tester('edge_table', 'the_geom', 0, false, 11.75, 9);
SELECT alphaShape_tester('edge_table', 'the_geom', 1.582, false, 11.75, 9);

-- next area
SELECT alphaShape_tester('edge_table', 'the_geom', 1.581, false, 9.75, 12);

-- some additional values for spoon radius
SELECT alphaShape_tester('edge_table', 'the_geom', 1.5, false, 9.75, 12);
SELECT alphaShape_tester('edge_table', 'the_geom', 1.4, false, 9.75, 12);
SELECT alphaShape_tester('edge_table', 'the_geom', 1.3, false, 9.75, 12);
SELECT alphaShape_tester('edge_table', 'the_geom', 1.2, false, 9.75, 12);
SELECT alphaShape_tester('edge_table', 'the_geom', 1.1, false, 9.75, 12);
SELECT alphaShape_tester('edge_table', 'the_geom', 1.0, false, 9.75, 12);
SELECT alphaShape_tester('edge_table', 'the_geom', 0.9, false, 9.75, 12);
SELECT alphaShape_tester('edge_table', 'the_geom', 0.8, false, 8, 12);
SELECT alphaShape_tester('edge_table', 'the_geom', 0.7, false, 1.75, 10);

-- no area
SELECT alphaShape_tester('edge_table', 'the_geom', 0.4301, true, 0, 0);




SELECT finish();
