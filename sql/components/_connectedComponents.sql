/*PGR-GNU*****************************************************************
File: _connectedComponents.sql

Generated with Template by:
Copyright (c) 2016 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2017 Maoguang Wang
Mail: xjtumg1007@gmail.com

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

---------------
---------------
--  COMPONENTS
---------------
---------------

--------------------------
-- pgr_connectedComponents
--------------------------

CREATE OR REPLACE FUNCTION _pgr_connectedComponents(
    edges_sql TEXT,

    OUT seq BIGINT,
    OUT component BIGINT,
    OUT node BIGINT)

RETURNS SETOF RECORD AS
'MODULE_PATHNAME', 'connectedComponents'
LANGUAGE c IMMUTABLE STRICT;

-- COMMENTS

COMMENT ON FUNCTION _pgr_connectedComponents(TEXT)
IS 'pgRouting internal function';
