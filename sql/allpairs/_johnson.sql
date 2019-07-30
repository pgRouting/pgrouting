/*PGR-GNU*****************************************************************

File: _johnson.sql

Template:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function developer:
Copyright (c) 2015 Vicky Vergara
vicky_vergara@hotmail.com

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

------------------
-- pgr_johnson
------------------

CREATE OR REPLACE FUNCTION _pgr_johnson(
    edges_sql TEXT,
    directed BOOLEAN,

    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
'MODULE_PATHNAME'
LANGUAGE C VOLATILE STRICT;

-- COMMENTS

COMMENT ON FUNCTION _pgr_johnson(TEXT, BOOLEAN)
IS 'pgRouting internal function';
