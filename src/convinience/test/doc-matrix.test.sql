BEGIN;

\echo --q1
SELECT * FROM pgr_pointstodmatrix(pgr_texttopoints('2,0;2,1;3,1;2,2;4,1;4,2;2,3;3,2', 0));

\echo --q2
SELECT * FROM pgr_pointstovids(pgr_texttopoints('2,0;2,1;3,1;2,2;4,1;4,2;2,3;3,2', 0), 'edge_table');
SELECT dmatrix::float8[] FROM pgr_vidstodmatrix(
    pgr_pointstovids(pgr_texttopoints('2,0;2,1;3,1;2,2;4,1;4,2;2,3;3,2', 0), 'edge_table'), 
    pgr_texttopoints('2,0;2,1;3,1;2,2;4,1;4,2;2,3;3,2', 0),
    'edge_table');
SELECT * FROM pgr_tsp(
    (SELECT dmatrix::float8[]
        FROM pgr_vidstodmatrix(
            pgr_pointstovids(
                pgr_texttopoints('2,0;2,1;3,1;2,2;4,1;4,2;2,3;3,2', 0),
                'edge_table'), 
            pgr_texttopoints('2,0;2,1;3,1;2,2;4,1;4,2;2,3;3,2', 0),
            'edge_table')
    ),
    1
);
\echo --q3
ROLLBACK;
