-- -------------------------------------------------------------------
-- pgrouting_version.sql
-- AuthorL Stephen Woodbridge <woodbri@imaptools.com>
-- Copyright 2013 Stephen Woodbridge
-- This file is release unde an MIT-X license.
-- -------------------------------------------------------------------

/*
.. function:: pgr_version()

   Author: Stephen Woodbridge <woodbri@imaptools.com>

   Returns the version of pgrouting,Git build,Git hash, Git branch and boost
*/

CREATE OR REPLACE FUNCTION pgr_version()
RETURNS TABLE(
        "version" varchar,
        tag varchar,
        hash varchar,
        branch varchar,
        boost varchar
    ) AS
$BODY$
    SELECT '2.5.0'::varchar AS version,
        'v2.5.0-dev'::varchar AS tag,
        ''::varchar AS hash,
        ''::varchar AS branch,
        '..'::varchar AS boost;

$BODY$
LANGUAGE sql IMMUTABLE;

