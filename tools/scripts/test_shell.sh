#!/bin/bash
# This file is part of the pgRouting project.
# Copyright (c) 2020-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE

# This runs shellcheck on all sh files


DIR=$(git rev-parse --show-toplevel)

pushd "${DIR}" > /dev/null || exit 1
code="0"

for f in $(git ls-files | grep '\.sh')
do
  if [ "${f}" = "tools/developer/addNewVersionLink.sh" ] ; then
    result=$(shellcheck --exclude=SC2046,SC2016 "${f}")
  elif [ "${f}" = "ci/winnie/build_pgrouting.sh" ] ; then
    result=$(shellcheck --exclude=SC2116,SC2046,SC2016,SC2141,SC2086 "${f}")
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
