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

SELECT plan(13);

DROP TABLE IF EXISTS turn_penalty_table;
CREATE TABLE turn_penalty_table(
  seq integer,
  source bigint,
  target bigint,
  cost float,
  original_source_vertex bigint,
  original_source_edge bigint,
  original_target_vertex bigint,
  original_target_edge bigint);

DROP TABLE IF EXISTS pgr_turnpenaltygraph_test_results;
CREATE TABLE pgr_turnpenaltygraph_test_results(
  start_vid bigint,
  end_vid bigint,
  agg_cost float);

CREATE or REPLACE FUNCTION turnPenaltyGraphCompareDijkstra(cant INTEGER default 17)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
inner_sql TEXT;
dijkstra_sql TEXT;
turn_penalty_graph_sql TEXT;
target_arr TEXT;
source_arr TEXT;
cur_target_arr TEXT;
orig_target_arr TEXT;
BEGIN

    inner_sql := 'SELECT id, source, target, cost FROM edge_table';
    EXECUTE 'INSERT INTO turn_penalty_table SELECT * FROM pgr_turnPenaltyGraph($$' || inner_sql || '$$)';

    FOR i IN 1.. cant LOOP

       source_arr := array_to_string(
                       array(
                         select source from turn_penalty_table where original_source_vertex = i
                       ), ',');
      
       CONTINUE WHEN source_arr = '';     

       target_arr := '';
       orig_target_arr := '';

        FOR j IN 1.. cant LOOP

            CONTINUE WHEN i = j;
       
            cur_target_arr := array_to_string(
                                array(
                                  select target from turn_penalty_table where original_target_vertex = j
                                ),',');

            CONTINUE WHEN cur_target_arr = '';

            IF orig_target_arr = '' THEN
              orig_target_arr := j;
            ELSE
              orig_target_arr := orig_target_arr || ',' || j;
            END IF;

            IF target_arr = '' THEN
              target_arr := cur_target_arr;
            ELSE
              target_arr := target_arr || ',' || cur_target_arr;
            END IF;

        END LOOP;

        CONTINUE WHEN target_arr = '' OR orig_target_arr = '';

        DELETE FROM pgr_turnpenaltygraph_test_results;

        inner_sql := 'SELECT seq as id, source, target, cost FROM turn_penalty_table';

        EXECUTE 'INSERT INTO pgr_turnpenaltygraph_test_results 
                   SELECT *
                     FROM pgr_dijkstraCost($$'|| inner_sql  || '$$, 
                                       ARRAY['|| source_arr || '], 
                                       ARRAY['|| target_arr || '])'; 

        UPDATE pgr_turnpenaltygraph_test_results 
          SET start_vid = i;

        UPDATE pgr_turnpenaltygraph_test_results
          SET end_vid = turn_penalty_table.original_target_vertex
            FROM turn_penalty_table
              WHERE pgr_turnpenaltygraph_test_results.end_vid = turn_penalty_table.target;

        turn_penalty_graph_sql := 'WITH grouped_paths AS (
                                      SELECT agg_cost, ROW_NUMBER()
                                        OVER (
                                          PARTITION BY end_vid 
                                            ORDER BY agg_cost DESC
                                        ) AS rin 
                                          FROM pgr_turnpenaltygraph_test_results
                                    ) 
                                    SELECT agg_cost 
                                      FROM grouped_paths g
                                        WHERE g.rin = 1';

        inner_sql := 'SELECT id, source, target, cost FROM edge_table';
 
        dijkstra_sql := 'SELECT agg_cost FROM pgr_dijkstraCost($$' || inner_sql || '$$, 
                                                            ARRAY['|| i || '], 
                                                            ARRAY['|| orig_target_arr || '])';

        RETURN query SELECT set_eq(turn_penalty_graph_sql, dijkstra_sql, turn_penalty_graph_sql);

    END LOOP;

    RETURN;
END
$BODY$
language plpgsql;

SELECT * from turnPenaltyGraphCompareDijkstra();

SELECT * FROM finish();

ROLLBACK;

