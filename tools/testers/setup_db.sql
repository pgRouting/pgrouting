\set QUIET 1

-- Tests for pgroutng.

-- Format the output for nice TAP.
\pset format unaligned
\pset tuples_only true
\pset pager

-- Revert all changes on failure.
\set ON_ERROR_ROLLBACK 1
\set ON_ERROR_STOP true

SET client_min_messages = WARNING;

CREATE EXTENSION postgis;
CREATE EXTENSION pgtap;
CREATE EXTENSION pgrouting;

BEGIN;

    \i sampledata.sql
    \i vrppdtw_data.sql
    \i pickDeliver_pgtap_data.sql
    \i innerQuery.sql
    \i innerQuery_old.sql
    \i inner_styles.sql
    \i old_inner_styles.sql

END;
