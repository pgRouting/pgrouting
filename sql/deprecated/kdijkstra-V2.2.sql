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
            WHILE (result.id1 != targets[i]) LOOP
                tmp.seq = sseq;
                tmp.id1 = targets[i];
                IF (targets[i] = source) THEN
                    tmp.id2 = source;
                    tmp.cost =0;
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
            result.seq = sseq;
            RETURN next result;
            sseq = sseq + 1;
            CONTINUE;
        END IF;
        IF (result.id1 = targets[i] AND result.id3 = -1) THEN
            result.seq = sseq;
            RETURN next result;
            i = i + 1;
            sseq = sseq + 1;
            CONTINUE;
        END IF;
    END LOOP;
    WHILE (i <= array_length(targets,1)) LOOP
        tmp.seq = sseq;
        tmp.id1 = targets[i];
        IF (targets[i] = source) THEN
            tmp.id2 = source;
            tmp.cost = 0;
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
LANGUAGE plpgsql VOLATILE STRICT
COST 100
ROWS 1000;


CREATE OR REPLACE FUNCTION pgr_kdijkstracost(
    sql text,
    source INTEGER,
    targets INTEGER array,
    directed BOOLEAN,
    has_rcost BOOLEAN)
RETURNS SETOF pgr_costResult AS
$BODY$
DECLARE
has_reverse BOOLEAN;
new_sql TEXT;
result pgr_costResult;
tmp pgr_costResult;
sseq INTEGER;
i INTEGER;
BEGIN
    RAISE NOTICE 'Deprecated function. Use pgr_dijkstraCost instead.';
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
            RAISE EXCEPTION 'Error, reverse_cost is used, but query did''t return ''reverse_cost'' column'
            USING ERRCODE = 'XX000';
        END IF;
    END IF;

    SELECT ARRAY(SELECT DISTINCT UNNEST(targets) ORDER BY 1) INTO targets;

    sseq = 0; i = 1;
    FOR result IN
        SELECT ((row_number() over()) -1)::INTEGER, a.start_vid::INTEGER, a.end_vid::INTEGER, agg_cost
        FROM pgr_dijkstraCost(new_sql, source, targets, directed) a ORDER BY end_vid LOOP
        WHILE (result.id2 != targets[i]) LOOP
            tmp.seq = sseq;
            tmp.id1 = source;
            tmp.id2 = targets[i];
            IF (targets[i] = source) THEN
                tmp.cost = 0;
            ELSE
                tmp.cost = -1;
            END IF;
            RETURN next tmp;
            i = i + 1;
            sseq = sseq + 1;
        END LOOP;
        IF (result.id2 = targets[i]) THEN
            result.seq = sseq;
            RETURN next result;
            i = i + 1;
            sseq = sseq + 1;
        END IF;
    END LOOP;
    WHILE (i <= array_length(targets,1)) LOOP
        tmp.seq = sseq;
        tmp.id1 = source;
        tmp.id2 = targets[i];
        IF (targets[i] = source) THEN
            tmp.cost = 0;
        ELSE
            tmp.cost = -1;
        END IF;
        RETURN next tmp;
        i = i + 1;
        sseq = sseq + 1;
    END LOOP;

END
$BODY$
LANGUAGE plpgsql VOLATILE STRICT
COST 100
ROWS 1000;


