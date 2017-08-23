/*
-- -------------------------------------------------------------------
-- pgrouting_version.sql
-- AuthorL Stephen Woodbridge <woodbri@imaptools.com>
-- Copyright 2013 Stephen Woodbridge
-- This file is release unde an MIT-X license.
-- -------------------------------------------------------------------
*/

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
    SELECT '${PGROUTING_VERSION}'::varchar AS version,
        '${PGROUTING_FULL_VERSION}'::varchar AS tag,
        '${PGROUTING_GIT_HASH}'::varchar AS hash,
        '${PGROUTING_GIT_BRANCH}'::varchar AS branch,
        '${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}'::varchar AS boost;
$BODY$
LANGUAGE sql IMMUTABLE;
