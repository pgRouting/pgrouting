BEGIN;

SELECT CASE WHEN NOT min_version('3.2.0') THEN plan(1) ELSE plan(4) END;

CREATE OR REPLACE FUNCTION issue_check()
RETURNS SETOF TEXT AS
$BODY$
BEGIN

IF NOT min_version('3.2.0') THEN
  RETURN QUERY SELECT skip(1,'Issue fixed on 3.2.0');
  RETURN;
END IF;

PREPARE q1 AS
SELECT * FROM pgr_withPoints (
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT pid, edge_id, fraction FROM pointsOfInterest WHERE pid IN (-1)',
    1, -2
);

RETURN QUERY
SELECT * FROM lives_ok('q1');
RETURN QUERY
SELECT * FROM is_empty('q1');

PREPARE q2 AS
SELECT * FROM pgr_withPoints (
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT pid, edge_id, fraction FROM pointsOfInterest WHERE pid IN (-1)',
    1, 2
);

RETURN QUERY
SELECT * FROM lives_ok('q2');
RETURN QUERY
SELECT * FROM isnt_empty('q2');

END;
$BODY$
LANGUAGE plpgsql;

SELECT issue_check();
SELECT finish();
ROLLBACK;
