#!/bin/bash
# /*PGR-GNU*****************************************************************
# File: test_signatures.sh
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
# This test checks that signatures within mayors
# Call from the root of the repository

DIR=$(git rev-parse --show-toplevel)/sql/sigs

pushd "${DIR}" > /dev/null || exit
# For bash, uses temporary files
mapfile -t SIGNATURES < <(git ls-files "*.sig" | perl -pe 's/pgrouting--(.*)\.sig/$1/')

for s1 in "${SIGNATURES[@]}"
do
    for s2 in "${SIGNATURES[@]}"
    do
        # only comparing lower version with higher version
        if (( $(echo "$s1 >= $s2" | bc -l) )); then continue; fi

        mayor1=$(echo "$s1" | perl -pe 's/([0-9]+).*/$1/')
        mayor2=$(echo "$s2" | perl -pe 's/([0-9]+).*/$1/')

        # comparing within same mayors only
        if [ "$mayor1" != "$mayor2" ]; then continue; fi

        missing+=$(diff "pgrouting--$s1.sig" "pgrouting--$s2.sig" | grep '<')
    done
done

popd > /dev/null || exit

#mylicensecheck doc
error=0
if [[ $missing ]]; then
  echo " ****************************************************"
  echo " *** Found removed signatures"
  echo " ****************************************************"
  echo "$missing"
  error=1
fi

exit $error

