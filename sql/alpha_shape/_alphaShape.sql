/*PGR-GNU*****************************************************************

Copyright (c) 2018 Celia Virginia Vergara Castillo
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

CREATE OR REPLACE FUNCTION _pgr_alphaShape(
    TEXT, -- edges sql
    alpha FLOAT DEFAULT 0,

    OUT seq1 BIGINT,
    OUT textgeom TEXT)
RETURNS SETOF record
AS 'MODULE_PATHNAME', 'alphaShape'
LANGUAGE c VOLATILE STRICT;

-- COMMENTS

COMMENT ON FUNCTION _pgr_alphashape(TEXT, FLOAT)
IS 'pgrouting internal function';
