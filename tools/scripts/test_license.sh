#!/bin/bash
# This file is part of the pgRouting project.
# Copyright (c) 2020-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE

# This test checks that all source files correctly have license headers

EXCLUDE_LIST="txt|sig|png|jpeg|_static|md|control|html|cfg|gif|result|conf"

mylicensecheck() {
    licensecheck -r --copyright -l 30 --tail 0 -i "$EXCLUDE_LIST" "$1"
}

DIR=$(git rev-parse --show-toplevel)
pushd "${DIR}" > /dev/null || exit
{ mylicensecheck src & mylicensecheck  sql &  mylicensecheck include & mylicensecheck pgtap;}  | grep "No copyright\|UNKNOWN"
missing1=$(mylicensecheck doc | grep "No copyright")
missing2=$(grep --files-without-match 'Creative Commons' doc/*/*.rst)
missing3=$(mylicensecheck docqueries | grep "No copyright")
missing4=$(grep --files-without-match 'Creative Commons' docqueries/*/*.pg)
popd > /dev/null || exit

#mylicensecheck doc
error=0
if [[ $missing ]]; then
  echo " ****************************************************"
  echo " *** Found source files without valid license headers"
  echo " ****************************************************"
  echo "$missing"
  error=1
fi

if [[ $missing1 ]]; then
  echo " ****************************************************"
  echo " *** Found documentation files without copyright"
  echo " ****************************************************"
  echo "$missing1"
  error=1
fi

if [[ $missing2 ]]; then
  echo " ****************************************************"
  echo " *** Found documentation files without valid license headers"
  echo " ****************************************************"
  echo "$missing2"
  error=1
fi

if [[ $missing3 ]]; then
  echo " ****************************************************"
  echo " *** Found docqueries files without copyright"
  echo " ****************************************************"
  echo "$missing3"
  error=1
fi

if [[ $missing4 ]]; then
  echo " ****************************************************"
  echo " *** Found docqueries files without copyright"
  echo " ****************************************************"
  echo "$missing4"
  error=1
fi


exit $error

