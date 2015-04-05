-- -------------------------------------------------------------------
-- pgrouting_utilities.sql
-- AuthorL Stephen Woodbridge <woodbri@imaptools.com>
-- Copyright 2013 Stephen Woodbridge
-- This file is release unde an MIT-X license.
-- -------------------------------------------------------------------


CREATE OR REPLACE FUNCTION pgr_getTableName(IN tab text,OUT sname text,OUT tname text)
RETURNS RECORD AS
$BODY$ 
BEGIN
    raise notice 'This function will no longer be soported';
    select * from _pgr_getTableName(tab) into sname,tname;
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

CREATE OR REPLACE FUNCTION pgr_getColumnName(tab text, col text)
RETURNS text AS
$BODY$
BEGIN
    raise notice 'This function will no longer be soported';
    return _pgr_getColumnName(tab,col);
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

CREATE OR REPLACE FUNCTION pgr_isColumnInTable(tab text, col text)
RETURNS boolean AS
$BODY$
BEGIN
    raise notice 'This function will no longer be soported';
    return _pgr_isColumnInTable(tab,col); -- into sname,tname;
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

CREATE OR REPLACE FUNCTION pgr_isColumnIndexed(tab text, col text)
RETURNS boolean AS
$BODY$
BEGIN
    raise notice 'This function will no longer be soported';
    return  _pgr_isColumnIndexed(tab,col);
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

CREATE OR REPLACE FUNCTION pgr_isColumnInTable(tab text, col text)
RETURNS boolean AS
$BODY$
BEGIN
    raise notice 'This function will no longer be soported';
    return _pgr_isColumnInTable(tab,col);
END;
$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;




create or replace function pgr_quote_ident(idname text)
returns text as
$BODY$
BEGIN
    raise notice 'This function will no longer be soported';
    return  _pgr_quote_ident(idname);
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

CREATE OR REPLACE FUNCTION pgr_versionless(v1 text, v2 text)
RETURNS boolean AS
$BODY$
BEGIN
    raise notice 'This function will no longer be soported';
    return  _pgr_versionless(v1,v2);
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

create or replace function pgr_startPoint(g geometry)
    returns geometry as
$body$
BEGIN
    raise notice 'This function will no longer be soported';
    return  _pgr_startPoint(g);
END;
$body$
language plpgsql IMMUTABLE;



create or replace function pgr_endPoint(g geometry)
    returns geometry as
$body$
BEGIN
    raise notice 'This function will no longer be soported';
    return  _pgr_endPoint(g);
END;
$body$
language plpgsql IMMUTABLE;
