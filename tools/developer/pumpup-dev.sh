#!/bin/bash
# /*PGR-GNU*****************************************************************
# File: pumpup-dev.sh
# Copyright (c) 2020 pgRouting developers
# Mail: project@pgrouting.org
# ------
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
# ********************************************************************PGR-GNU*/
set -e

# ------------------------------------------------------------------------------
# pgRouting Scripts
# Copyright(c) pgRouting Contributors
#
# Pump up version
# ------------------------------------------------------------------------------

# Script to pump up branch to next minor development

OLD_VERSION=$(grep -Po '(?<=project\(PGROUTING VERSION )[^;]+' CMakeLists.txt)
KIND=$(grep -Po '(?<=set\(PROJECT_VERSION_DEV )[^;]+\"\)' CMakeLists.txt)
KIND=$(echo "${KIND}" | awk -F'"' '{print $2}')
echo "OLD_VERSION=${OLD_VERSION}"
echo "KIND=${KIND}"
IFS='\. ' read -r -a a <<< "${OLD_VERSION}"

MAYOR="${a[0]}"
MINOR="${a[1]}"
#MICRO="${a[2]}"

((a[1]++))
NEW_MINOR="${a[1]}"
NEW_MICRO="0"
NEW_KIND="-dev"
NEW_VERSION="${MAYOR}.${NEW_MINOR}.${NEW_MICRO}"

echo "NEW_VERSION=${NEW_VERSION}"

# --------------------------------------------
# Modifications to CMakeLists
# --------------------------------------------

# set version to new version
perl -pi -e 's/project\(PGROUTING VERSION (.*)$/project\(PGROUTING VERSION '"${NEW_VERSION}"'/g' CMakeLists.txt
perl -pi -e 's/set\(PROJECT_VERSION_DEV(.*)$/set\(PROJECT_VERSION_DEV "'"${NEW_KIND}"'"\)/g'  CMakeLists.txt
perl -pi -e 's/set\(MINORS(.*)$/set\(MINORS '"${MAYOR}"'.'"${NEW_MINOR}"'$1/g'  CMakeLists.txt
perl -pi -e 's/OLD_SIGNATURES$/OLD_SIGNATURES\n    '"${OLD_VERSION}"' /g' CMakeLists.txt

# --------------------------------------------
# --------------------------------------------
# sql directory
# --------------------------------------------
# --------------------------------------------

# --------------------------------------------
# Copy last signature file
# --------------------------------------------

cp -f "sql/sigs/pgrouting--${MAYOR}.${MINOR}.sig" "sql/sigs/pgrouting--${MAYOR}.${NEW_MINOR}.sig"
#git add "sql/sigs/pgrouting--${MAYOR}.${NEW_MINOR}.sig

# --------------------------------------------
# Adding section in release notes & news
# --------------------------------------------
perl -pi -e 's/(pgRouting '"${OLD_VERSION}"' Release Notes)/
pgRouting '"${NEW_VERSION}"' Release Notes
-------------------------------------------------------------------------------

No Changes Yet

$1/g' doc/src/release_notes.rst

tools/release-scripts/notes2news.pl

# --------------------------------------------
# tests are done to new version
# --------------------------------------------

perl -pi -e 's/'"${OLD_VERSION}"'/'"${NEW_VERSION}"'/g' tools/testers/pg_prove_tests.sh

# --------------------------------------------
# Change return values of version
# --------------------------------------------

perl -pi -e 's/'"${OLD_VERSION}${KIND}"'/'"${NEW_VERSION}${NEW_KIND}"'/' docqueries/version/*.result
perl -pi -e 's/'"${OLD_VERSION}"'/'"${NEW_VERSION}"'/' docqueries/version/*.result

# --------------------------------------------
# Update action to include old version
# --------------------------------------------

perl -pi -e 's/'"${OLD_VERSION}"'/'"${NEW_VERSION}"'/g' .github/workflows/update.yml
perl -pi -e 's/old_pgr: \[/old_pgr: \['"${OLD_VERSION}"', /g' .github/workflows/update.yml

# --------------------------------------------
# Include file in CMakeLists.txt
# --------------------------------------------

perl -pi -e 's/# add minor here/my \$version_'"${MAYOR}"'_'"${NEW_MINOR}"' = qr\/\('"${MAYOR}"'\.'"${NEW_MINOR}"'\.\[\\d\+\]\)\/;\n# add minor here/g' sql/scripts/build-extension-update-files.pl
perl -pi -e 's/my \$current = (.*)$/my \$current = \$version_'"${MAYOR}"'\_'"${NEW_MINOR}"';/g' sql/scripts/build-extension-update-files.pl
perl -pi -e 's/unless \$old_version =~ (.*)\/;$/unless \$old_version =~ $1|\$version_'"${MAYOR}"'_'"${NEW_MINOR}"'\/;/g' sql/scripts/build-extension-update-files.pl
