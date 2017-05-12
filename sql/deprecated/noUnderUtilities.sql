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


CREATE OR REPLACE FUNCTION pgr_getTableName(IN tab text,OUT sname text,OUT tname text)
RETURNS RECORD AS
$BODY$
BEGIN
    raise notice 'pgr_getTableName: This function will no longer be soported';
    select * from _pgr_getTableName(tab, 0, 'pgr_getTableName') into sname,tname;
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

CREATE OR REPLACE FUNCTION pgr_getColumnName(tab text, col text)
RETURNS text AS
$BODY$
BEGIN
    raise notice 'pgr_getColumnName: This function will no longer be soported';
    return _pgr_getColumnName(tab,col, 0, 'pgr_getColumnName');
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

CREATE OR REPLACE FUNCTION pgr_isColumnInTable(tab text, col text)
RETURNS boolean AS
$BODY$
DECLARE
    cname text;
BEGIN
    raise notice 'pgr_isColumnInTable: This function will no longer be soported';
    select * from _pgr_getColumnName(tab,col,0, 'pgr_isColumnInTable') into cname;
    return  cname IS not NULL;
END;
$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;

CREATE OR REPLACE FUNCTION pgr_isColumnIndexed(tab text, col text)
RETURNS boolean AS
$BODY$
BEGIN
    raise notice 'pgr_isColumnIndexed: This function will no longer be soported';
    return  _pgr_isColumnIndexed(tab,col);
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;


create or replace function pgr_quote_ident(idname text)
returns text as
$BODY$
BEGIN
    raise notice 'pgr_isColumnInTable: This function will no longer be soported';
    return  _pgr_quote_ident(idname);
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

CREATE OR REPLACE FUNCTION pgr_versionless(v1 text, v2 text)
RETURNS boolean AS
$BODY$
BEGIN
    raise notice 'pgr_versionless: This function will no longer be soported';
    return  _pgr_versionless(v1,v2);
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

create or replace function pgr_startPoint(g geometry)
    returns geometry as
$body$
BEGIN
    raise notice 'pgr_startPoint: This function will no longer be soported';
    return  _pgr_startPoint(g);
END;
$body$
language plpgsql IMMUTABLE;



create or replace function pgr_endPoint(g geometry)
    returns geometry as
$body$
BEGIN
    raise notice 'pgr_endPoint: This function will no longer be soported';
    return  _pgr_endPoint(g);
END;
$body$
language plpgsql IMMUTABLE;
