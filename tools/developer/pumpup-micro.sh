#!/bin/bash
# /*PGR-GNU*****************************************************************
# File: pumpup-micro.sh
# Copyright (c) 2021 pgRouting developers
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
# ------------------------------------------------------------------------------
# pgRouting Scripts
# Copyright(c) pgRouting Contributors
#
# Pump up version
# ------------------------------------------------------------------------------

# Script to pump up branch to next development

OLD_VERSION=$(grep -Po '(?<=project\(PGROUTING VERSION )[^;]+' CMakeLists.txt)
KIND=$(grep -Po '(?<=set\(PROJECT_VERSION_DEV )[^;]+\"\)' CMakeLists.txt)
echo "KIND=${KIND}"
KIND=$(echo "${KIND}" | awk -F'"' '{print $2}')
echo "OLD_VERSION=${OLD_VERSION}"
echo "KIND=${KIND}"

IFS='\. ' read -r -a a <<< "${OLD_VERSION}"

MAYOR="${a[0]}"
MINOR="${a[1]}"
MICRO="${a[2]}"
((a[2]++))                              # increment micro
NEW_MICRO="${a[2]}"

echo "MAYOR=${MAYOR}"
echo "MINOR=${MINOR}"
echo "MICRO=${MICRO}"
echo "NEW_MICRO=${NEW_MICRO}"


NEW_VERSION="${MAYOR}.${MINOR}.${NEW_MICRO}"
U_NEW_VERSION="${NEW_VERSION//./_}"
U_OLD_VERSION="${OLD_VERSION//./_}"

echo "NEW_VERSION=${NEW_VERSION}"
echo "U_VERSION=${U_NEW_VERSION}"
echo "U_OLD_VERSION=${U_OLD_VERSION}"


# --------------------------------------------
# Modifications to CMakeLists
# --------------------------------------------

# set version to new version
perl -pi -e 's/project\(PGROUTING VERSION (.*)$/project\(PGROUTING VERSION '"${NEW_VERSION}"'/g' CMakeLists.txt
perl -pi -e 's/OLD_SIGNATURES$/OLD_SIGNATURES\n    '"${OLD_VERSION}"' /g' CMakeLists.txt

# --------------------------------------------
# Adding section in release notes & news
# --------------------------------------------
#perl -pi -e 's/(\* :ref:`changelog_'"${U_OLD_VERSION}"'`)$/\* :ref:`changelog_'"${U_NEW_VERSION}"'`\n$1/g' doc/src/release_notes.rst
#perl -pi -e 's/(\* :ref:`changelog_'"${U_OLD_VERSION}"'`)$/\* :ref:`changelog_'"${U_NEW_VERSION}"'`\n$1/g' doc/src/release_notes.rst
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

perl -pi -e 's/'"${OLD_VERSION}"'/'"${NEW_VERSION}"'/g' docqueries/version/*.result

# --------------------------------------------
# Update action to include old version
# --------------------------------------------

perl -pi -e 's/'"${OLD_VERSION}"'/'"${NEW_VERSION}"'/g' .github/workflows/update.yml
perl -pi -e 's/old_pgr: \[/old_pgr: \['"${OLD_VERSION}"', /g' .github/workflows/update.yml
