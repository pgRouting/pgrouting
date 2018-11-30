\i setup.sql
\i tmp_net.sql

SELECT plan(71);


CREATE or REPLACE FUNCTION issue1154()
RETURNS SETOF TEXT AS
$BODY$
DECLARE
    dd_query TEXT;
    dijkstra_sql TEXT;
    astar_sql TEXT;
    vids TEXT;
    data INTEGER[];
    cant INTEGER;
BEGIN
    data := ARRAY[
    28143,37447,43401,50409,91800,
    98234,108908,1888,1897,85914,
    1898,112714,11851,31406,
    37961,43534,50994,57875,71749,
    77860,4424,4641,35309,53002,
    57762,75549,79856,80019,88626,
    89435,116859,37736,51812,97808]::INTEGER[];

    RETURN QUERY
    SELECT is_empty($$
        SELECT * FROM tmp_net WHERE target = 85930
        UNION
        SELECT * FROM tmp_net WHERE target = 85930
        $$);

    cant := array_length(data, 1);

    FOR i IN 1.. cant LOOP

        dd_query := format($$
            SELECT
            route.node AS node_id,
            route.agg_cost
            FROM pgr_drivingdistance(
                'SELECT * FROM tmp_net',
                ARRAY[%1$s, %2$s]::INTEGER[],
                2680, equicost:=TRUE, directed:=TRUE) route;
            $$, 85930, data[i]);


        RETURN QUERY SELECT lives_ok(dd_query, 'i=' || data[i]);

        dd_query := format($$
            SELECT
            route.node AS node_id,
            route.agg_cost
            FROM pgr_drivingdistance(
                'SELECT * FROM tmp_net',
                ARRAY[%1$s, %2$s]::INTEGER[],
                2680, equicost:=TRUE, directed:=TRUE) route;
            $$, data[i], 85930);


        RETURN QUERY SELECT lives_ok(dd_query, 'i=' || data[i]);

    END LOOP;

RETURN;
END
$BODY$
language plpgsql;

SELECT * FROM issue1154();

SELECT lives_ok($$
    SELECT *
    FROM pgr_drivingdistance(
        'SELECT * FROM tmp_net',
        ARRAY[
            28143,37447,43401,50409,91800,
            98234,108908,1888,1897,85914,
            1898,85930,112714,11851,31406,
            37961,43534,50994,57875,71749,
            77860,4424,4641,35309,53002,
            57762,75549,79856,80019,88626,
            89435,116859,37736,51812,97808]::INTEGER[],
        2680,
        equicost:=TRUE,
        directed:=TRUE
    )
    $$);

SELECT bag_has($$
    SELECT *
    FROM pgr_drivingdistance(
        'SELECT * FROM tmp_net',
        ARRAY[
            28143,37447,43401,50409,91800,
            98234,108908,1888,1897,85914,
            1898,85930,112714,11851,31406,
            37961,43534,50994,57875,71749,
            77860,4424,4641,35309,53002,
            57762,75549,79856,80019,88626,
            89435,116859,37736,51812,97808]::INTEGER[],
        2680,
        equicost:=TRUE,
        directed:=TRUE
    )$$,
    $$ VALUES (12, 85930, 85930, -1, 0, 0) $$
);

SELECT finish();
ROLLBACK;
