BEGIN;

--------------------------------------------------------------------------------
--                pgr_version
--------------------------------------------------------------------------------
-- SELECT pgr_version(); -- DONT USE THIS IT CHANGES WITH EVERY COMMIT
SELECT version FROM pgr_version(); -- This changes infrequently
ROLLBACK;
