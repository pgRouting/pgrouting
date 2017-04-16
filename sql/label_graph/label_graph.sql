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
CREATE OR REPLACE FUNCTION pgr_labelGraph(
                edge_table text,
                id text default 'id',
                source text default 'source',
                target text default 'target',
                subgraph text default 'subgraph',
                rows_where text default 'true'
        )
        RETURNS character varying AS
$BODY$

DECLARE
        naming record;
        schema_name text;
        table_name text;
        garbage text;
        incre integer;
        table_schema_name text;
        query text;
        ecnt integer;
        sql1 text;
        rec1 record;
        sql2 text;
        rec2 record;
        rec_count record;
        rec_single record;
        graph_id integer;
        gids int [];

BEGIN
        raise notice 'Processing:';
        raise notice 'pgr_brokenGraph(''%'',''%'',''%'',''%'',''%'',''%'')', edge_table,id,source,target,subgraph,rows_where;
        raise notice 'Performing initial checks, please hold on ...';

        Raise Notice 'Starting - Checking table ...';
        BEGIN
                raise debug 'Checking % table existance', edge_table;
                execute 'select * from pgr_getTableName('|| quote_literal(edge_table) ||')' into naming;
                schema_name = naming.sname;
                table_name = naming.tname;
                table_schema_name = schema_name||'.'||table_name;
                IF schema_name is null then
                        raise notice 'no schema';
                        return 'FAIL';
                else
                        if table_name is null then
                                raise notice 'no table';
                                return 'FAIL';
                        end if;
                end if;
        END;
        Raise Notice 'Ending - Checking table';

        Raise Notice 'Starting - Checking columns';
        BEGIN
                raise debug 'Checking exitance of necessary columns inside % table', edge_table;
                execute 'select * from pgr_isColumnInTable('|| quote_literal(table_schema_name) ||', '|| quote_literal(id) ||')' into naming;
                if naming.pgr_iscolumnintable = 'f' then
                        raise notice 'no id column';
                        return 'FAIL';
                end if;
                execute 'select * from pgr_isColumnInTable('|| quote_literal(table_schema_name) ||', '|| quote_literal(source) ||')' into naming;
                if naming.pgr_iscolumnintable = 'f' then
                        raise notice 'no source column';
                        return 'FAIL';
                end if;
                execute 'select * from pgr_isColumnInTable('|| quote_literal(table_schema_name) ||', '|| quote_literal(target) ||')' into naming;
                if naming.pgr_iscolumnintable = 'f' then
                        raise notice 'no target column';
                        return 'FAIL';
                end if;
                execute 'select * from pgr_isColumnInTable('|| quote_literal(table_schema_name) ||', '|| quote_literal(subgraph) ||')' into naming;
                if naming.pgr_iscolumnintable = 't' then
                        raise notice 'subgraph column already in the table';
                        return 'FAIL';
                end if;
        END;
        Raise Notice 'Ending - Checking columns';

        Raise Notice 'Starting - Checking rows_where condition';
        BEGIN
                raise debug 'Checking rows_where condition';
                query='select count(*) from '|| pgr_quote_ident(table_schema_name) ||' where '|| rows_where;
                execute query into ecnt;
                raise debug '-->Rows where condition: OK';
                raise debug '    --> OK';
                EXCEPTION WHEN OTHERS THEN
                        raise notice 'Got %', SQLERRM;
                        Raise notice 'ERROR: Condition is not correct. Please execute the following query to test your condition';
                        Raise notice '%', query;
                        return 'FAIL';
        END;
        Raise Notice 'Ending - Checking rows_where condition';

        garbage := 'garbage001';
        incre := 1;
        Raise Notice 'Starting - Checking temporary column';
        Begin
                raise debug 'Checking Checking temporary columns existance';

                While True
                        Loop
                                execute 'select * from pgr_isColumnInTable('|| quote_literal(table_schema_name) ||', '|| quote_literal(garbage) ||')' into naming;
                                If naming.pgr_iscolumnintable = 't' THEN
                                        incre := incre + 1;
                                        garbage := 'garbage00'||incre||'';
                                ELSE
                                        EXIT;
                                END IF;
                        End Loop;
        End;
        Raise Notice 'Ending - Checking temporary column';

        Raise Notice 'Starting - Calculating subgraphs';
        BEGIN
                --------- Add necessary columns ----------
                EXECUTE 'ALTER TABLE '|| pgr_quote_ident(table_schema_name) ||' ADD COLUMN ' || pgr_quote_ident(subgraph) || ' INTEGER DEFAULT -1';
                EXECUTE 'ALTER TABLE '|| pgr_quote_ident(table_schema_name) ||' ADD COLUMN ' || pgr_quote_ident(garbage) || ' INTEGER DEFAULT 0';
                graph_id := 1;

                EXECUTE 'select count(*) as count from '|| pgr_quote_ident(table_schema_name) ||' where '|| rows_where ||'' into rec_count;
                if rec_count.count = 0 then
                        RETURN 'rows_where condition generated 0 rows';
                end if;

                WHILE TRUE
                        LOOP
                                ---------- Assign the very first -1 row graph_id ----------
                                EXECUTE 'SELECT ' || pgr_quote_ident(id) || ' AS gid FROM '|| pgr_quote_ident(table_schema_name) ||' WHERE '|| rows_where ||' AND ' || pgr_quote_ident(subgraph) || ' = -1 LIMIT 1' INTO rec_single;
                                EXECUTE 'UPDATE '|| pgr_quote_ident(table_schema_name) ||' SET ' || pgr_quote_ident(subgraph) || ' = ' || graph_id || ' WHERE ' || pgr_quote_ident(id) || ' = ' || rec_single.gid || '';

                                --------- Search other rows with that particular graph_id -----------
                                WHILE TRUE
                                        LOOP
                                                EXECUTE 'SELECT COUNT(*) FROM '|| pgr_quote_ident(table_schema_name) ||' WHERE ' || pgr_quote_ident(subgraph) || ' = ' || graph_id || ' AND ' || pgr_quote_ident(garbage) || ' = 0' into rec_count;
                                                ----------- The following if else will check those rows which already have entertained ------------
                                                IF (rec_count.count > 0) THEN
                                                        sql1 := 'SELECT ' || pgr_quote_ident(id) || ' AS gid, ' || pgr_quote_ident(source) || ' AS source, ' || pgr_quote_ident(target) || ' AS target FROM '|| pgr_quote_ident(table_schema_name) ||' WHERE ' || pgr_quote_ident(subgraph) || ' = ' || graph_id || ' AND ' || pgr_quote_ident(garbage) || ' = 0';
                                                        FOR rec1 IN EXECUTE sql1
                                                                LOOP
                                                                        sql2 := 'SELECT ' || pgr_quote_ident(id) || ' AS gid, ' || pgr_quote_ident(source) || ' AS source, ' || pgr_quote_ident(target) || ' AS target FROM '|| pgr_quote_ident(table_schema_name) ||' WHERE '|| pgr_quote_ident(source) ||' = '|| rec1.source ||' OR '|| pgr_quote_ident(target) ||' = '|| rec1.source ||' OR '|| pgr_quote_ident(source) ||' = '|| rec1.target ||' OR '|| pgr_quote_ident(target) ||' = '|| rec1.target ||'';
                                                                        FOR rec2 IN EXECUTE sql2
                                                                                LOOP
                                                                                        EXECUTE 'UPDATE '|| pgr_quote_ident(table_schema_name) ||' SET ' || pgr_quote_ident(subgraph) || ' = ' || graph_id || ' WHERE ' || pgr_quote_ident(id) || ' = ' || rec2.gid || '';
                                                                                END LOOP;
                                                                        EXECUTE 'UPDATE '|| pgr_quote_ident(table_schema_name) ||' SET ' || pgr_quote_ident(garbage) || ' = 1 WHERE ' || pgr_quote_ident(id) || ' = ' || rec1.gid || '';
                                                                END LOOP;
                                                ELSE
                                                        EXIT;
                                                END IF;
                                        END LOOP;

                                ------ Following is to exit the while loop. 0 means no more -1 id.
                                EXECUTE 'SELECT COUNT(*) AS count FROM '|| pgr_quote_ident(table_schema_name) ||' WHERE '|| rows_where ||' AND ' || pgr_quote_ident(subgraph) || ' = -1' INTO rec_count;
                                If (rec_count.count = 0) THEN
                                        EXIT;
                                ELSE
                                        graph_id := graph_id + 1;
                                END IF;
                        END LOOP;

                ----------- Drop garbage column ------------
                EXECUTE 'ALTER TABLE '|| pgr_quote_ident(table_schema_name) ||' DROP COLUMN ' || pgr_quote_ident(garbage) ||'';
                Raise Notice 'Successfully complicated calculating subgraphs';
        END;
        Raise Notice 'Ending - Calculating subgraphs';

        RETURN 'OK';

END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;
