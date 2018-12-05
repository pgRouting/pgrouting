/*PGR-GNU*****************************************************************

Copyright (c) 2015 Celia Virginia Vergara Castillo
Copyright (c) 2006-2007 Anton A. Patrushev, Orkney, Inc.
Copyright (c) 2005 Sylvain Pasche,
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
--------------
--------------
-- alpha_shape
--------------
--------------


------------------
-- pgr_alphashape
------------------


/*
-----------------------------------------------------------------------
-- Core function for alpha shape computation.
-- The sql should return vertex ids and x,y values. Return ordered
-- vertex ids.
-----------------------------------------------------------------------
*/
CREATE OR REPLACE FUNCTION pgr_alphashape(
    TEXT, -- sql (required)
    alpha FLOAT8 DEFAULT 0,

    OUT x FLOAT8,
    OUT y FLOAT8)
    RETURNS SETOF record
    AS 'MODULE_PATHNAME', 'alphashape'
    LANGUAGE c VOLATILE STRICT;

COMMENT ON FUNCTION pgr_alphashape(TEXT, FLOAT8)
IS 'pgr_alphashape
- Parameters
	- An SQL with columns: id, x, y 
- Optional Parameters
	- alpha := 0
- Documentation:
    - ${PGROUTING_DOC_LINK}/pgr_alphashape.html
';