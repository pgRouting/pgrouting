#!/bin/bash
# ------------------------------------------------------------------------------
# pgRouting Scripts
# Copyright(c) pgRouting Contributors
#
# Publish updated documentation to Transifex
# ------------------------------------------------------------------------------

CONFIG="build/doc/_build"
DOCDIR="doc/i18n"

LANGUAGES=(de es ja)

echo "*************************************************************************"
echo "Pull translations from Transifex and commit"
echo "*************************************************************************"
for i in "${LANGUAGES[@]}"; do
	tx pull -l "${i}" -f --minimum-perc=1
done
git commit -m "pulled translations from Transifex"

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
