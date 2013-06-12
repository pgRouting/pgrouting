--
-- Copyright (c) 2005 Sylvain Pasche,
--               2006-2007 Anton A. Patrushev, Orkney, Inc.
--
-- This program is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation; either version 2 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with this program; if not, write to the Free Software
-- Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
--


-----------------------------------------------------------------------
-- Core function for TSP
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION pgr_tsp(sql text, ids varchar, source integer)
    RETURNS SETOF pgr_costResult
    AS '$libdir/librouting_tsp', 'tsp'
    LANGUAGE c IMMUTABLE STRICT;

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
    AS '$libdir/librouting_tsp', 'tsp_matrix'
    LANGUAGE c IMMUTABLE STRICT;
