
\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT CASE WHEN  min_lib_version('3.2.1') THEN plan(20) ELSE plan(1) END;

CREATE FUNCTION issue()
RETURNS SETOF TEXT AS
$BODY$
BEGIN

  IF NOT min_lib_version('3.2.1') THEN
    RETURN QUERY
    SELECT skip(1, 'pgr_TSP server crash fixed on 3.2.1');
    RETURN;
  END IF;

  CREATE TEMP TABLE stop_points(
    id bigint,
    x double precision,
    y double precision
  );

  INSERT INTO stop_points (id, x, y) VALUES
  (1044, -71.141013, 42.431611),
  (1111, -71.140838, 42.431231),
  (1128, -71.134264, 42.431178),
  (1248, -71.135613, 42.430675),
  (1483, -71.137247, 42.431289);


  IF min_version('4.0.0') THEN
    -- todo remove annaeling parameters
    PREPARE euclidean_query AS
    SELECT * FROM pgr_TSPeuclidean('SELECT id, x,  y FROM stop_points');
  ELSE
    -- note randomize is ignored
    PREPARE euclidean_query AS
    SELECT * FROM pgr_TSPeuclidean('SELECT id, x,  y FROM stop_points',
      randomize => false);
  END IF;

  FOR i IN 1..10 LOOP
    RETURN QUERY
    SELECT lives_ok('euclidean_query', 'euclidean run #' || i::TEXT);
  END LOOP;

  CREATE TEMP TABLE stop_points_cost_matrix AS
  SELECT DISTINCT ON(a.id, b.id) row_number() OVER() AS id,
  a.id AS start_vid, b.id AS end_vid,
  ST_Distance(ST_Point(a.x, a.y), ST_Point(b.x, b.y)) AS agg_cost
  FROM stop_points AS a CROSS JOIN stop_points AS b WHERE a.id <> b.id;


  IF min_version('4.0.0') THEN
    PREPARE matrix_query AS
    SELECT * FROM pgr_TSP($$SELECT * FROM stop_points_cost_matrix$$);
  ELSE
    -- note randomize is ignored
    PREPARE matrix_query AS
    SELECT * FROM pgr_TSP($$SELECT * FROM stop_points_cost_matrix$$,
      randomize := false);
  END IF;

  FOR i IN 1..10 LOOP
    RETURN QUERY
    SELECT lives_ok('matrix_query', 'normal run #' || i::TEXT);
  END LOOP;

END;
$BODY$
language plpgsql;

SELECT issue();
SELECT finish();
ROLLBACK;


