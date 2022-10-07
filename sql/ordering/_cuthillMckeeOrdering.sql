/*PGR-GNU*****************************************************************
File: _cuthillMckeeOrdering.sql

Generated with Template by:
Copyright (c) 2022 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2022 Shobhit Chaurasia
Mail: 000shobhitchaurasia at gmail.com

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

----------------------------
-- _pgr_cuthillMckeeOrdering
----------------------------

--v3.4.0
CREATE FUNCTION _pgr_cuthillMckeeOrdering(
    TEXT,
    OUT seq BIGINT,
    OUT node BIGINT
    )

RETURNS SETOF RECORD AS
'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

-- COMMENTS

COMMENT ON FUNCTION _pgr_cuthillMckeeOrdering(TEXT)
IS 'pgRouting internal function';

