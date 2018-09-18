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


--FUNCTIONS

CREATE OR REPLACE FUNCTION pgr_maxFlowPushRelabel(
    edges_sql TEXT,
    source_vertex BIGINT,
    sink_vertex BIGINT,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
  BEGIN
        RETURN QUERY SELECT *
        FROM pgr_PushRelabel($1, $2, $3);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE STRICT;

CREATE OR REPLACE FUNCTION pgr_maxFlowBoykovKolmogorov(
    edges_sql TEXT,
    source_vertex BIGINT,
    sink_vertex BIGINT,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
  BEGIN
        RETURN QUERY SELECT *
        FROM pgr_boykovKolmogorov($1, $2, $3);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE STRICT;

CREATE OR REPLACE FUNCTION pgr_maxFlowEdmondsKarp(
    edges_sql TEXT,
    source_vertex BIGINT,
    sink_vertex BIGINT,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
  BEGIN
        RETURN QUERY SELECT *
        FROM pgr_edmondsKarp($1, $2, $3);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE STRICT;

/***********************************
        ONE TO MANY
***********************************/

--INTERNAL FUNCTIONS

CREATE OR REPLACE FUNCTION pgr_maxFlowPushRelabel(
    edges_sql TEXT,
    source_vertex BIGINT,
    sink_vertices ANYARRAY,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
  BEGIN
        RETURN QUERY SELECT *
        FROM pgr_PushRelabel($1, $2, $3);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE STRICT;

CREATE OR REPLACE FUNCTION pgr_maxFlowBoykovKolmogorov(
    edges_sql TEXT,
    source_vertex BIGINT,
    sink_vertices ANYARRAY,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
  BEGIN
        RETURN QUERY SELECT *
        FROM pgr_boykovKolmogorov($1, $2, $3);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE STRICT;

CREATE OR REPLACE FUNCTION pgr_maxFlowEdmondsKarp(
    edges_sql TEXT,
    source_vertex BIGINT,
    sink_vertices ANYARRAY,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
  BEGIN
        RETURN QUERY SELECT *
        FROM pgr_edmondsKarp($1, $2, $3);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE STRICT;

/***********************************
        MANY TO ONE
***********************************/

--FUNCTIONS

CREATE OR REPLACE FUNCTION pgr_maxFlowPushRelabel(
    edges_sql TEXT,
    source_vertices ANYARRAY,
    sink_vertex BIGINT,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
  BEGIN
        RETURN QUERY SELECT *
        FROM pgr_PushRelabel($1, $2, $3);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE STRICT;

CREATE OR REPLACE FUNCTION pgr_maxFlowBoykovKolmogorov(
    edges_sql TEXT,
    source_vertices ANYARRAY,
    sink_vertex BIGINT,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
  BEGIN
        RETURN QUERY SELECT *
        FROM pgr_boykovKolmogorov($1, $2, $3);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE STRICT;

CREATE OR REPLACE FUNCTION pgr_maxFlowEdmondsKarp(
    edges_sql TEXT,
    source_vertices ANYARRAY,
    sink_vertex BIGINT,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
  BEGIN
        RETURN QUERY SELECT *
        FROM pgr_edmondsKarp($1, $2, $3);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE STRICT;

/***********************************
        MANY TO MANY
***********************************/


--FUNCTIONS

CREATE OR REPLACE FUNCTION pgr_maxFlowPushRelabel(
    edges_sql TEXT,
    source_vertices ANYARRAY,
    sink_vertices ANYARRAY,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
  BEGIN
        RETURN QUERY SELECT *
        FROM pgr_PushRelabel($1, $2, $3);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE STRICT;

CREATE OR REPLACE FUNCTION pgr_maxFlowBoykovKolmogorov(
    edges_sql TEXT,
    source_vertices ANYARRAY,
    sink_vertices ANYARRAY,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
  BEGIN
        RETURN QUERY SELECT *
        FROM pgr_boykovKolmogorov($1, $2, $3);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE STRICT;

CREATE OR REPLACE FUNCTION pgr_maxFlowEdmondsKarp(
    edges_sql TEXT,
    source_vertices ANYARRAY,
    sink_vertices ANYARRAY,
    OUT seq INTEGER,
    OUT edge_id BIGINT,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
  BEGIN
        RETURN QUERY SELECT *
        FROM pgr_edmondsKarp($1, $2, $3);
  END
  $BODY$
  LANGUAGE plpgsql VOLATILE STRICT;

