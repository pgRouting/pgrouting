#!/usr/bin/env bash
# /*PGR-GNU*****************************************************************
# File: test_license.sh
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

