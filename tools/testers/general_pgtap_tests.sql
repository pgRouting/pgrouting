
-- checks the minimum version version for C++ code
CREATE OR REPLACE FUNCTION min_lib_version(min_version TEXT)
RETURNS BOOLEAN AS
$BODY$
DECLARE val BOOLEAN;
BEGIN
WITH
  current_version AS (SELECT string_to_array(regexp_replace((SELECT library FROM pgr_full_version()), '.*-', '', 'g'),'.')::int[] AS version),
  asked_version AS (SELECT string_to_array(min_version, '.')::int[] AS version)
  SELECT (current_version.version >= asked_version.version) FROM current_version, asked_version INTO val;
  RETURN val;
END;
$BODY$
LANGUAGE plpgsql;

-- checks the minimum version version for SQL code
CREATE OR REPLACE FUNCTION min_version(min_version TEXT)
RETURNS BOOLEAN AS
$BODY$
DECLARE val BOOLEAN;
BEGIN
WITH
  current_version AS (SELECT string_to_array(regexp_replace(pgr_version(), '-.*', '', 'g'),'.')::int[] AS version),
  asked_version AS (SELECT string_to_array(min_version, '.')::int[] AS version)
  SELECT (current_version.version >= asked_version.version) FROM current_version, asked_version INTO val;
  RETURN val;
END;
$BODY$
LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION column_missing(TEXT, TEXT)
RETURNS TEXT AS
$BODY$
  SELECT CASE WHEN min_version('3.8.0') THEN
    collect_tap(throws_ok($1, '42703','column "' || $2 || '" does not exist'))
  ELSE
    collect_tap(throws_ok($1, 'P0001', 'Missing column'))
  END;
$BODY$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION wrong_relation(TEXT, TEXT)
RETURNS TEXT AS
$BODY$
  SELECT CASE WHEN min_version('3.8.0') THEN
    collect_tap(throws_ok($1, '42P01', 'relation "' || $2 || '" does not exist'))
  ELSE
    collect_tap(throws_ok($1, 'P0001', 'relation "' || $2 || '" does not exist'))
  END;
$BODY$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION function_types(TEXT, TEXT)
RETURNS TEXT AS
$BODY$
    SELECT set_eq(format($$
      WITH
      a AS (
        SELECT oid, u.name, u.idx
        FROM pg_proc p CROSS JOIN unnest(p.proallargtypes)
        WITH ordinality as u(name, idx)
        WHERE p.proname = '%1$s'),
      b AS (
        SELECT a.*, t.typname FROM a JOIN pg_type As t on (t.oid = a.name))
      SELECT array_agg(typname ORDER BY idx)  FROM b GROUP BY oid
      $$, $1), $2, $1 || ': Function types');
$BODY$ LANGUAGE SQL;
