#!/bin/bash

# ------------------------------------------------------------------------------
# pgRouting Scripts
# Copyright(c) pgRouting Contributors
#
# Pump up version
# ------------------------------------------------------------------------------
# shellcheck disable=SC2016
WHAT_NEXT=$1

# Script to pump up branch to next development
# Usage:
# bash tools/developer/pumpup_version.sh minor


OLD_VERSION=$(grep -Po '(?<=project\(PGROUTING VERSION )[^;]+' CMakeLists.txt)
KIND=$(grep -Po '(?<=set\(PROJECT_VERSION_DEV )[^;]+\"\)' CMakeLists.txt)
echo "KIND=${KIND}"
KIND=$(echo "${KIND}" | awk -F'"' '{print $2}')
echo "OLD_VERSION=${OLD_VERSION}"
echo "KIND=${KIND}"

IFS='\. ' read -r -a a <<< "${OLD_VERSION}"

MAYOR="${a[0]}"
MINOR="${a[1]}"

function pumpup_doc {
    echo "pump up doc"
    OLDSTR='^  \(`'"${1}"' (.*)\/'"${1}"'(.*)\)$'
    NEWSTR='  \(`'"${2}"' $1\/'"${2}"'$2\)\n  `'"${1}"' $1\/'"${1}"'$2'
    perl -pi -e 's/'"$OLDSTR"'/'"${NEWSTR}"'/' "$(git ls-files './*.rst')"
}

function pumpup_mayor {
    ((a[0]++))
    NEW_MAYOR="${a[0]}"
    NEW_MINOR="0"
    NEW_MICRO="0"
    NEW_KIND="-dev"
    pumpup_doc "${MAYOR}.${MINOR}" "${NEW_MAYOR}.${NEW_MINOR}"
}

function pumpup_minor {
    ((a[1]++))
    NEW_MAYOR="${a[0]}"
    NEW_MINOR="${a[1]}"
    NEW_MICRO="0"
    NEW_KIND="-dev"
    pumpup_doc "${MAYOR}.${MINOR}" "${NEW_MAYOR}.${NEW_MINOR}"
}

function pumpup_micro {
    ((a[2]++))
    NEW_MAYOR="${a[0]}"
    NEW_MINOR="${a[1]}"
    NEW_MICRO="${a[2]}"
    NEW_KIND="${KIND}"
}


if [ "${WHAT_NEXT}" == "mayor" ]
then
    pumpup_mayor
elif [ "${WHAT_NEXT}" == "micro" ]
then
    pumpup_micro
elif [ "${WHAT_NEXT}" == "minor" ]
then
    pumpup_minor
fi


NEW_VERSION="${NEW_MAYOR}.${NEW_MINOR}.${NEW_MICRO}"
echo "pumpup from ${OLD_VERSION}${KIND} to ${NEW_VERSION}${NEW_KIND}"

# --------------------------------------------
# --------------------------------------------
# Modifications to CMakeLists
# --------------------------------------------
# --------------------------------------------

# set version to new version
perl -pi -e 's/project\(PGROUTING VERSION (.*)$/project\(PGROUTING VERSION '"${NEW_VERSION}"'/g' CMakeLists.txt
perl -pi -e 's/set\(PROJECT_VERSION_DEV(.*)$/set\(PROJECT_VERSION_DEV "'"${NEW_KIND}"'"\)/g'  CMakeLists.txt
perl -pi -e 's/OLD_SIGNATURES$/OLD_SIGNATURES\n    '"${OLD_VERSION}"'/g' CMakeLists.txt
if [ "${WHAT_NEXT}" != "micro" ]
then
    perl -pi -e 's/set\(MINORS(.*)$/set\(MINORS '"${NEW_MAYOR}"'.'"${NEW_MINOR}"'$1/g'  CMakeLists.txt
fi
# --------------------------------------------
# --------------------------------------------
# sql directory
# --------------------------------------------
# --------------------------------------------

# --------------------------------------------
# Copy last signature file
# --------------------------------------------

if [ "${WHAT_NEXT}" != "micro" ]
then
    cp -f "sql/sigs/pgrouting--${MAYOR}.${MINOR}.sig" "sql/sigs/pgrouting--${NEW_MAYOR}.${NEW_MINOR}.sig"
    git add "sql/sigs/pgrouting--${MAYOR}.${NEW_MINOR}.sig"
fi

# --------------------------------------------
# Adding section in release notes & news
# --------------------------------------------
perl -pi -e 's/(pgRouting '"${OLD_VERSION}"' Release Notes)/
pgRouting '"${NEW_VERSION}"' Release Notes
-------------------------------------------------------------------------------

No Changes Yet

$1/g' doc/src/release_notes.rst

# adding to news
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
perl -pi -e 's/'"${OLD_VERSION}"'/'"${NEW_VERSION}"'/g' .github/workflows/boost_version.yml

# --------------------------------------------
# Include file in CMakeLists.txt
# --------------------------------------------

if [ "${WHAT_NEXT}" != "micro" ]
then
    perl -pi -e 's/# add minor here/my \$version_'"${NEW_MAYOR}"'_'"${NEW_MINOR}"' = qr\/\('"${NEW_MAYOR}"'\.'"${NEW_MINOR}"'\.\[\\d\+\]\)\/;\n# add minor here/g' sql/scripts/build-extension-update-files.pl
    perl -pi -e 's/my \$current = (.*)$/my \$current = \$version_'"${NEW_MAYOR}"'\_'"${NEW_MINOR}"';/g' sql/scripts/build-extension-update-files.pl
    perl -pi -e 's/unless \$old_version =~ (.*)\/;$/unless \$old_version =~ $1|\$version_'"${NEW_MAYOR}"'_'"${NEW_MINOR}"'\/;/g' sql/scripts/build-extension-update-files.pl
fi


perl -pi -e 's/# Copyright(.*) v(.*)$/# Copyright$1 v'"${NEW_VERSION}${NEW_KIND}"'/' locale/pot/*.pot
perl -pi -e 's/# Copyright(.*) v(.*)$/# Copyright$1 v'"${NEW_VERSION}${NEW_KIND}"'/' locale/*/*/*.po
