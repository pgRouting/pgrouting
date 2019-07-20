/*PGR-GNU*****************************************************************

FILE: _startPoint.sql

Copyright (c) 2019 Regina Obe

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

CREATE OR REPLACE FUNCTION _pgr_startpoint(g geometry)
RETURNS geometry AS
$$
SELECT CASE WHEN geometryType($1) ~ '^MULTI' THEN ST_StartPoint(ST_geometryN($1,1))
ELSE ST_StartPoint($1)
END;
$$
LANGUAGE sql IMMUTABLE;

COMMENT ON FUNCTION _pgr_startPoint(geometry)
IS 'pgRouting internal function';
