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
along with this program; IF not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

********************************************************************PGR-GNU*/

/*
CREATE OR REPLACE FUNCTION pgr_kdijkstrapath(
    sql text,
    source_vid integer,
    target_vid integer array,
    directed boolean,
    has_reverse_cost boolean)
    RETURNS SETOF pgr_costResult3
    AS '$libdir/${PGROUTING_LIBRARY_NAME}', 'onetomany_dijkstra_path'
    LANGUAGE C STABLE STRICT;

*/
CREATE OR REPLACE FUNCTION pgr_kdijkstraPath(
    sql text,
    source INTEGER,
    targets INTEGER ARRAY,
    directed BOOLEAN,
    has_rcost BOOLEAN)
    RETURNS SETOF pgr_costResult3 AS
    $BODY$
    DECLARE
    has_reverse BOOLEAN;
    new_sql TEXT;
    result pgr_costResult3;
    tmp pgr_costResult3;
    sseq INTEGER;
    i INTEGER;
    BEGIN
        RAISE NOTICE 'Deprecated function: Use pgr_dijkstra instead.';
        has_reverse =_pgr_parameter_check('dijkstra', sql, false);
        new_sql = sql;
        IF (array_ndims(targets) != 1) THEN
            raise EXCEPTION 'Error, reverse_cost is used, but query did''t return ''reverse_cost'' column'
            USING ERRCODE = 'XX000';
        END IF;

        IF (has_reverse != has_rcost) THEN
            IF (has_reverse) THEN
                new_sql = 'SELECT id, source, target, cost FROM (' || sql || ') a';
            ELSE
                raise EXCEPTION 'Error, reverse_cost is used, but query did''t return ''reverse_cost'' column'
                USING ERRCODE = 'XX000';
            END IF;
        END IF;
        SELECT ARRAY(SELECT DISTINCT UNNEST(targets) ORDER BY 1) INTO targets;

        sseq = 0; i = 1;
        FOR result IN 
            SELECT seq, a.end_vid::INTEGER AS id1, a.node::INTEGER AS i2, a.edge::INTEGER AS id3, cost
            FROM pgr_dijkstra(new_sql, source, targets, directed) a ORDER BY a.end_vid, seq LOOP
            --raise notice 'id1:%     id2:% id3:% cost:%',  result.id1, result.id2, result.id3, result.cost;
            WHILE (result.id1 != targets[i]) LOOP
                --raise notice 'we didnt find the target so need to put the record for no path found';
                --raise notice '%: targets:% result: id1%', i, targets[i], result.id1;
                tmp.seq = sseq;
                tmp.id1 = targets[i];
                IF (targets[i] = source) THEN
                    tmp.id2 = source;
                    tmp.cost = -1;
                ELSE
                    tmp.id2 = 0;
                    tmp.cost = -1;
                END IF;
                tmp.id3 = -1;
                RETURN next tmp;
                i = i + 1;
                sseq = sseq + 1;
            END LOOP;
        IF (result.id1 = targets[i] AND result.id3 != -1) THEN
            --raise notice 'we are working on %', targets[i];
            result.seq = sseq;
            RETURN next result;
            sseq = sseq + 1;
            CONTINUE;
        END IF;
        IF (result.id1 = targets[i] AND result.id3 = -1) THEN
            --raise notice 'we are working on the last record of %', targets[i];
            result.seq = sseq;
            RETURN next result;
            i = i + 1;
            sseq = sseq + 1;
            CONTINUE;
        END IF;
    END LOOP;
    WHILE (i <= array_length(targets,1)) LOOP
        -- we didnt find the target so need to put the record for no path found
        --raise notice '%: not found targets:%', i, targets[i];
        tmp.seq = sseq;
        tmp.id1 = targets[i];
        IF (targets[i] = source) THEN
            tmp.id2 = source;
            tmp.cost = -1;
        ELSE
            tmp.id2 = 0;
            tmp.cost = -1;
        END IF;
        tmp.id3 = -1;
        RETURN next tmp;
        i = i + 1;
        sseq = sseq + 1;
    END LOOP;

END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;



CREATE OR REPLACE FUNCTION pgr_kdijkstracost(
    edges_sql text,
    start_vid INTEGER,
    end_vids INTEGER array,
    directed BOOLEAN,
    has_rcost BOOLEAN)
RETURNS SETOF pgr_costResult AS
$BODY$
DECLARE
has_reverse BOOLEAN;
new_sql TEXT;
BEGIN
    RAISE NOTICE 'Deprecated function';
    has_reverse =_pgr_parameter_check('dijkstra', edges_sql, false);
    new_sql = edges_sql;

    IF (has_reverse != has_rcost) THEN
        IF (has_reverse) THEN
            new_sql = 'SELECT id, source, target, cost FROM (' || edges_sql || ') a';
        else
            raise EXCEPTION 'has_rcost set to true but reverse_cost not found';
END IF;
END IF;

return query SELECT ((row_number() over()) -1)::INTEGER, a.start_vid::INTEGER, a.end_vid::INTEGER, agg_cost
FROM pgr_dijkstraCost(new_sql, start_vid, end_vids, directed) a;
END
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;



CREATE OR REPLACE FUNCTION pgr_vidsToDMatrix(sql text,
    vids integer[], dir bool, has_rcost bool, want_symmetric bool)
RETURNS float8[]
AS '$libdir/${PGROUTING_LIBRARY_NAME}', 'manytomany_dijkstra_dmatrix'
LANGUAGE C STABLE STRICT;
