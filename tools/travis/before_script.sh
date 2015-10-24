#!/bin/bash
# ------------------------------------------------------------------------------
# Travis CI scripts 
# Copyright(c) pgRouting Contributors
#
# Build pgRouting
# ------------------------------------------------------------------------------

# exit script on error
set -e 

psql --version
psql -U postgres -c "SELECT version();"

