/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

********************************************************************PGR-GNU*/

\i setup.sql

SELECT plan(87);
SET client_min_messages TO ERROR;


SELECT has_function('pgr_astar', ARRAY['text', 'integer', 'integer', 'boolean', 'boolean']);
SELECT function_returns('pgr_astar', ARRAY['text', 'integer', 'integer', 'boolean', 'boolean'],'setof pgr_costresult');

CREATE OR REPLACE FUNCTION test_Integer(fn TEXT, params TEXT[], parameter TEXT)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
start_sql TEXT;
end_sql TEXT;
query TEXT;
p TEXT;
BEGIN
    start_sql = 'select * from ' || fn || '($$ SELECT ';
    FOREACH  p IN ARRAY params LOOP
        IF p = parameter THEN CONTINUE;
        END IF;
        start_sql = start_sql || p || ', ';
    END LOOP;
    end_sql = ' FROM edge_table $$, 2, 3, true, false)';

    query := start_sql || '('|| parameter || ')::SMALLINT ' || end_sql;
    RETURN query SELECT throws_ok(query,
        'XX000',
        'Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
        'SHOULD throw BECAUSE ' || parameter || ' is SMALLINT'
    );

    query := start_sql || parameter || '::INTEGER ' || end_sql;
    RETURN query SELECT lives_ok(query);

    query := start_sql || parameter || '::BIGINT ' || end_sql;
    RETURN query SELECT throws_ok(query,
        'XX000',
        'Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
        'SHOULD throw BECAUSE ' || parameter || ' is BIGINT'
    );

    query := start_sql || parameter || '::REAL ' || end_sql;
    RETURN query SELECT throws_ok(query,
        'XX000',
        'Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
        'SHOULD throw BECAUSE ' || parameter || ' is REAL'
    );

    query := start_sql || parameter || '::FLOAT8 ' || end_sql;
    RETURN query SELECT throws_ok(query,
        'XX000',
        'Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
        'SHOULD throw BECAUSE ' || parameter || ' is FLOAT8'
    );
END;
$BODY$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION test_float8(fn TEXT, params TEXT[], parameter TEXT)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
start_sql TEXT;
end_sql TEXT;
query TEXT;
p TEXT;
BEGIN
    start_sql = 'select * from ' || fn || '($$ SELECT ';
    FOREACH  p IN ARRAY params LOOP
        IF p = parameter THEN CONTINUE;
        END IF;
        start_sql = start_sql || p || ', ';
    END LOOP;
    end_sql = ' FROM edge_table $$, 2, 3, true, false)';

    query := start_sql || parameter || '::SMALLINT ' || end_sql;
    RETURN query SELECT throws_like(query,
        '%must be of type float8',
        'SHOULD throw BECAUSE ' || parameter || ' is SMALLINT'
    );

    query := start_sql || parameter || '::INTEGER ' || end_sql;
    RETURN query SELECT throws_like(query,
        '%must be of type float8',
        'SHOULD throw BECAUSE ' || parameter || ' is INTEGER'
    );

    query := start_sql || parameter || '::BIGINT ' || end_sql;
    RETURN query SELECT throws_like(query,
        '%must be of type float8',
        'SHOULD throw BECAUSE ' || parameter || ' is BIGINT'
    );

    query := start_sql || parameter || '::REAL ' || end_sql;
    RETURN query SELECT throws_like(query,
        '%must be of type float8',
        'SHOULD throw BECAUSE ' || parameter || ' is REAL'
    );

    query := start_sql || parameter || '::FLOAT8 ' || end_sql;
    RETURN query SELECT lives_ok(query);
END;
$BODY$ LANGUAGE plpgsql;

-- with reverse cost
SELECT test_integer('pgr_astar',
    ARRAY['id::INTEGER', 'source::INTEGER', 'target::INTEGER',
    'cost::FLOAT8', 'reverse_cost::FLOAT8',
    'x1::FLOAT8', 'y1::FLOAT8', 'x2::FLOAT8', 'y2::FLOAT8'],
    'id::INTEGER');
SELECT test_integer('pgr_astar',
    ARRAY['id::INTEGER', 'source::INTEGER', 'target::INTEGER',
    'cost::FLOAT8', 'reverse_cost::FLOAT8',
    'x1::FLOAT8', 'y1::FLOAT8', 'x2::FLOAT8', 'y2::FLOAT8'],
    'source::INTEGER');
SELECT test_integer('pgr_astar',
    ARRAY['id::INTEGER', 'source::INTEGER', 'target::INTEGER',
    'cost::FLOAT8', 'reverse_cost::FLOAT8',
    'x1::FLOAT8', 'y1::FLOAT8', 'x2::FLOAT8', 'y2::FLOAT8'],
    'source::INTEGER');
