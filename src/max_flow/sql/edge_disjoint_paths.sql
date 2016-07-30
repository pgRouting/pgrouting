/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2016 Andrea Nardelli
mail: nrd.nardelli@gmail.com

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


/***********************************
        ONE TO ONE
***********************************/

CREATE OR REPLACE FUNCTION pgr_edgeDisjointPaths(
    IN edges_sql TEXT,
    IN source_vertex bigint,
    IN sink_vertex bigint,
    IN directed BOOLEAN DEFAULT TRUE,
    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT node BIGINT,
    OUT edge BIGINT
    )
  RETURNS SETOF RECORD AS
 '$libdir/${PGROUTING_LIBRARY_NAME}', 'edge_disjoint_paths_one_to_one'
    LANGUAGE c IMMUTABLE STRICT;

/***********************************
        ONE TO MANY
***********************************/

CREATE OR REPLACE FUNCTION pgr_edgeDisjointPaths(
    IN edges_sql TEXT,
    IN source_vertex bigint,
    IN sink_vertices ANYARRAY,
    IN directed BOOLEAN DEFAULT TRUE,
    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT end_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT
    )
  RETURNS SETOF RECORD AS
 '$libdir/${PGROUTING_LIBRARY_NAME}', 'edge_disjoint_paths_one_to_many'
    LANGUAGE c IMMUTABLE STRICT;

/***********************************
        MANY TO ONE
***********************************/

CREATE OR REPLACE FUNCTION pgr_edgeDisjointPaths(
    IN edges_sql TEXT,
    IN source_vertices ANYARRAY,
    IN sink_vertex BIGINT,
    IN directed BOOLEAN DEFAULT TRUE,
    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT start_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT
    )
  RETURNS SETOF RECORD AS
 '$libdir/${PGROUTING_LIBRARY_NAME}', 'edge_disjoint_paths_many_to_one'
    LANGUAGE c IMMUTABLE STRICT;

/***********************************
        MANY TO MANY
***********************************/

CREATE OR REPLACE FUNCTION pgr_edgeDisjointPaths(
    IN edges_sql TEXT,
    IN source_vertices ANYARRAY,
    IN sink_vertices ANYARRAY,
    IN directed BOOLEAN DEFAULT TRUE,
    OUT seq INTEGER,
    OUT path_seq INTEGER,
    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT node BIGINT,
    OUT edge BIGINT
    )
  RETURNS SETOF RECORD AS
 '$libdir/${PGROUTING_LIBRARY_NAME}', 'edge_disjoint_paths_many_to_many'
    LANGUAGE c IMMUTABLE STRICT;