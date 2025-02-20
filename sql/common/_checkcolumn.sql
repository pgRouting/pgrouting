/*PGR-GNU*****************************************************************
File: _checkcolumn.sql

Copyright (c) 2022 ~ Celia Virginia Vergara Castillo
mail: vicky@georepublic.de

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


--v3.4
CREATE FUNCTION _pgr_checkColumn(
  TEXT, -- SQL
  TEXT, -- Column
  TEXT, -- Types
  is_optional BOOLEAN DEFAULT false,
  dryrun BOOLEAN DEFAULT false
)
RETURNS BOOLEAN AS
$BODY$
DECLARE
  has_column BOOLEAN := TRUE;
  rec RECORD;
  sqlhint TEXT;

BEGIN

  BEGIN
    EXECUTE format('SELECT %1$s FROM ( %2$s ) AS __a__ limit 1', $2, $1);
    EXCEPTION WHEN OTHERS THEN
      BEGIN
      IF NOT is_optional THEN
        RAISE EXCEPTION '%', SQLERRM USING HINT = $1, ERRCODE = SQLSTATE;
      ELSE
        has_column := FALSE;
      END IF;
      END;
  END;

  BEGIN
    EXECUTE format('SELECT pg_typeof(%1$s) FROM ( %2$s ) AS __a__ limit 1', $2, $1)
    INTO rec;

    EXCEPTION WHEN OTHERS THEN
      has_column := FALSE;
  END;

  IF dryrun THEN
    RETURN has_column;
  END IF;

  IF NOT is_optional AND NOT has_column THEN
    RAISE EXCEPTION 'Missing column'
    USING HINT = format('Column "%1$s" missing in: %2$s', $2, $1);
  END IF;

  IF has_column THEN
    CASE $3
    WHEN 'ANY-INTEGER' THEN
      IF  rec.pg_typeof NOT IN ('smallint','integer','bigint') THEN
        RAISE EXCEPTION 'Expected type of column "%" is ANY-INTEGER', $2
        USING HINT = 'Query: ' || $1;
      END IF;
    WHEN 'ANY-INTEGER[]' THEN
      IF  rec.pg_typeof NOT IN ('smallint[]','integer[]','bigint[]') THEN
        RAISE EXCEPTION 'Expected type of column "%" is ANY-INTEGER-ARRAY', $2
        USING HINT = 'Query: ' || $1;
      END IF;
    WHEN 'ANY-NUMERICAL' THEN
      IF  rec.pg_typeof NOT IN ('smallint','integer','bigint','real','float','numeric') THEN
        RAISE EXCEPTION 'Expected type of column "%s" is ANY-NUMERICAL', $2
        USING HINT = 'Query: ' || $1;
      END IF;
    ELSE
      IF rec.pg_typeof::TEXT != $3 THEN
        RAISE EXCEPTION 'Expected type of column "%" is %', $2, $3
        USING HINT = 'Query: ' || $1;
      END IF;
    END CASE;
  END IF;

  RETURN has_column;

END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

COMMENT ON FUNCTION _pgr_checkColumn(TEXT, TEXT, TEXT, BOOLEAN, BOOLEAN)
IS 'pgRouting internal function';
