CREATE OR REPLACE FUNCTION pgr_version()
RETURNS TABLE(
		"version" varchar, 
		tag varchar,
		build varchar,
		hash varchar,
		branch varchar,
		boost varchar
	) AS
$BODY$
DECLARE

BEGIN
    RETURN QUERY SELECT '${PGROUTING_VERSION_STRING}'::varchar AS version, 
    					'${PGROUTING_GIT_TAG}'::varchar AS tag, 
                        '${PGROUTING_GIT_BULD}'::varchar AS build, 
                        '${PGROUTING_GIT_HASH}'::varchar AS hash, 
                        '${PGROUTING_GIT_BRANCH}'::varchar AS branch, 
                        '${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}'::varchar AS boost;
END;
$BODY$
LANGUAGE plpgsql IMMUTABLE;

