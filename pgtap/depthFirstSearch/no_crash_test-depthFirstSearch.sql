\i setup.sql

SELECT plan(2);

PREPARE edges AS
SELECT id, source, target, cost, reverse_cost  FROM edge_table;

PREPARE null_vertex AS
SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1);


SELECT isnt_empty('edges', 'Should be not empty to tests be meaningful');
SELECT is_empty('null_vertex', 'Should be empty to tests be meaningful');


CREATE OR REPLACE FUNCTION test_function()
RETURNS SETOF TEXT AS
$BODY$
DECLARE
params TEXT[];
subs TEXT[];
BEGIN
    PERFORM todo_start('Complete the no crash test');

    PERFORM todo_end();
END
$BODY$
LANGUAGE plpgsql VOLATILE;


SELECT * FROM test_function();

ROLLBACK;
