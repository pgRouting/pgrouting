-------------------------------------------------------------------------------
-- Legacy FUNCTIONs that were ment to be used by 
-- pgRouting developers
-- These FUNCTIONs wer used on the plpgsql functions


-- FILE intended to be used on 3.0
-------------------------------------------------------------------------------


-- deprecated on 2.1.0
CREATE OR REPLACE FUNCTION pgr_getColumnName(tab text, col text)
RETURNS text AS
$BODY$
DECLARE
    table_oid regclass;
BEGIN
    table_oid := $1::regclass;
    $1 := replace($1, '"', '');
    return _pgr_getColumnName($1, col, 0, 'pgr_getColumnName');
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;




-- deprecated on 2.1.0
CREATE OR REPLACE FUNCTION pgr_getTableName(IN tab text,OUT sname text,OUT tname text)
RETURNS RECORD AS
$BODY$ 
DECLARE table_oid regclass;
BEGIN
    table_oid := $1::regclass;
    $1 := replace($1, '"', '');
    SELECT * FROM _pgr_getTableName($1, 0, 'pgr_getTableName') into sname,tname;
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;





-- deprecated on 2.1.0
CREATE OR REPLACE FUNCTION pgr_isColumnIndexed(tab text, col text)
RETURNS boolean AS
$BODY$
DECLARE table_oid regclass;
BEGIN
    table_oid := $1::regclass;
    $1 := replace(tab, '"', '');
    return  _pgr_isColumnIndexed($1, $2);
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
    table_oid := $1::regclass;
    $1 := replace($1, '"', '');
    SELECT * FROM _pgr_getColumnName($1, $2, 0, 'pgr_isColumnInTable') into cname;
    return  cname IS not NULL;
END;
$BODY$
  LANGUAGE plpgsql VOLATILE STRICT;




-- deprecated on 2.1.0
CREATE OR REPLACE FUNCTION pgr_quote_ident(idname text)
returns text as
$BODY$
    SELECT  quote_ident($1);
$BODY$
LANGUAGE sql IMMUTABLE;





-- deprecated on 2.1.0
CREATE OR REPLACE FUNCTION pgr_versionless(v1 text, v2 text)
RETURNS boolean AS
$BODY$
    SELECT _pgr_versionless($1, $2);
$BODY$
LANGUAGE sql VOLATILE STRICT;




-- deprecated on 2.1.0
CREATE OR REPLACE FUNCTION pgr_startPoint(g geometry)
    returns geometry as
$BODY$
    SELECT  ST_startPoint($1);
$BODY$
LANGUAGE sql IMMUTABLE;




-- deprecated on 2.1.0
CREATE OR REPLACE FUNCTION pgr_endPoint(g geometry)
    returns geometry as
$BODY$
    SELECT  ST_endPoint($1);
$BODY$
LANGUAGE sql IMMUTABLE;



-- deprecated on 2.1.0
CREATE OR REPLACE FUNCTION pgr_pointoid(point geometry, tolerance double precision, vertname text, srid integer)
RETURNS bigint AS
$BODY$
    SELECT _pgr_pointoid(i$1, $2, $3, $4);
$BODY$
LANGUAGE sql VOLATILE STRICT;

