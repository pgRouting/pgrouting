\i setup.sql


--   SAMPLE DATA FROM THE ISSUE 285

CREATE TABLE parallel (
    id serial,
    source integer,
    target integer,
    cost double precision,
    reverse_cost double precision,
    x1 double precision,
    y1 double precision,
    x2 double precision,
    y2 double precision,
    the_geom geometry
);

INSERT INTO parallel (x1,y1,x2,y2)
VALUES (1,0,1,1),(1,1,1,3),(1,1,1,3),(1,1,1,3),(1,3,1,4);

UPDATE parallel SET the_geom = ST_makeline(ST_point(x1,y1),ST_point(x2,y2));
UPDATE parallel SET the_geom = ST_makeline(ARRAY[ST_point(1,1),ST_point(0,2),ST_point(1,3)]) WHERE id = 3;
UPDATE parallel SET the_geom = ST_makeline(ARRAY[ST_point(1,1),ST_point(2,1),ST_point(2,3),ST_point(1,3)]) WHERE id = 4;
UPDATE parallel SET cost = ST_length(the_geom), reverse_cost = ST_length(the_geom);
SET client_min_messages=WARNING;
SELECT pgr_createTopology('parallel',0.001);
SET client_min_messages=NOTICE;

SELECT PLAN(7);

PREPARE q1 AS
SELECT *
FROM pgr_ksp(
    'SELECT id, source, target, cost, reverse_cost FROM parallel',
    2, 3, 1, directed:=true
);

PREPARE q2 AS
SELECT *
FROM pgr_ksp(
    'SELECT id, source, target, cost, reverse_cost FROM parallel',
    2, 3, 2, directed:=true
);

PREPARE q3 AS
SELECT *
FROM pgr_ksp(
    'SELECT id, source, target, cost, reverse_cost FROM parallel',
    2, 3, 3, directed:=true
);

PREPARE q4 AS
SELECT *
FROM pgr_ksp(
    'SELECT id, source, target, cost, reverse_cost FROM parallel',
    2, 3, 4, directed:=true
);


PREPARE q5 AS
SELECT *
FROM pgr_ksp(
    'SELECT id, source, target, cost, reverse_cost FROM parallel',
    2, 3, 100, directed:=true
);

SELECT set_eq('q1', 'q2');
SELECT set_eq('q1', 'q3');
SELECT set_eq('q1', 'q4');
SELECT set_eq('q1', 'q5');

PREPARE q6 AS
SELECT *
FROM pgr_ksp(
    'SELECT id, source, target, cost, reverse_cost FROM parallel',
    1, 4, 1, directed:=true
);

PREPARE q7 AS
SELECT *
FROM pgr_ksp(
    'SELECT id, source, target, cost, reverse_cost FROM parallel',
    1, 4, 2, directed:=true
);

PREPARE q8 AS
SELECT *
FROM pgr_ksp(
    'SELECT id, source, target, cost, reverse_cost FROM parallel',
    1, 4, 3, directed:=true
);

PREPARE q9 AS
SELECT *
FROM pgr_ksp(
    'SELECT id, source, target, cost, reverse_cost FROM parallel',
    1, 4, 100, directed:=true
);

SELECT set_eq('q6', 'q7');
SELECT set_eq('q6', 'q8');
SELECT set_eq('q6', 'q9');

SELECT finish();
ROLLBACK;
