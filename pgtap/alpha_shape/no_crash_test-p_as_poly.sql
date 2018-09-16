\i setup.sql

SELECT plan(5);

PREPARE alpha_sql AS
SELECT id::INTEGER, ST_X(the_geom) AS x, ST_Y(the_geom) AS y FROM edge_table_vertices_pgr;

SELECT isnt_empty('alpha_sql', 'Should not be empty true to tests be meaningful');




CREATE OR REPLACE FUNCTION test(params TEXT[], subs TEXT[])
RETURNS SETOF TEXT AS
$BODY$
DECLARE
mp TEXT[];
q1 TEXT;
q TEXT;
BEGIN
    FOR i IN 0..array_length(params, 1) LOOP
        mp := params;
        IF i != 0 THEN
            mp[i] = subs[i];
        END IF;

        q1 := format($$
            SELECT * FROM pgr_pointsAsPolygon(
                %1$L
            )
            $$,
            mp[1]
        );

        RETURN query SELECT * FROM lives_ok(q1);
        IF i = 0 THEN
            RETURN query SELECT * FROM isnt_empty(q1);
        ELSE
            RETURN query SELECT * FROM set_eq(q1, 'SELECT NULL::geometry');
        END IF;
    END LOOP;

END
$BODY$
LANGUAGE plpgsql VOLATILE;

CREATE OR REPLACE FUNCTION test_function()
RETURNS SETOF TEXT AS
$BODY$
DECLARE
params TEXT[];
subs TEXT[];
BEGIN
    params = ARRAY['SELECT id::INTEGER, ST_X(the_geom) AS x, ST_Y(the_geom) AS y FROM edge_table_vertices_pgr']::TEXT[];
    subs = ARRAY[
    NULL
    ]::TEXT[];

    RETURN query SELECT * FROM test(params, subs);
END
$BODY$
LANGUAGE plpgsql VOLATILE;


SELECT * FROM test_function();

ROLLBACK;
