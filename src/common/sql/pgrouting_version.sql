CREATE OR REPLACE FUNCTION pgr_version()
RETURNS TABLE(
		"version" varchar, 
		tag varchar,
		build varchar,
		hash varchar,
		branch varchar
	) AS
$BODY$
DECLARE

BEGIN
    RETURN QUERY SELECT '${PGROUTING_VERSION_STRING}'::varchar AS version, 
    					'${PGROUTING_GIT_TAG}'::varchar AS tag, 
                        '${PGROUTING_GIT_BULD}'::varchar AS build, 
                        '${PGROUTING_GIT_HASH}'::varchar AS hash, 
                        's${PGROUTING_GIT_BRANCH}'::varchar AS branch;
END;
$BODY$
LANGUAGE plpgsql IMMUTABLE;

