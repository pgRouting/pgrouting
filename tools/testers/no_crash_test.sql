
CREATE OR REPLACE FUNCTION no_crash_test(func TEXT, params TEXT[], subs TEXT[])
RETURNS SETOF TEXT AS
$BODY$
DECLARE
mp TEXT[];
q1 TEXT;
q TEXT;
separator TEXT;
BEGIN
    FOR i IN 0..array_length(params, 1) LOOP
        IF _pgr_versionless((SELECT boost from pgr_full_version()), '1.54.0') AND func='pgr_alphashape' THEN
            RETURN QUERY SELECT * FROM  skip('pgr_alphaSahpe not supported when compiled with Boost version < 1.54.0', 2);
            CONTINUE;
        END IF;
        separator = ' ';
        mp := params;
        IF i != 0 THEN
            mp[i] = subs[i];
        END IF;

        q1 := 'SELECT * FROM ' || $1 || ' (';

        FOR i IN 1..array_length(mp, 1) LOOP
            q1 := q1 || separator || mp[i];
            separator :=',';
        END LOOP;

        q1 := q1 || ')';

        -- RAISE WARNING '%', q1;


        RETURN query SELECT * FROM lives_ok(q1);
        IF i = 0 THEN
            RETURN query SELECT * FROM isnt_empty(q1);
        ELSE
            IF func='pgr_alphashape' THEN
                RETURN query SELECT * FROM isnt_empty(q1);
            ELSE
                RETURN query SELECT * FROM is_empty(q1);
            END IF;
        END IF;

    END LOOP;

END
$BODY$
LANGUAGE plpgsql VOLATILE;
