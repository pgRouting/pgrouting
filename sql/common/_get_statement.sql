/*PGR-GNU*****************************************************************

 _get_statement.sql

 Copyright (c) 2014 Celia Virginia Vergara Castillo
 mail: vicky at erosion.dev

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

--v3.0
CREATE FUNCTION _pgr_get_statement(o_sql text)
RETURNS text AS
$BODY$
DECLARE
sql TEXT;
BEGIN
    EXECUTE 'SELECT statement FROM pg_prepared_statements WHERE name ='  || quote_literal(o_sql) || ' limit 1 ' INTO sql;
    IF (sql IS NULL) THEN
      RETURN   o_sql;
    ELSE
      RETURN  regexp_replace(regexp_replace(regexp_replace(sql, '\s(as)\s', '___foo___', 'i'), '^.*___foo___', '','i'), ';$', '');
    END IF;
END
$BODY$
LANGUAGE plpgsql STABLE STRICT;

COMMENT ON FUNCTION _pgr_get_statement(TEXT)
IS 'pgRouting internal function';
