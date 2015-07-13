CREATE OR REPLACE FUNCTION pgr_brokenGraph(
                IN schm character varying,
                IN tbl character varying,
                IN clmn character varying
        )
        RETURNS SETOF VOID AS
$BODY$
DECLARE
        sql1 text;
        rec1 record;
        sql2 text;
        rec2 record;
        rec_count record;
        rec_single record;
        graph_id integer;
        gids int [];        
BEGIN   
        EXECUTE 'SELECT COUNT(*) AS count FROM information_schema.columns WHERE table_schema = ''' || quote_ident(schm) || ''' AND  table_name= ''' || quote_ident(tbl) || ''' AND column_name= ''' || quote_ident(clmn) || '''' INTO rec_count;

        IF (rec_count.count > 0) THEN
                RAISE NOTICE 'Column % in the table % (schema %) already exists!', quote_ident(clmn), quote_ident(tbl), quote_ident(schm);
        ELSE
                --------- Add necessary columns ----------
                EXECUTE 'ALTER TABLE ' || quote_ident(schm) || '.' || quote_ident(tbl) || ' ADD COLUMN ' || quote_ident(clmn) || ' INTEGER DEFAULT -1';
                EXECUTE 'ALTER TABLE ' || quote_ident(schm) || '.' || quote_ident(tbl) || ' ADD COLUMN garbage INTEGER DEFAULT 0';
                graph_id := 1;
                
                WHILE TRUE
                        LOOP
                                ---------- Assign the very first -1 row graph_id ----------
                                EXECUTE 'SELECT * FROM ' || quote_ident(schm) || '.' || quote_ident(tbl) || ' WHERE ' || quote_ident(clmn) || ' = -1 LIMIT 1' INTO rec_single;
                                EXECUTE 'UPDATE ' || quote_ident(schm) || '.' || quote_ident(tbl) || ' SET ' || quote_ident(clmn) || ' = ' || graph_id || ' WHERE gid = ' || rec_single.gid || '';

                                --------- Search other rows with that particular graph_id -----------
                                WHILE TRUE
                                        LOOP
                                                EXECUTE 'SELECT COUNT(*) FROM ' || quote_ident(schm) || '.' || quote_ident(tbl) || ' WHERE ' || quote_ident(clmn) || ' = ' || graph_id || ' AND garbage = 0' into rec_count;
                                                ----------- The following if else will check those rows which already have entertained ------------
                                                IF (rec_count.count > 0) THEN
                                                        sql1 := 'SELECT * FROM ' || quote_ident(schm) || '.' || quote_ident(tbl) || ' WHERE ' || quote_ident(clmn) || ' = ' || graph_id || ' AND garbage = 0';
                                                        FOR rec1 IN EXECUTE sql1
                                                                LOOP
                                                                        sql2 := 'SELECT * FROM ' || quote_ident(schm) || '.' || quote_ident(tbl) || ' WHERE source = '|| rec1.source ||' OR target = '|| rec1.source ||' OR source = '|| rec1.target ||' OR target = '|| rec1.target ||'';
                                                                        FOR rec2 IN EXECUTE sql2
                                                                                LOOP
                                                                                        EXECUTE 'UPDATE ' || quote_ident(schm) || '.' || quote_ident(tbl) || ' SET ' || quote_ident(clmn) || ' = ' || graph_id || ' WHERE gid = ' || rec2.gid || '';
                                                                                END LOOP;
                                                                        EXECUTE 'UPDATE ' || quote_ident(schm) || '.' || quote_ident(tbl) || ' SET garbage = 1 WHERE gid = ' || rec1.gid || '';
                                                                END LOOP;
                                                ELSE
                                                        EXIT;
                                                END IF;
                                        END LOOP;
                                
                                ------ Following is to exit the while loop. 0 means no more -1 id.
                                EXECUTE 'SELECT COUNT(*) AS count FROM ' || quote_ident(schm) || '.' || quote_ident(tbl) || ' WHERE ' || quote_ident(clmn) || ' = -1' INTO rec_count;
                                If (rec_count.count = 0) THEN
                                        EXIT;
                                ELSE
                                        graph_id := graph_id + 1;
                                END IF;
                        END LOOP;
                        
                ----------- Drop garbage column ------------
                EXECUTE 'ALTER TABLE ' || quote_ident(schm) || '.' || quote_ident(tbl) || ' DROP COLUMN garbage';
                RAISE NOTICE 'Success';
        END IF;
        
        EXCEPTION
                WHEN internal_error THEN
                        RAISE NOTICE 'Something went wrong!';
END;
$BODY$
LANGUAGE 'plpgsql' VOLATILE STRICT;

------------ USAGE ------------
-- SELECT pgr_brokenGraph('schema_name', 'edgetable_name', 'subgraphcolumn_name')