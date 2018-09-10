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

------------------------------------
-- 2 boykov_kolmogorov
------------------------------------


CREATE OR REPLACE FUNCTION pgr_boykovKolmogorov(
    TEXT,
    BIGINT,
    BIGINT,
    OUT seq INTEGER,
    OUT edge BIGINT,
    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT *
        FROM _pgr_maxflow(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], ARRAY[$3]::BIGINT[], 2);
  $BODY$
  LANGUAGE sql VOLATILE STRICT;



CREATE OR REPLACE FUNCTION pgr_boykovKolmogorov(
    TEXT,
    BIGINT,
    ANYARRAY,
    OUT seq INTEGER,
    OUT edge BIGINT,
    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT *
        FROM _pgr_maxflow(_pgr_get_statement($1), ARRAY[$2]::BIGINT[], $3::BIGINT[], 2);
  $BODY$
  LANGUAGE sql VOLATILE STRICT;



CREATE OR REPLACE FUNCTION pgr_boykovKolmogorov(
    TEXT,
    ANYARRAY,
    BIGINT,
    OUT seq INTEGER,
    OUT edge BIGINT,
    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT *
        FROM _pgr_maxflow(_pgr_get_statement($1), $2::BIGINT[], ARRAY[$3]::BIGINT[], 2);
  $BODY$
  LANGUAGE sql VOLATILE STRICT;


CREATE OR REPLACE FUNCTION pgr_boykovKolmogorov(
    TEXT,
    ANYARRAY,
    ANYARRAY,
    OUT seq INTEGER,
    OUT edge BIGINT,
    OUT start_vid BIGINT,
    OUT end_vid BIGINT,
    OUT flow BIGINT,
    OUT residual_capacity BIGINT
    )
  RETURNS SETOF RECORD AS
  $BODY$
        SELECT *
        FROM _pgr_maxflow(_pgr_get_statement($1), $2::BIGINT[], $3::BIGINT[], 2);
  $BODY$
  LANGUAGE sql VOLATILE STRICT;

