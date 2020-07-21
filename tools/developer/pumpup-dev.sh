#!/bin/bash

set -e

# ------------------------------------------------------------------------------
# pgRouting Scripts
# Copyright(c) pgRouting Contributors
#
# Pump up version
# ------------------------------------------------------------------------------

# Script to pump up branch to next development

MAYOR="$1"
MINOR="$2"
MICRO="$3"
KIND="$4"
OLD_VERSION="$5"
VERSION="${MAYOR}.${MINOR}.${MICRO}"
U_VERSION=$(echo "${VERSION//./_}")
U_OLD_VERSION=$(echo "${OLD_VERSION//./_}")

# --------------------------------------------
# Modifications to CMakeLists
# --------------------------------------------

# set version to new version
perl -pi -e 's/project\(PGROUTING VERSION (.*)$/project\(PGROUTING VERSION '${VERSION}'/g' CMakeLists.txt

# set PGROUTING_VERSION_DEV to "-dev"
perl -pi -e 's/set\(PGROUTING_VERSION_DEV(.*)$/set\(PGROUTING_VERSION_DEV "'${KIND}'"\)/g'  CMakeLists.txt

# --------------------------------------------
# --------------------------------------------
# sql directory
# --------------------------------------------
# --------------------------------------------

# --------------------------------------------
# Copy last signature file
# --------------------------------------------

cp -f "sql/sigs/pgrouting--${OLD_VERSION}.sig" "sql/sigs/pgrouting--${VERSION}.sig"
ls "sql/sigs/pgrouting--${VERSION}.sig"
perl -pi -e 's/#VERSION pgrouting (.*)/#VERSION pgrouting '${VERSION}'/g' "sql/sigs/pgrouting--${VERSION}.sig"
git add "sql/sigs/pgrouting--${VERSION}.sig"

# --------------------------------------------
# Include file in CMakeLists.txt
# --------------------------------------------

perl -pi -e 's/SET\(OLD_SIGNATURES/SET\(OLD_SIGNATURES\n    '${OLD_VERSION}'/g' sql/sigs/CMakeLists.txt


#if ! grep "my \$version_${MAYOR}_${MINOR} = qr" sql/scripts/build-extension-update-files1.pl
perl -pi -e 's/# add minor here/my \$version_'${MAYOR}'_'${MINOR}' = qr\/\('${MAYOR}'\.'${MINOR}'\.\[\\d\+\]\)\/;\n# add minor here/g' sql/scripts/build-extension-update-files1.pl
perl -pi -e 's/my \$current = (.*)$/my \$current = \$version_'${MAYOR}'\_'${MINOR}';/g' sql/scripts/build-extension-update-files1.pl
perl -pi -e 's/unless \$old_version =~ (.*)\/;$/unless \$old_version =~ $1|\$version_'${MAYOR}'_'${MINOR}'\/;/g' sql/scripts/build-extension-update-files1.pl


# --------------------------------------------
# Adding section in release notes & news
# --------------------------------------------
perl -pi -e 's/(\* :ref:`changelog_'${U_OLD_VERSION}'`)$/\* :ref:`changelog_'${U_VERSION}'`\n$1/g' doc/src/release_notes.rst
perl -pi -e 's/(\.\. _changelog_'${U_OLD_VERSION}':)$/
.. _changelog_'${U_VERSION}':

pgRouting '${VERSION}' Release Notes
-------------------------------------------------------------------------------

No Changes Yet

$1/g' doc/src/release_notes.rst

tools/release-scripts/notes2news.pl



# --------------------------------------------
# Change return values of version
# --------------------------------------------

perl -pi -e 's/'${OLD_VERSION}'/'${VERSION}'/g' docqueries/version/*.result

