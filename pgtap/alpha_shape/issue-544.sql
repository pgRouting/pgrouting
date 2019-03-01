\i setup.sql

SELECT plan(3);

--DROP TABLE nodes;
CREATE TABLE nodes (
id integer,
x float8,
y float8);

copy nodes(id,x,y) FROM stdin WITH DELIMITER '|';
156|351885.927|739332.781
156|351893|739579
156|352004.408|739375.19
126|352004.408|739375.19
125|352473|739498
126|352473|739498
150|352543|741610
161|352893.205|740082.415
160|352893.205|740082.415
280|352893.205|740082.415
146|352984.15|739941.023
160|352984.15|739941.023
125|352984.15|739941.023
150|353370|740397
151|353370|740397
146|353370|740397
151|353400|740376
154|353400.927|741347.893
251|353486.777|740517.047
154|353486.777|740517.047
151|353486.777|740517.047
251|353752.373|740768.525
\.


PREPARE q1 AS
SELECT * FROM pgr_alphaShape((SELECT ST_Collect(ST_MakePoint(x,y)) FROM nodes where id = 161));
-- ERROR:  Distance is too short. only 1 vertex for alpha shape calculation. alpha shape calculation needs at least 3 vertices.

PREPARE q2 AS
SELECT * FROM pgr_alphaShape((SELECT ST_Collect(ST_MakePoint(x,y)) FROM nodes where id IN (160, 161)));
-- ERROR:  Error computing shape: After eliminating duplicated points, less than 3 points remain!!

PREPARE q3 AS
SELECT * FROM pgr_alphaShape((SELECT ST_Collect(ST_MakePoint(x,y)) FROM nodes));

SELECT throws_ok('q1',
 'XX000','Less than 3 vertices. pgr_alphaShape needs at least 3 vertices.',
 'SHOULD THROW, because there is only one point');

SELECT throws_ok('q2',
 'XX000','Less than 3 vertices. pgr_alphaShape needs at least 3 vertices.',
 'SHOULD THROW, because there are less than 3 distinc points');

SELECT lives_ok('q3', 'SHOULD LIVE because ater eliminating duplicates there are enough points to work with');


-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;
