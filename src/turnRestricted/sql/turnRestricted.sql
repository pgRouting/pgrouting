/*PGR-GNU*****************************************************************
File: turnRestricted.sql

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: 

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License AS published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; IF not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

********************************************************************PGR-GNU*/


/*
CREATE OR REPLACE FUNCTION pgr_turnRestricted(
    edges_sql text,
    source_vid BIGINT,
    target_vid BIGINT,
    restrictions_sql text DEFAULT NULL
    directed BOOLEAN DEFAULT TRUE)
RETURNS SETOF pgr_costResult
AS '$libdir/${PGROUTING_LIBRARY_NAME}', 'turn_restricted'
LANGUAGE 'c' IMMUTABLE;
*/

-- Equivalent to a normal dijkstra
CREATE OR REPLACE FUNCTION pgr_turnRestricted(
    edges_sql text,
    start_vid INTEGER,
    end_vid INTEGER,
    directed BOOLEAN DEFAULT TRUE)
RETURNS SETOF pgr_costResult AS
$BODY$
DECLARE
BEGIN
    RETURN query SELECT seq-1 AS seq, node::integer AS id1, edge::integer AS id2, cost
        FROM pgr_dijkstra(edges_sql, start_vid, end_vid, directed);
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;

CREATE OR REPLACE FUNCTION pgr_turnRestricted(
    edges_sql text,
    start_vid INTEGER,
    end_vid INTEGER,
    restrictions_sql text,
    directed BOOLEAN DEFAULT TRUE)
RETURNS SETOF pgr_costResult AS
$BODY$
DECLARE
has_reverse BOOLEAN;
sql TEXT;
BEGIN
    IF (length(restrictions_sql) = 0) THEN
        -- no restrirctions then its a dijkstra
        RETURN query SELECT seq-1 AS seq, node::integer AS id1, edge::integer AS id2, cost
        FROM pgr_dijkstra(edges_sql, start_vid, end_vid, directed);
    ELSE
        -- otherwise its a trsp
        has_reverse =_pgr_parameter_check('dijkstra', edges_sql, false);
        RETURN query SELECT * FROM pgr_trsp(edges_sql, start_vid, end_vid, directed, has_reverse, restrictions_sql);
    END IF;
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;

