BEGIN;

-- set client_min_messages to notice;
SELECT* from pgr_xydtsp($$
    SELECT * FROM pgr_dijkstraDMatrix('
        SELECT * from edge_table',array[1,2,3,4,5], directed:=false)$$,
    2,5);

CREATE TABLE vertex_table (
    id serial,
    x double precision,
    y double precision
);

INSERT INTO vertex_table VALUES
(1,2,0), (2,2,1), (3,3,1), (4,4,1), (5,0,2), (6,1,2), (7,2,2),
(8,3,2), (9,4,2), (10,2,3), (11,3,3), (12,4,3), (13,2,4);


SELECT * from pgr_xydtsp($$select * from pgr_eucledianDmatrix('vertex_table'::regclass)$$, 6);


ROLLBACK;
