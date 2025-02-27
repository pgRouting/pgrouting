/*PGR-GNU*****************************************************************
File: _checkquery.sql

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
CREATE FUNCTION _pgr_checkquery(
  TEXT  -- SQL
)
RETURNS TEXT AS
$BODY$
DECLARE
  main_sql TEXT;

BEGIN

  IF $1 !~ '[[:space:]]' THEN
    -- prepared statements no arguments
    EXECUTE format($$
      SELECT regexp_replace(regexp_replace(statement, %1$L,'','i'),';$','')
      FROM pg_prepared_statements WHERE name = %2$L$$,
      '.*' || $1 || '\s*as', $1)
    INTO main_sql;

    IF main_sql IS NULL THEN
      RAISE EXCEPTION 'prepared statement "%" does not exist', $1;
    END IF;
  ELSE
    -- normal query
    main_sql := $1;
  END IF;

  BEGIN
    EXECUTE format('SELECT * FROM ( %1$s ) AS __a__ limit 1', main_sql);

    EXCEPTION WHEN OTHERS THEN
      RAISE EXCEPTION '%', SQLERRM USING HINT = $1, ERRCODE = SQLSTATE;
  END;

  RETURN main_sql;

END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

COMMENT ON FUNCTION _pgr_checkquery(TEXT)
IS 'pgrouting internal function';
