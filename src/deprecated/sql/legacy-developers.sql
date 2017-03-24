-------------------------------------------------------------------------------
-- Legacy functions that were ment to be used by 
-- pgRouting developers
-- These functions wer used on the plpgsql functions
-------------------------------------------------------------------------------


/*PGR-GNU*****************************************************************

FILE: noUnderUtilities.sql

Copyright (c) 2015 Celia Vriginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

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

-- deprecated on 2.1.0
CREATE OR REPLACE FUNCTION pgr_getColumnName(tab text, col text)
RETURNS text AS
$BODY$
DECLARE
    table_oid regclass;
BEGIN
    table_oid := tab::regclass;
    tab := replace(tab, '"', '');
    return _pgr_getColumnName(tab, col, 0, 'pgr_getColumnName');
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;




-- deprecated on 2.1.0
CREATE OR REPLACE FUNCTION pgr_getTableName(IN tab text,OUT sname text,OUT tname text)
RETURNS RECORD AS
$BODY$ 
DECLARE table_oid regclass;
BEGIN
    table_oid := tab::regclass;
    tab := replace(tab, '"', '');
    SELECT * from _pgr_getTableName(tab, 0, 'pgr_getTableName') into sname,tname;
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;





-- deprecated on 2.1.0
CREATE OR REPLACE FUNCTION pgr_isColumnIndexed(tab text, col text)
RETURNS boolean AS
$BODY$
DECLARE table_oid regclass;
BEGIN
    table_oid := tab::regclass;
    tab := replace(tab, '"', '');
    return  _pgr_isColumnIndexed(tab, col);
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;




-- deprecated on 2.1.0
CREATE OR REPLACE FUNCTION pgr_isColumnInTable(tab text, col text)
RETURNS boolean AS
$BODY$
DECLARE
    cname text;
    table_oid regclass;
BEGIN
    table_oid := tab::regclass;
    tab := replace(tab, '"', '');
    select * from _pgr_getColumnName(tab, col, 0, 'pgr_isColumnInTable') into cname;
    return  cname IS not NULL;
END;
$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;




-- deprecated on 2.1.0
create or replace function pgr_quote_ident(idname text)
returns text as
$BODY$
    SELECT  _pgr_quote_ident(idname);
$BODY$
LANGUAGE sql VOLATILE STRICT;





-- deprecated on 2.1.0
CREATE OR REPLACE FUNCTION pgr_versionless(v1 text, v2 text)
RETURNS boolean AS
$BODY$
BEGIN
    return  _pgr_versionless(v1,v2);
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;




-- deprecated on 2.1.0
create or replace function pgr_startPoint(g geometry)
    returns geometry as
$body$
    SELECT  ST_startPoint(g);
$body$
language sql IMMUTABLE;




-- deprecated on 2.1.0
create or replace function pgr_endPoint(g geometry)
    returns geometry as
$body$
    SELECT  ST_endPoint(g);
$body$
language sql IMMUTABLE;



