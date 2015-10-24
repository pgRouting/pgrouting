--
-- Copyright (c) 2013 Stephen Woodbridge
--
-- This files is released under an MIT-X license.
--


-----------------------------------------------------------------------
-- Core function for TSP
-----------------------------------------------------------------------
/*
 * select seq, id from pgr_tsp(matrix float8[][], start int,
 *                             OUT seq int, OUT id int);
*/
-- endpt does not work, and is ignored in the code at the moment
-- we hope to support it in the future but the tsp algorithm needs to
-- change or be replaced to support this functionality.
CREATE OR REPLACE FUNCTION pgr_tsp(matrix float8[][], startpt integer, endpt integer DEFAULT -1, OUT seq integer, OUT id integer)

--CREATE OR REPLACE FUNCTION pgr_tsp(matrix float8[][], startpt integer, OUT seq integer, OUT id integer)
    RETURNS SETOF record
    AS '$libdir/lib${PGROUTING_LIBRARY_NAME}', 'tsp_matrix'
    LANGUAGE c IMMUTABLE STRICT;
