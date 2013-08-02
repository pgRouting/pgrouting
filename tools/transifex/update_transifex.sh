#!/bin/bash
# ------------------------------------------------------------------------------
# pgRouting Scripts
# Copyright(c) pgRouting Contributors
#
# Publish updated documentation to Transifex
# ------------------------------------------------------------------------------

DIRECTORY=$(cd `dirname $0` && pwd)

CONFIG="build/doc/_build"
DOCDIR="doc/i18n"

LANGUAGES=(de es)

echo "*************************************************************************"
echo "Create languages and update POT files"
echo "*************************************************************************"
for i in "${LANGUAGES[@]}"; do
	sphinx-intl update -l "${i}" -c "${CONFIG}/conf.py" -p "${DOCDIR}/pot" -d "${DOCDIR}"
done
 
echo "*************************************************************************"
echo "Register POT files for upload"
echo "*************************************************************************"
sphinx-intl update-txconfig-resources -c "${CONFIG}/conf.py" -p "${DOCDIR}/pot" -d "${DOCDIR}" --transifex-project-name=pgrouting
 
echo "*************************************************************************"
echo "Upload to Transifex"
echo "*************************************************************************"
tx push -s
