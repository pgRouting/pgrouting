#!/bin/bash
# ------------------------------------------------------------------------------
# Travis CI scripts 
# Copyright(c) pgRouting Contributors
#
# Test pgRouting
# ------------------------------------------------------------------------------

DBUSER="gis"
DATABASE="gis"

psql -U $DBUSER -d $DATABASE -c "CREATE EXTENSION pgrouting;"
../tools/test-runner.pl
