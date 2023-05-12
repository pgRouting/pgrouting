#!/bin/bash
# /*PGR-GNU*****************************************************************
# File: test_shell.sh
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
# This runs shellcheck on all sh files


DIR=$(git rev-parse --show-toplevel)

pushd "${DIR}" > /dev/null || exit 1
code="0"

for f in $(git ls-files | grep '\.sh')
do
  if [ "${f}" = "tools/developer/addNewVersionLink.sh" ] ; then
    result=$(shellcheck --exclude=SC2046,SC2016 "${f}")
  elif [ "${f}" = "ci/winnie/build_pgrouting.sh" ] ; then
    result=$(shellcheck --exclude=SC2046,SC2016,SC2141,SC2086 "${f}")
  else
    result=$(shellcheck --exclude=SC2141 "${f}")
  fi

  if [[ $result ]]; then
    echo "$result"
    echo " *** shellcheck found script errors while processing $f"
    code=1
  fi
done
popd || exit 1
exit $code