SELECT test_float8('pgr_astar',
    ARRAY['id::INTEGER', 'source::INTEGER', 'target::INTEGER',
    'cost::FLOAT8', 'reverse_cost::FLOAT8',
    'x1::FLOAT8', 'y1::FLOAT8', 'x2::FLOAT8', 'y2::FLOAT8'],
    'cost::FLOAT8');
SELECT test_float8('pgr_astar',
    ARRAY['id::INTEGER', 'source::INTEGER', 'target::INTEGER',
    'cost::FLOAT8', 'reverse_cost::FLOAT8',
    'x1::FLOAT8', 'y1::FLOAT8', 'x2::FLOAT8', 'y2::FLOAT8'],
    'reverse_cost::FLOAT8');
SELECT test_float8('pgr_astar',
    ARRAY['id::INTEGER', 'source::INTEGER', 'target::INTEGER',
    'cost::FLOAT8', 'reverse_cost::FLOAT8',
    'x1::FLOAT8', 'y1::FLOAT8', 'x2::FLOAT8', 'y2::FLOAT8'],
    'x1::FLOAT8');
SELECT test_float8('pgr_astar',
    ARRAY['id::INTEGER', 'source::INTEGER', 'target::INTEGER',
    'cost::FLOAT8', 'reverse_cost::FLOAT8',
    'x1::FLOAT8', 'y1::FLOAT8', 'x2::FLOAT8', 'y2::FLOAT8'],
    'y1::FLOAT8');
SELECT test_float8('pgr_astar',
    ARRAY['id::INTEGER', 'source::INTEGER', 'target::INTEGER',
    'cost::FLOAT8', 'reverse_cost::FLOAT8',
    'x1::FLOAT8', 'y1::FLOAT8', 'x2::FLOAT8', 'y2::FLOAT8'],
    'x2::FLOAT8');
SELECT test_float8('pgr_astar',
    ARRAY['id::INTEGER', 'source::INTEGER', 'target::INTEGER',
    'cost::FLOAT8', 'reverse_cost::FLOAT8',
    'x1::FLOAT8', 'y1::FLOAT8', 'x2::FLOAT8', 'y2::FLOAT8'],
    'y2::FLOAT8');




-- without reverse cost
SELECT test_integer('pgr_astar',
    ARRAY['id::INTEGER', 'source::INTEGER', 'target::INTEGER',
    'cost::FLOAT8',
    'x1::FLOAT8', 'y1::FLOAT8', 'x2::FLOAT8', 'y2::FLOAT8'],
    'id::INTEGER');
SELECT test_integer('pgr_astar',
    ARRAY['id::INTEGER', 'source::INTEGER', 'target::INTEGER',
    'cost::FLOAT8',
    'x1::FLOAT8', 'y1::FLOAT8', 'x2::FLOAT8', 'y2::FLOAT8'],
    'source::INTEGER');
SELECT test_integer('pgr_astar',
    ARRAY['id::INTEGER', 'source::INTEGER', 'target::INTEGER',
    'cost::FLOAT8',
    'x1::FLOAT8', 'y1::FLOAT8', 'x2::FLOAT8', 'y2::FLOAT8'],
    'source::INTEGER');
SELECT test_float8('pgr_astar',
    ARRAY['id::INTEGER', 'source::INTEGER', 'target::INTEGER',
    'cost::FLOAT8',
    'x1::FLOAT8', 'y1::FLOAT8', 'x2::FLOAT8', 'y2::FLOAT8'],
    'cost::FLOAT8');
SELECT test_float8('pgr_astar',
    ARRAY['id::INTEGER', 'source::INTEGER', 'target::INTEGER',
    'cost::FLOAT8',
    'x1::FLOAT8', 'y1::FLOAT8', 'x2::FLOAT8', 'y2::FLOAT8'],
    'x1::FLOAT8');
SELECT test_float8('pgr_astar',
    ARRAY['id::INTEGER', 'source::INTEGER', 'target::INTEGER',
    'cost::FLOAT8',
    'x1::FLOAT8', 'y1::FLOAT8', 'x2::FLOAT8', 'y2::FLOAT8'],
    'y1::FLOAT8');
SELECT test_float8('pgr_astar',
    ARRAY['id::INTEGER', 'source::INTEGER', 'target::INTEGER',
    'cost::FLOAT8',
    'x1::FLOAT8', 'y1::FLOAT8', 'x2::FLOAT8', 'y2::FLOAT8'],
    'x2::FLOAT8');
SELECT test_float8('pgr_astar',
    ARRAY['id::INTEGER', 'source::INTEGER', 'target::INTEGER',
    'cost::FLOAT8',
    'x1::FLOAT8', 'y1::FLOAT8', 'x2::FLOAT8', 'y2::FLOAT8'],
    'y2::FLOAT8');


SELECT finish();
ROLLBACK;
