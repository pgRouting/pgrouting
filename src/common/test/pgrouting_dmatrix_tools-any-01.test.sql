BEGIN;
\echo '---- pgr_pointstodmatrix ----'
select * from pgr_pointstodmatrix(pgr_texttopoints('2,0;2,1;3,1;2,2;4,1;4,2;2,3;3,2', 0));

\echo '---- pgr_vidstodmatrix ----'
select * from pgr_tsp(
    (select dmatrix::float8[]
       from pgr_vidstodmatrix(
                pgr_pointstovids(
                    pgr_texttopoints('2,0;2,1;3,1;2,2;4,1;4,2;2,3;3,2', 0),
                    'edge_table'), 
                pgr_texttopoints('2,0;2,1;3,1;2,2;4,1;4,2;2,3;3,2', 0),
                'edge_table')
    ),
    1
);
ROLLBACK;
